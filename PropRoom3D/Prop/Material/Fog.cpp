#include "Fog.h"

#include <GLM/gtc/random.hpp>

#include "../Ray/Raycast.h"
#include "../Ray/RayUtils.h"
#include "../Ray/RayHitList.h"
#include "../Surface/Sphere.h"
#include "../../Scene/SceneVisitor.h"


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

    void Fog::brdf(
        std::vector<Raycast>& raycasts,
        const RayHitReport& report,
        const std::shared_ptr<Material>& leavedMaterial,
        const std::shared_ptr<Material>& selfEnteredMaterial,
        unsigned int outRayCountHint) const
    {
        specularRefraction(
            raycasts,
            report,
            leavedMaterial,
            selfEnteredMaterial);
    }

    double Fog::lightFreePathLength(
        const Ray& ray) const
    {
        if(_concentration == 0.0)
        {
            return Ray::BACKDROP_DISTANCE;
        }
        else
        {
            double enterDist;
            double distInFog = getDistanceInFog(ray, ray.limit, enterDist);

            if(distInFog <= 0.0)
                return Ray::BACKDROP_DISTANCE;

            std::exponential_distribution<> distrib(_concentration);
            double distance = distrib(_randomEngine);

            if(distance > distInFog)
                return Ray::BACKDROP_DISTANCE;

            return enterDist + distance;
        }
    }

    void Fog::scatterLight(
            std::vector<Raycast>& raycasts,
            const Ray& ray, double distance,
            const std::shared_ptr<Material>& self,
            unsigned int outRayCountHint) const
    {
        glm::dvec3 origin = ray.origin + ray.direction * distance;
        for(unsigned int i=0; i < outRayCountHint; ++i)
        {
            Ray scatter(origin, glm::sphericalRand(1.0));
            raycasts.push_back(Raycast(scatter, _color, self));
        }
    }

    void Fog::accept(SceneVisitor& visitor)
    {
        visitor.visit(*this);
    }

    double Fog::getDistanceInFog(const Ray& ray, double rayDistance) const
    {
        double enterDist;
        return getDistanceInFog(ray, rayDistance, enterDist);
    }

    double Fog::getDistanceInFog(
            const Ray& ray,
            double rayDistance,
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
                double maxDist = report->distance;
                double minDist = report->distance;
                if(report->_next)
                {
                    maxDist = glm::max(maxDist, report->_next->distance);
                    minDist = glm::min(minDist, report->_next->distance);
                    minDist = glm::max(minDist, 0.0);
                }
                else
                {
                    minDist = 0.0;
                }

                maxDist = glm::min(maxDist, rayDistance);
                distInFog = glm::max(maxDist - minDist, 0.0);
                enterDist = minDist;
            }
        }

        RayHitList::releaseMemoryPool(pool);

        return distInFog;
    }
}
