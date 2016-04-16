#ifndef PROPROOM3D_PROP_H
#define PROPROOM3D_PROP_H

#include <memory>
#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/Node/HandleNode.h>


namespace prop3
{
    class Surface;
    class Coating;
    class Material;


    class PROP3D_EXPORT Prop : public HandleNode
    {
    public:
        Prop(const std::string& name);
        virtual ~Prop();


        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual std::vector<std::shared_ptr<Node>> children() const override;


        // Surface
        const std::vector<std::shared_ptr<Surface>>& surfaces() const;
        virtual void addSurface(const std::shared_ptr<Surface>& surface);

        virtual void setCoating(const std::shared_ptr<Coating>& coating);
        virtual void setInnerMaterial(const std::shared_ptr<Material>& mat);
        virtual void setOuterMaterial(const std::shared_ptr<Material>& mat);


        // Clear
        virtual void clear() override;

        // Transformations
        virtual void transform(const glm::dmat4& mat) override;
        virtual void translate(const glm::dvec3& dis) override;
        virtual void rotate(double angle, const glm::dvec3& axis) override;
        virtual void scale(double coeff) override;


    private:
        std::vector<std::shared_ptr<Surface>> _surfaces;
    };



    // IMPLEMENTATION //
    inline const std::vector<std::shared_ptr<Surface> >& Prop::surfaces() const
    {
        return _surfaces;
    }
}

#endif // PROPROOM3D_PROP_H
