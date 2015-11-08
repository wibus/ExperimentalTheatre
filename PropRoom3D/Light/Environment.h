#ifndef PROPROOM3D_ENVIRONMENT_H
#define PROPROOM3D_ENVIRONMENT_H

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Backdrop;
    class Material;


    class PROP3D_EXPORT Environment : public Node
    {
    public:
        Environment();
        virtual ~Environment();


        virtual void accept(Visitor& visito) override;

        virtual std::vector<std::shared_ptr<Node>> children() const override;


        std::shared_ptr<Backdrop> backdrop() const;

        virtual void setBackdrop(const std::shared_ptr<Backdrop>& backdrop);


        std::shared_ptr<Material> ambientMaterial() const;

        virtual void setAmbientMaterial(const std::shared_ptr<Material>& ambientMaterial);


    private:
        std::shared_ptr<Backdrop> _backdrop;
        std::shared_ptr<Material> _ambientMaterial;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<Backdrop> Environment::backdrop() const
    {
        return _backdrop;
    }

    inline std::shared_ptr<Material> Environment::ambientMaterial() const
    {
        return _ambientMaterial;
    }
}

#endif // PROPROOM3D_ENVIRONMENT_H
