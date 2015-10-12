#include "Fog.h"

#include <GLM/gtc/random.hpp>

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"
#include "Ray/RayHitList.h"
#include "../Surface/Sphere.h"
#include "../../StageSet/StageSetVisitor.h"


namespace prop3
{
    Fog::Fog(const glm::dvec3& color,
             double concentration,
             double radius,
             double refractiveIndex) :
        _color(color),
        _concentration(concentration),
        _radius(radius),
        _fogGeometry(Sphere::sphere(glm::dvec3(), radius)),
        _randomEngine(std::random_device()())
    {
        setRefractiveIndex(refractiveIndex);
    }

    Fog::~Fog()
    {

    }

    void Fog::indirectBrdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        indirectSpecularRefraction(
            raycasts,
            report,
            leavedMaterial,
            selfEnteredMaterial);
    }

    glm::dvec3 Fog::directBrdf(
        const RayHitReport& report,
        const glm::dvec3& outDirection,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& enteredMaterial) const
    {
        return directSpecularRefraction(report, outDirection);
    }

    double Fog::lightFreePathLength(
        const Raycast& ray) const
    {
        if(_concentration == 0.0)
        {
            return ray.limit;
        }
        else
        {
            double enterDist;
            double distInFog = getDistanceInFog(ray, enterDist);

            if(distInFog <= 0.0)
                return ray.limit;

            std::exponential_distribution<> distrib(_concentration);
            double distance = distrib(_randomEngine);

            if(distance > distInFog)
                return ray.limit;

            return enterDist + distance;
        }
    }

    void Fog::scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const
    {
        size_t preSize = raycasts.size();
        indirectDiffuseScattering(raycasts, ray, self, outRayCountHint);
        size_t postSize = raycasts.size();

        for(size_t i=preSize; i<postSize; ++i)
        {
            raycasts[i].color *= _color;
        }
    }

    glm::dvec3 Fog::gatherLight(
            const Raycast& ray,
            const glm::dvec3& outDirection) const
    {
        return _color * directDiffuseScattering(ray, outDirection);
    }

    void Fog::accept(StageSetVisitor& visitor)
    {
        visitor.visit(*this);
    }

    double Fog::getDistanceInFog(
            const Raycast& ray,
            double& enterDist) const
    {
        double distInFog = 0.0;

        std::vector<RayHitReport*> pool;
        {
            // Rely on RayHitList destructor
            //  to dispose its RayHitReports
            RayHitList list(pool);

            _fogGeometry->raycast(ray, list);
            RayHitReport* report = list.head;
            if(report != nullptr)
            {
                double minDist = report->distance;
                double maxDist = report->distance;
                if(report->_next)
                {
                    minDist = glm::min(minDist, report->_next->distance);
                    minDist = glm::max(minDist, 0.0);

                    maxDist = glm::max(maxDist, report->_next->distance);
                    maxDist = glm::min(maxDist, ray.limit);
                }
                else
                {
                    if(_fogGeometry->isIn(ray.origin) == EPointPosition::IN)
                        minDist = 0.0;
                    else
                        maxDist = ray.limit;
                }

                distInFog = glm::max(maxDist - minDist, 0.0);
                enterDist = minDist;
            }
        }

        RayHitList::releaseMemoryPool(pool);

        return distInFog;
    }
}
