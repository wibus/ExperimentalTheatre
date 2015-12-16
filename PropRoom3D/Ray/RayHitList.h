#ifndef PROPROOM3D_RAYHITLIST_H
#define PROPROOM3D_RAYHITLIST_H

#include <vector>

#include "RayHitReport.h"


namespace prop3
{
    class RayHitList
    {
    public:
        RayHitList();
        ~RayHitList();

        void add(
            double length,
            const Raycast& incidentRay,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating,
            const Material* innerMat,
            const Material* outerMat);
        void add(RayHitReport* report);
        void clear();

        void dispose(RayHitReport* node);

        void releaseMemoryPool();

        RayHitReport* head;

    private:
        std::vector<RayHitReport*> _memoryPool;
    };



    // IMPLEMENTATION //
    inline void RayHitList::add(
            double length,
            const Raycast& incidentRay,
            const glm::dvec3& position,
            const glm::dvec3& normal,
            const glm::dvec3& texCoord,
            const Coating* coating,
            const Material* innerMat,
            const Material* outerMat)
    {
        RayHitReport* report;
        if(_memoryPool.empty())
        {
            report = new RayHitReport(
                        length,
                        incidentRay,
                        position,
                        normal,
                        texCoord,
                        coating,
                        innerMat,
                        outerMat);
        }
        else
        {
            report = _memoryPool.back();

            report->length = length;
            report->incidentRay = incidentRay;
            report->position = position;
            report->normal = normal;
            report->texCoord = texCoord;
            report->coating = coating;
            report->innerMat = innerMat;
            report->outerMat = outerMat;

            _memoryPool.pop_back();
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
            _memoryPool.push_back(report);
            report = report->_next;
        }
        head = nullptr;
    }

    inline void RayHitList::dispose(RayHitReport* node)
    {
        _memoryPool.push_back(node);
    }
}

#endif // PROPROOM3D_RAYHITLIST_H
