#ifndef PROPROOM3D_MATERIAL_H
#define PROPROOM3D_MATERIAL_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;
    class RayHitReport;


    class PROP3D_EXPORT Material : public Node
    {
    protected:
        Material();

    public:
        virtual ~Material();


        // OPTICAL PROPERTIES
        /// 0.0 -> Fully opaque (diffuse surface reflection)
        /// 1.0 -> Fully transparent (specular surface refraction)
        virtual double opacity(const glm::dvec3& pos) const = 0;

        /// 0.0 -> Perfect insulator (no metallic reflection ar the surface)
        /// 1.0 -> Perfect conductor (give rise to mettalic reflexions)
        virtual double conductivity(const glm::dvec3& pos) const = 0;

        /// 1.0 -> Vacuum refractive index
        /// x.x -> Material's mean refractive index in the viible spectrum
        virtual double refractiveIndex(const glm::dvec3& pos) const = 0;

        /// 0.0 -> Material is perfectly homogeneous (no internal light scattering)
        /// 1.0 -> Fully diffuse scattering
        virtual double scattering(const glm::dvec3& pos) const = 0;

        /// (0, 0, 0) -> Total absorption of light's wavelength (red, green, blue)
        /// (1, 1, 1) -> Total reflexion of light's wavelength (red, green, blue)
        virtual glm::dvec3 color(const glm::dvec3& pos) const = 0;


        virtual double lightFreePathLength(
            const Raycast& ray) const = 0;

        virtual glm::dvec3 lightAttenuation(
            const Raycast& ray) const = 0;

        virtual void scatterLight(
            std::vector<Raycast>& raycasts,
            const Raycast& ray) const = 0;
    };


    namespace color
    {
        PROP3D_EXPORT const extern glm::dvec3 black;
        PROP3D_EXPORT const extern glm::dvec3 white;

        PROP3D_EXPORT const extern glm::dvec3 red;
        PROP3D_EXPORT const extern glm::dvec3 green;
        PROP3D_EXPORT const extern glm::dvec3 blue;

        PROP3D_EXPORT const extern glm::dvec3 cyan;
        PROP3D_EXPORT const extern glm::dvec3 magenta;
        PROP3D_EXPORT const extern glm::dvec3 yellow;

        PROP3D_EXPORT const extern glm::dvec3 candleFlame;
        PROP3D_EXPORT const extern glm::dvec3 sodiumVapor;
        PROP3D_EXPORT const extern glm::dvec3 tungsten40W;
        PROP3D_EXPORT const extern glm::dvec3 tungsten100W;
    }


    // Default materials
    namespace material
    {
        /////////////////////////
        // Constant attributes //
        /////////////////////////
		PROP3D_EXPORT extern const double AIR_REFRACTIVE_INDEX;
		PROP3D_EXPORT extern const double GLASS_REFRACTIVE_INDEX;
		PROP3D_EXPORT extern const double WATER_REFRACTIVE_INDEX;


        ///////////////////////
        // Classic materials //
        ///////////////////////

        // Clear materials
		PROP3D_EXPORT extern const std::shared_ptr<Material> AIR;
		PROP3D_EXPORT extern const std::shared_ptr<Material> GLASS;
		PROP3D_EXPORT extern const std::shared_ptr<Material> WATER;

        // Metals
		PROP3D_EXPORT extern const std::shared_ptr<Material> GOLD;
		PROP3D_EXPORT extern const std::shared_ptr<Material> SILVER;
		PROP3D_EXPORT extern const std::shared_ptr<Material> TITANIUM;

        // Minerals


        /////////////////////
        // Factory methods //
        /////////////////////
		PROP3D_EXPORT std::shared_ptr<Material> createMetal(
                const glm::dvec3& color);

		PROP3D_EXPORT std::shared_ptr<Material> createInsulator(
                const glm::dvec3& color,
                double refractiveIndex,
                double opacity,
                double scattering);
    }
}

#endif // PROPROOM3D_MATERIAL_H
