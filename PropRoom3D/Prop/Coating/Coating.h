#ifndef PROPROOM3D_COATING_H
#define PROPROOM3D_COATING_H

#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;
    class RayHitReport;
    class Material;


    class PROP3D_EXPORT Coating : public Node
    {
    protected:
        Coating();

    public:
        virtual ~Coating();

        virtual glm::dvec4 indirectBrdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const = 0;

        virtual glm::dvec4 directBrdf(
            const RayHitReport& report,
            const glm::dvec3& outDirection,
            const Material& leavedMaterial,
            const Material& enteredMaterial) const = 0;

        virtual glm::dvec3 albedo(
                const RayHitReport& report) const = 0;


    protected:
        static double computeReflexionRatio(
                double leavedRefractiveIndex,
                double enteredRefractiveIndex,
                const glm::dvec3& incident,
                const glm::dvec3& normal);

        static glm::dvec3 computeRefraction(
                double leavedRefractiveIndex,
                double enteredRefractiveIndex,
                const glm::dvec3& incident,
                const glm::dvec3& normal);
    };


    namespace coating
    {
		PROP3D_EXPORT extern const std::shared_ptr<Coating> CLEAR_POLISH;
		PROP3D_EXPORT extern const std::shared_ptr<Coating> CLEAR_ROUGH;

		PROP3D_EXPORT std::shared_ptr<Coating> createClearCoat(
                double roughness);

		PROP3D_EXPORT std::shared_ptr<Coating> createClearPaint(
                const glm::dvec3& color,
                double roughness);
    }
}

#endif // PROPROOM3D_COATING_H
