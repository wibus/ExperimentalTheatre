#include "Material.h"

#include "Ray/Raycast.h"
#include "Ray/RayUtils.h"

#include "UniformStdMaterial.h"


namespace prop3
{
    Material::Material()
    {

    }

    Material::~Material()
    {

    }


    namespace material
    {
        const double AIR_REFRACTIVE_INDEX = 1.0002772;
        const double GLASS_REFRACTIVE_INDEX = 1.50000;
        const double WATER_REFRACTIVE_INDEX = 1.33000;


        const std::shared_ptr<Material> AIR   = createInsulator(color::white, AIR_REFRACTIVE_INDEX,   0.0, 0.0);
        const std::shared_ptr<Material> GLASS = createInsulator(color::white, GLASS_REFRACTIVE_INDEX, 0.0, 0.0);
        const std::shared_ptr<Material> WATER = createInsulator(color::white, WATER_REFRACTIVE_INDEX, 0.0, 0.0);

        const std::shared_ptr<Material> GOLD     = createMetal(glm::dvec3(1.000000, 0.765557, 0.336057));
        const std::shared_ptr<Material> SILVER   = createMetal(glm::dvec3(0.89803,  0.89411,  0.88627));
        const std::shared_ptr<Material> TITANIUM = createMetal(glm::dvec3(0.541931, 0.496791, 0.449419));


        std::shared_ptr<Material> createMetal(
                const glm::dvec3& color)
        {
            UniformStdMaterial* mat = new UniformStdMaterial();

            mat->setOpacity(1.0);
            mat->setConductivity(1.0);
            mat->setRefractiveIndex(1.40);
            mat->setScattering(1.0);
            mat->setColor(color);

            return std::shared_ptr<Material>(mat);
        }

        std::shared_ptr<Material> createInsulator(
                const glm::dvec3 &color,
                double refractiveIndex,
                double opacity,
                double scattering)
        {
            UniformStdMaterial* mat = new UniformStdMaterial();

            mat->setOpacity(opacity);
            mat->setConductivity(0.0);
            mat->setRefractiveIndex(refractiveIndex);
            mat->setScattering(scattering);
            mat->setColor(color);

            return std::shared_ptr<Material>(mat);
        }
    }

    namespace color
    {
        const glm::dvec3 black          = glm::dvec3(0.000, 0.000, 0.000);
        const glm::dvec3 white          = glm::dvec3(1.000, 1.000, 1.000);

        const glm::dvec3 red            = glm::dvec3(1.000, 0.000, 0.000);
        const glm::dvec3 green          = glm::dvec3(0.000, 1.000, 0.000);
        const glm::dvec3 blue           = glm::dvec3(0.000, 0.000, 1.000);

        const glm::dvec3 cyan           = glm::dvec3(0.000, 1.000, 1.000);
        const glm::dvec3 magenta        = glm::dvec3(1.000, 0.000, 1.000);
        const glm::dvec3 yellow         = glm::dvec3(1.000, 1.000, 0.000);
    }
}
