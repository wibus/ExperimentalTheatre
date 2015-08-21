#ifndef PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM3D_ABSTRACTCHOREOGRAPHER_H

#include <memory>

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Prop;
    class Scene;
    class Environment;


    class PROP3D_EXPORT AbstractChoreographer
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void setup() =0;
        virtual void reset() =0;
        virtual void update(double dt) =0;

        virtual void manageProp(const std::shared_ptr<Prop>& prop) =0;
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop) =0;

        virtual void setEnvironment(const std::shared_ptr<Environment>& env) = 0;
    };
}

#endif // PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
