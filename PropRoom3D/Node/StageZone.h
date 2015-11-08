#ifndef PROPROOM3D_STAGEZONE_H
#define PROPROOM3D_STAGEZONE_H

#include <GLM/glm.hpp>

#include "HandleNode.h"


namespace prop3
{
    class Prop;
    class Surface;


    class PROP3D_EXPORT StageZone : public HandleNode
    {
    public:
        StageZone(const std::string& name);
        virtual ~StageZone();


        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual std::vector<std::shared_ptr<Node>> children() const;


        // Bounds
        std::shared_ptr<Surface> bounds() const;
        virtual void setBounds(const std::shared_ptr<Surface>& bounds);

        // Props
        std::vector<std::shared_ptr<Prop>> props() const;
        virtual void addProp(const std::shared_ptr<Prop>& prop);

        // Sub-zones
        std::vector<std::shared_ptr<StageZone>> subzones() const;
        virtual void addSubzone(const std::shared_ptr<StageZone>& zone);

        // Clear
        virtual void clear() override;


        // Transformations
        virtual void transform(const glm::dmat4& mat) override;
        virtual void translate(const glm::dvec3& dis) override;
        virtual void rotate(double angle, const glm::dvec3& axis) override;
        virtual void scale(double coeff) override;


        static const std::shared_ptr<Surface> UNBOUNDED;


    private:
        std::shared_ptr<Surface> _bounds;
        std::vector<std::shared_ptr<Prop>> _props;
        std::vector<std::shared_ptr<StageZone>> _subzones;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<Surface> StageZone::bounds() const
    {
        return _bounds;
    }

    inline std::vector<std::shared_ptr<Prop>> StageZone::props() const
    {
        return _props;
    }

    inline std::vector<std::shared_ptr<StageZone>> StageZone::subzones() const
    {
        return _subzones;
    }
}

#endif // PROPROOM3D_STAGEZONE_H
