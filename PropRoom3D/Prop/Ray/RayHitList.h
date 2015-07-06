#ifndef PROPROOM3D_RAYHITLIST_H
#define PROPROOM3D_RAYHITLIST_H

#include <vector>

#include "RayHitReport.h"


namespace prop3
{
    class RayHitList
    {
    public:
        RayHitList(std::vector<RayHitReport*>& memoryPool);
        ~RayHitList();

        void add(
            const Ray& ray,
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const Coating* coating,
            const glm::dvec3& texCoord = glm::dvec3());
        void add(RayHitReport* report);
        void clear();

        static void releaseMemoryPool(std::vector<RayHitReport*>& pool);

        RayHitReport* head;
        std::vector<RayHitReport*>& memoryPool;
    };



    // IMPLEMENTATION //
    inline void RayHitList::add(
            const Ray& ray,
            double distance,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const Coating* coating,
            const glm::dvec3& texCoord)
    {
        RayHitReport* report;
        if(memoryPool.empty())
        {
            report = new RayHitReport(
                        ray,
                        distance,
                        position,
                        normal,
                        coating,
                        texCoord);
        }
        else
        {
            report = memoryPool.back();

            report->ray = ray;
            report->distance = distance;
            report->position = position;
            report->normal = normal;
            report->coating = coating;
            report->texCoord = texCoord;

            memoryPool.pop_back();
        }

        add(report);
    }

    inline void RayHitList::add(RayHitReport* report)
    {
        report->_next = head;
        head = report;
    }

    inline void RayHitList::clear()
    {
        RayHitReport* report = head;
        while(report != nullptr)
        {
            memoryPool.push_back(report);
            report = report->_next;
        }
        head = nullptr;
    }
}

#endif // PROPROOM3D_RAYHITLIST_H
