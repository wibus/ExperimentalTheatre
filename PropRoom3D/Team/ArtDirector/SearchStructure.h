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


        bool isEmpty() const;

        std::shared_ptr<AbstractTeam> team() const;

        const std::vector<std::shared_ptr<const LightBulb>>& lights() const;


    private:
        // Main Structures
        std::shared_ptr<AbstractTeam> _team;
        std::vector<SearchZone> _searchZones;
        std::vector<std::shared_ptr<const Surface>> _searchSurfaces;
        std::vector<std::shared_ptr<const LightBulb>> _searchLights;
    };



    // IMPLEMENTATION //
    inline bool SearchStructure::isEmpty() const
    {
        return _searchSurfaces.empty();
    }

    inline std::shared_ptr<AbstractTeam> SearchStructure::team() const
    {
        return _team;
    }

    inline const std::vector<std::shared_ptr<const LightBulb>>& SearchStructure::lights() const
    {
        return _searchLights;
    }
}

#endif // PROPROOM3D_SEARCHSTRUCTURE_H
