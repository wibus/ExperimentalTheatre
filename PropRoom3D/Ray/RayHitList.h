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
            double distance,
            const Raycast& incidentRay,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating);
        void add(RayHitReport* report);
        void clear();

        static void releaseMemoryPool(std::vector<RayHitReport*>& pool);

        RayHitReport* head;
        std::vector<RayHitReport*>& memoryPool;
    };



    // IMPLEMENTATION //
    inline void RayHitList::add(
            double distance,
            const Raycast& incidentRay,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating)
    {
        RayHitReport* report;
        if(memoryPool.empty())
        {
            report = new RayHitReport(
                        distance,
                        incidentRay,
                        position,
                        normal,
                        texCoord,
                        coating);
        }
        else
        {
            report = memoryPool.back();

            report->distance = distance;
            report->incidentRay = incidentRay;
            report->position = position;
            report->normal = normal;
            report->texCoord = texCoord;
            report->coating = coating;

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
