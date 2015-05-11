#ifndef SCAENA_VIEW_H
#define SCAENA_VIEW_H

#include <string>
#include <memory>
#include <functional>

#include <GLM/glm.hpp>

#include "../libScaena_global.h"

namespace cellar
{
    class Camera;
}

namespace prop2
{
    class AbstractArtDirector;
}

namespace prop3
{
    class AbstractArtDirector;
    class PostProdManager;
}


namespace scaena
{
    class Play;


    class SCAENA_EXPORT View
    {
    public:
        View(const std::string& id);
        virtual ~View();

        virtual void draw(double dt,
            const std::function<void()>& customDraw) = 0;

        virtual void setPostProdManager(
            const std::shared_ptr<prop3::PostProdManager>& manager);

        const std::string& id() const;
        virtual glm::ivec2 viewport() const;
        std::shared_ptr<cellar::Camera> camera2D() const;
        std::shared_ptr<cellar::Camera> camera3D() const;

    protected:
        friend class Play;
        virtual void lookAt(const Play& play);

        const std::shared_ptr<prop2::AbstractArtDirector>& artDirector2D() const;
        const std::shared_ptr<prop3::AbstractArtDirector>& artDirector3D() const;
        void setArtDirector2D(const std::shared_ptr<prop2::AbstractArtDirector>& director);
        void setArtDirector3D(const std::shared_ptr<prop3::AbstractArtDirector>& director);


    private:
        std::string _id;
        std::shared_ptr<prop2::AbstractArtDirector> _artDirector2D;
        std::shared_ptr<prop3::AbstractArtDirector> _artDirector3D;
    };



    // IMPLEMENTATION //
    inline const std::string& View::id() const
        {return _id;}

    inline const std::shared_ptr<prop2::AbstractArtDirector>& View::artDirector2D() const
        {return _artDirector2D;}

    inline const std::shared_ptr<prop3::AbstractArtDirector>& View::artDirector3D() const
        {return _artDirector3D;}

    inline void View::setArtDirector2D(const std::shared_ptr<prop2::AbstractArtDirector>& director)
        {_artDirector2D = director;}

    inline void View::setArtDirector3D(const std::shared_ptr<prop3::AbstractArtDirector>& director)
        {_artDirector3D = director;}
}

#endif // SCAENA_VIEW_H
