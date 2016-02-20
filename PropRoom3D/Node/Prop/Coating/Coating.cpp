#include "Coating.h"

#include "UniformStdCoating.h"


namespace prop3
{
    Coating::Coating()
    {

    }

    Coating::~Coating()
    {

    }

    double Coating::computeReflexionRatio(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal)
    {
        // Schlick's approximation
        double R0 = (leavedRefractiveIndex - enteredRefractiveIndex) /
                    (leavedRefractiveIndex + enteredRefractiveIndex);
        R0 = R0 * R0;

        double cosNV = 1.0 - glm::abs(glm::dot(incident, normal));
        double cosNV2 = cosNV * cosNV;
        double cosNV5 = cosNV2 * cosNV2 * cosNV;
        return R0 + (1.0 - R0) * cosNV5;
    }

    glm::dvec3 Coating::computeRefraction(
            double leavedRefractiveIndex,
            double enteredRefractiveIndex,
            const glm::dvec3& incident,
            const glm::dvec3& normal)
    {
        double eta = leavedRefractiveIndex / enteredRefractiveIndex;
        double cosI = glm::dot(normal, incident);
        double sinT2 = eta * eta * (1.0 - cosI * cosI);

        if(sinT2 <= 1.0)
        {
            double normFact = (eta * cosI + sqrt(1.0 - sinT2));
            return eta * incident - normFact * normal;
        }
        else
        {
            return incident - 2.0 * cosI * normal;
        }
    }


    namespace coating
    {
        const std::shared_ptr<Coating> CLEAR_POLISH = createClearCoat(0.0);
        const std::shared_ptr<Coating> CLEAR_ROUGH  = createClearCoat(1.0);

        std::shared_ptr<Coating> createClearCoat(
                double roughness)
        {
            UniformStdCoating* coat = new UniformStdCoating();
            coat->setRoughness(roughness);
            coat->setPaintColor(glm::dvec4(0));
            coat->setPaintRefractiveIndex(1.0);
            return std::shared_ptr<Coating>(coat);
        }

        std::shared_ptr<Coating> createClearPaint(
                const glm::dvec3& color,
                double roughness,
                double opacity)
        {
            UniformStdCoating* coat = new UniformStdCoating();
            coat->setRoughness(roughness);
            coat->setPaintColor(glm::dvec4(color, opacity));
            coat->setPaintRefractiveIndex(1.55);
            return std::shared_ptr<Coating>(coat);
        }
    }
}
