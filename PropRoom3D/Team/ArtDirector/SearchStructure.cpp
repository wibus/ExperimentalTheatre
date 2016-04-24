#include "SearchStructure.h"

#include <atomic>
#include "Team/DummyTeam.h"

#include "Node/StageSet.h"
#include "Node/Prop/Prop.h"
#include "Node/Prop/Surface/Surface.h"
#include "Node/Light/LightBulb/LightBulb.h"

#include "Serial/JsonReader.h"

#include "Ray/RayHitList.h"


namespace prop3
{
    struct SearchZone
    {
        size_t parent;
        size_t endZone;
        size_t begSurf;
        size_t endSurf;
        Surface* bounds;
    };

    struct SearchSurface
    {
        SearchSurface(const std::shared_ptr<Surface>& surface) :
            surface(surface), hitCount(0) {}

        SearchSurface(const SearchSurface& search) :
            surface(search.surface), hitCount(search.hitCount.load()) {}

        SearchSurface(SearchSurface&& search) :
            surface(search.surface), hitCount(search.hitCount.load()) {}

        inline Surface* operator -> () const {return surface.get();}

        std::shared_ptr<Surface> surface;
        mutable std::atomic_long hitCount;
    };


    SearchStructure::SearchStructure(const std::string &stageStream) :
        _team(new DummyTeam()),
        _isOptimized(false)
    {
        _team->setup();

        StageSetJsonReader reader;
        reader.deserialize(*_team, stageStream);
        std::shared_ptr<StageSet> stageSet = _team->stageSet();

        _searchZones.clear();
        _searchSurfaces.clear();

        if(!stageSet->isVisible())
            return;

        std::vector<std::pair<StageZone*, size_t>> zoneStack;
        zoneStack.push_back(std::make_pair(stageSet.get(), -1));
        while(!zoneStack.empty())
        {
            StageZone* zone = zoneStack.back().first;
            size_t parentId = zoneStack.back().second;
            zoneStack.pop_back();

            size_t addedSubzones = 0;
            for(size_t s=0; s < zone->subzones().size(); ++s)
            {
                StageZone* subz = zone->subzones()[s].get();

                if(!subz->isVisible())
                    continue;

                // Bubble up unbounded subzones
                if(subz->bounds().get() == StageZone::UNBOUNDED.get())
                {
                    // Bubble up props
                    size_t propCount = subz->props().size();
                    for(size_t p=0; p < propCount; ++p)
                    {
                        auto prop = subz->props()[p];
                        if(prop->isVisible())
                            zone->addProp(subz->props()[p]);
                    }

                    // Bubble up lights
                    size_t lightCount = subz->lights().size();
                    for(size_t l=0; l < lightCount; ++l)
                    {
                        auto light = subz->lights()[l];
                        if(light->isVisible())
                            zone->addLight(light);
                    }

                    // Bubble up subzones
                    size_t subzCount = subz->subzones().size();
                    for(size_t z=0; z < subzCount; ++z)
                    {
                        auto subsubz = subz->subzones()[z];
                        if(subsubz->isVisible())
                            zone->addSubzone(subsubz);
                    }
                }
                else
                {
                    size_t currId = _searchZones.size();
                    zoneStack.push_back(std::make_pair(subz, currId));
                    ++addedSubzones;
                }
            }


            size_t startSurfCount = _searchSurfaces.size();

            size_t lightCount = zone->lights().size();
            for(size_t l=0; l < lightCount; ++l)
            {
                auto light = zone->lights()[l];
                if(light->isVisible())
                {
                    _searchSurfaces.emplace_back(light->surface());

                    if(light->isOn())
                        _lights.push_back(light);
                }
            }

            size_t propCount = zone->props().size();
            for(size_t p=0; p < propCount; ++p)
            {
                auto prop = zone->props()[p];
                if(prop->isVisible())
                {
                    size_t surfCount = prop->surfaces().size();
                    for(size_t s=0; s < surfCount; ++s)
                    {
                        _searchSurfaces.emplace_back(
                            prop->surfaces()[s]);
                    }
                }
            }
            size_t endSurfCount = _searchSurfaces.size();
            size_t addedSurfaces = endSurfCount - startSurfCount;


            if(addedSubzones == 0 && addedSurfaces == 0)
                continue;

            SearchZone searchZone;
            searchZone.parent = parentId;
            searchZone.endZone = _searchZones.size() + 1;
            searchZone.endSurf = _searchSurfaces.size();
            searchZone.begSurf = searchZone.endSurf - addedSurfaces;
            searchZone.bounds = zone->bounds().get();
            _searchZones.push_back(searchZone);
        }

        int last = int(_searchZones.size() - 1);
        for(int i = last; i >= 0; --i)
        {
            const SearchZone& zone = _searchZones[i];
            if(zone.parent != -1)
            {
                SearchZone& parent = _searchZones[zone.parent];
                parent.endZone = glm::max(parent.endZone, zone.endZone);
            }
        }
    }

    SearchStructure::~SearchStructure()
    {

    }

    double SearchStructure::findNearestIntersection(
            const Raycast& raycast,
            RayHitReport& reportMin,
            RayHitList& rayHitList) const
    {
        Raycast ray(raycast);

        size_t minId = -1;

        size_t zId = 0;
        size_t zoneCount = _searchZones.size();
        while(zId < zoneCount)
        {
            const SearchZone& zone = _searchZones[zId];

            if(zone.bounds == StageZone::UNBOUNDED.get() ||
               zone.bounds->intersects(ray, rayHitList))
            {
                for(size_t s = zone.begSurf; s < zone.endSurf; ++s)
                {
                    rayHitList.clear();

                    _searchSurfaces[s]->raycast(ray, rayHitList);

                    RayHitReport* node = rayHitList.head;
                    while(node != nullptr)
                    {
                        if(0.0 < node->length && node->length < ray.limit)
                        {
                            ray.limit = node->length;
                            reportMin = *node;
                            minId = s;
                        }

                        node = node->_next;
                    }
                }

                ++zId;
            }
            else
            {
                zId = zone.endZone;
            }
        }

        if(!_isOptimized && reportMin.length != raycast.limit)
        {
            incrementCounter(_searchSurfaces[minId], ray.entropy);
        }

        return reportMin.length;
    }

    bool SearchStructure::intersectsScene(
            const Raycast& raycast,
            RayHitList& rayHitList,
            double incomingEntropy) const
    {
        rayHitList.clear();

        size_t zId = 0;
        size_t zoneCount = _searchZones.size();
        while(zId < zoneCount)
        {
            const SearchZone& zone = _searchZones[zId];

            if(zone.bounds == StageZone::UNBOUNDED.get() ||
               zone.bounds->intersects(raycast, rayHitList))
            {
                for(size_t s = zone.begSurf; s < zone.endSurf; ++s)
                {
                    if(_searchSurfaces[s]->intersects(raycast, rayHitList))
                    {
                        if(!_isOptimized)
                            incrementCounter(_searchSurfaces[s],
                                             incomingEntropy);

                        return true;
                    }
                }

                ++zId;
            }
            else
            {
                zId = zone.endZone;
            }
        }

        return false;
    }

    void SearchStructure::removeHiddenSurfaces(
            int threshold,
            size_t& removedZones,
            size_t& removedSurfaces)
    {
        removedZones = 0;
        removedSurfaces = 0;

        bool invertRemoval = (threshold < 0);
        threshold = glm::abs(threshold);


        std::vector<bool> removeSurface;
        removeSurface.reserve(_searchSurfaces.size());
        for(size_t i=0; i < _searchSurfaces.size(); ++i)
        {
            bool remove = _searchSurfaces[i].hitCount
                .load(std::memory_order_relaxed) < threshold;

            if(invertRemoval)
                remove = !remove;

            if(remove) ++removedSurfaces;
            removeSurface[i] = remove;
        }


        std::vector<bool> removeZone;
        removeZone.reserve(_searchZones.size());
        for(int i=_searchZones.size()-1; i > 0; --i)
        {
            SearchZone& zone = _searchZones[i];

            bool remove = true;
            for(size_t z=i+1; z < zone.endZone; ++z)
                remove = remove && removeZone[z];
            for(size_t s=zone.begSurf; s < zone.endSurf; ++s)
                remove = remove && removeSurface[s];

            if(remove)++removedZones;
            removeZone[i] = remove;
        }

        std::vector<SearchZone> newZones;
        std::vector<SearchSurface> newSurfs;

        size_t cumZoneRemove = 0;
        size_t cumSurfRemove = 0;

        for(int i=0; i < _searchZones.size(); ++i)
        {
            SearchZone zone = _searchZones[i];

            if(removeZone[i])
            {
                ++cumZoneRemove;
                cumSurfRemove += zone.endSurf - zone.begSurf;
            }
            else
            {
                size_t preCumSurfRemove = cumSurfRemove;
                for(size_t s=zone.begSurf; s < zone.endSurf; ++s)
                {
                    if(removeSurface[s])
                        ++cumSurfRemove;
                    else
                        newSurfs.push_back(_searchSurfaces[s]);
                }
                zone.begSurf -= preCumSurfRemove;
                zone.endSurf -= cumSurfRemove;


                for(size_t z=i+1; z < zone.endZone; ++z)
                {
                    if(removeZone[z])
                        --zone.endZone;
                }
                zone.endZone -= cumZoneRemove;

                newZones.push_back(zone);
            }
        }

        std::swap(_searchZones, newZones);
        std::swap(_searchSurfaces, newSurfs);
        _isOptimized = true;
    }

    void SearchStructure::resetHitCounters()
    {
        for(SearchSurface& surf : _searchSurfaces)
            surf.hitCount.store(0);
    }

    void SearchStructure::incrementCounter(
            const SearchSurface& surf,
            double entropy) const
    {
        surf.hitCount.fetch_add(1 + (1.0-entropy) * 99);
    }
}
