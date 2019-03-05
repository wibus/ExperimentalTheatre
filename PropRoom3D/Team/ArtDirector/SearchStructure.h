#ifndef PROPROOM3D_SEARCHSTRUCTURE_H
#define PROPROOM3D_SEARCHSTRUCTURE_H

#include <vector>
#include <memory>
#include <atomic>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Surface;
    class LightBulb;

    class Raycast;
    class RayHitList;
    class RayHitReport;

    class AbstractTeam;


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

		inline Surface* operator -> () const { return surface.get(); }

		inline SearchSurface& operator=(const SearchSurface& ss)
		{
			surface = ss.surface;
			hitCount.store(ss.hitCount.load());
			return *this;
		}

		std::shared_ptr<Surface> surface;
		mutable std::atomic_long hitCount;
	};

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
                RayHitList& rayHitList,
                double incomingEntropy) const;

        void removeHiddenSurfaces(
                int threshold,
                size_t& removedZones,
                size_t& removedSurfaces);

        void resetHitCounters();


        bool isEmpty() const;

        bool isOptimized() const;

        std::shared_ptr<AbstractTeam> team() const;

        const std::vector<std::shared_ptr<const LightBulb>>& lights() const;


    protected:
        void incrementCounter(
                const SearchSurface& surf,
                double entropy) const;

    private:
        // Main Structures
        std::shared_ptr<AbstractTeam> _team;
        std::vector<SearchZone> _searchZones;
        std::vector<SearchSurface> _searchSurfaces;

        bool _isEmpty;
        bool _isOptimized;
        std::vector<std::shared_ptr<const LightBulb>> _lights;
    };



    // IMPLEMENTATION //
    inline bool SearchStructure::isEmpty() const
    {
        return _isEmpty;
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
