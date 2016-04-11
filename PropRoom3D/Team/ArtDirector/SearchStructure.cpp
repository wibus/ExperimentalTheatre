#include "SearchStructure.h"

#include "Team/AbstractTeam.h"

#include "Node/StageSet.h"
#include "Node/Prop/Prop.h"
#include "Node/Prop/Surface/Surface.h"
#include "Node/Light/LightBulb/LightBulb.h"

#include "Serial/JsonReader.h"

#include "Ray/RayHitList.h"


namespace prop3
{
    class WorkerTeam : public AbstractTeam
    {
    public:
        WorkerTeam() :
            AbstractTeam(nullptr /* Choreographer */)
        {
        }
    };

    struct SearchZone
    {
        size_t parent;
        size_t endZone;
        size_t begLight;
        size_t endLight;
        size_t begSurf;
        size_t endSurf;
        Surface* bounds;
    };

    SearchStructure::SearchStructure(const std::string &stageStream) :
        _team(new WorkerTeam())
    {
        _team->setup();

        StageSetJsonReader reader;
        reader.deserialize(*_team, stageStream);
        std::shared_ptr<StageSet> stageSet = _team->stageSet();

        _searchZones.clear();
        _searchSurfaces.clear();
        _searchLights.clear();

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


            size_t lightCount = zone->lights().size();
            size_t startLightCount = _searchLights.size();
            for(size_t l=0; l < lightCount; ++l)
            {
                auto light = zone->lights()[l];
                if(light->isVisible())
                    _searchLights.push_back(light);
            }
            size_t endLightCount = _searchLights.size();
            size_t addedLights = endLightCount - startLightCount;


            size_t propCount = zone->props().size();
            size_t startSurfCount = _searchSurfaces.size();
            for(size_t p=0; p < propCount; ++p)
            {
                auto prop = zone->props()[p];
                if(prop->isVisible())
                {
                    size_t surfCount = prop->surfaces().size();
                    for(size_t s=0; s < surfCount; ++s)
                    {
                        _searchSurfaces.push_back(prop->surfaces()[s]);
                    }
                }
            }
            size_t endSurfCount = _searchSurfaces.size();
            size_t addedSurfaces = endSurfCount - startSurfCount;


            if(addedSubzones == 0 && addedLights == 0 && addedSurfaces == 0)
                continue;

            SearchZone searchZone;
            searchZone.parent = parentId;
            searchZone.endZone = _searchZones.size() + 1;
            searchZone.endLight = _searchLights.size();
            searchZone.begLight = searchZone.endLight - addedLights;
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
                        }

                        node = node->_next;
                    }
                }

                for(size_t l = zone.begLight; l < zone.endLight; ++l)
                {
                    rayHitList.clear();

                    _searchLights[l]->raycast(ray, rayHitList);

                    RayHitReport* node = rayHitList.head;
                    while(node != nullptr)
                    {
                        if(0.0 < node->length && node->length < ray.limit)
                        {
                            ray.limit = node->length;
                            reportMin = *node;
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

        return reportMin.length;
    }

    bool SearchStructure::intersectsScene(
            const Raycast& raycast,
            RayHitList& rayHitList) const
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
                        return true;
                }

                for(size_t l = zone.begLight; l < zone.endLight; ++l)
                {
                    if(_searchLights[l]->intersects(raycast, rayHitList))
                        return true;
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
}
