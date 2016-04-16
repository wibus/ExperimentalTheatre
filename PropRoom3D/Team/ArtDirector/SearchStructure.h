#ifndef PROPROOM3D_SEARCHSTRUCTURE_H
#define PROPROOM3D_SEARCHSTRUCTURE_H

#include <vector>
#include <memory>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Surface;
    class LightBulb;

    struct SearchZone;
    struct SearchSurface;
    struct SearchLightBulb;

    class Raycast;
    class RayHitList;
    class RayHitReport;

    class AbstractTeam;


    class PROP3D_EXPORT SearchStructure
    {
    public:
        SearchStructure(const std::string& stageStream);
        ~SearchStructure();

        double findNearestIntersection(
                const Raycast& raycast,
                RayHitReport& reportMin,
                RayHitList& rayHitList) const;

        bool intersectsScene(
                const Raycast& raycast,
                RayHitList& rayHitList) const;

        void removeHiddenSurfaces(
                size_t threshold,
                size_t& removedZones,
                size_t& removedSurfaces);

        void resetHitCounters();


        bool isEmpty() const;

        bool isOptimized() const;

        std::shared_ptr<AbstractTeam> team() const;

        const std::vector<std::shared_ptr<const LightBulb>>& lights() const;


    private:
        // Main Structures
        std::shared_ptr<AbstractTeam> _team;
        std::vector<SearchZone> _searchZones;
        std::vector<SearchSurface> _searchSurfaces;

        bool _isOptimized;
        std::vector<std::shared_ptr<const LightBulb>> _lights;
    };



    // IMPLEMENTATION //
    inline bool SearchStructure::isEmpty() const
    {
        return _searchSurfaces.empty();
    }

    inline bool SearchStructure::isOptimized() const
    {
        return _isOptimized;
    }

    inline std::shared_ptr<AbstractTeam> SearchStructure::team() const
    {
        return _team;
    }

    inline const std::vector<std::shared_ptr<const LightBulb>>& SearchStructure::lights() const
    {
        return _lights;
    }
}

#endif // PROPROOM3D_SEARCHSTRUCTURE_H
