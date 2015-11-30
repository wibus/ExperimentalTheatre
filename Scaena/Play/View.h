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
}


namespace scaena
{
    class Play;


    class SCAENA_EXPORT View
    {
    public:
        View(const std::string& id);
        virtual ~View();

        const std::string& id() const;
        virtual glm::ivec2 viewport() const = 0;
        virtual std::shared_ptr<cellar::Camera> camera2D() const = 0;
        virtual std::shared_ptr<cellar::Camera> camera3D() const = 0;


    protected:
        friend class Play;

        virtual void install(Play& play) = 0;
        virtual void setup() = 0;

        virtual void beginDraw(double dt) = 0;
        virtual void endDraw(double dt) = 0;


    private:
        std::string _id;
    };



    // IMPLEMENTATION //
    inline const std::string& View::id() const
        {return _id;}
}

#endif // SCAENA_VIEW_H
