#ifndef PROPROOM3D_LIGHTBULB_H
#define PROPROOM3D_LIGHTBULB_H

#include <memory>

#include <PropRoom3D/Node/HandleNode.h>


namespace prop3
{
    class Raycast;
    class RayHitList;
    class LightCast;
    class EmissiveCoating;
    class Material;
    class Surface;


    class PROP3D_EXPORT LightBulb : public HandleNode
    {
    protected:
        LightBulb(const std::string& name);

    public:
        virtual ~LightBulb();


        // Clear
        virtual void clear() override;


        // Transformations
        virtual void transform(const glm::dmat4& mat) override;

        virtual void translate(const glm::dvec3& dis) override;

        virtual void rotate(double angle, const glm::dvec3& axis) override;

        virtual void scale(double coeff) override;


        // Light Rays
        virtual void fireOn(
                std::vector<LightCast>& lightCasts,
                const glm::dvec3& pos,
                unsigned int count) const = 0;


        // Surface
        std::shared_ptr<Surface> surface() const;


        // Properties
        virtual double area() const = 0;

        virtual double visibility(const Raycast& ray) const = 0;

        virtual void setIsOn(bool isOn);
        bool isOn() const;

        void setRadiantFlux(const glm::dvec3& radiantFlux);
        glm::dvec3 radiantFlux() const;

        glm::dmat4 transform() const;


    protected:
        void setSurface(const std::shared_ptr<Surface>& surf);

        double diffuseSize(
                const LightCast& lightCast,
                const Raycast& eyeRay,
                double roughness) const;

        void commitTransform(const glm::dmat4& mat);
        virtual void onTransform() = 0;


    protected:
        bool _isOn;
        glm::dvec3 _radiantFlux;
        std::shared_ptr<Surface> _surface;
        std::shared_ptr<EmissiveCoating> _coating;
        glm::dmat4 _invTransform;
        glm::dmat4 _transform;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<Surface> LightBulb::surface() const
    {
        return _surface;
    }

    inline bool LightBulb::isOn() const
    {
        return _isOn;
    }

    inline glm::dvec3 LightBulb::radiantFlux() const
    {
        return _radiantFlux;
    }

    inline glm::dmat4 LightBulb::transform() const
    {
        return _transform;
    }
}

#endif // PROPROOM3D_LIGHT_H
