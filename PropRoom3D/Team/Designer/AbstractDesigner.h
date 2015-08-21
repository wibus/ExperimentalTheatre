#ifndef PROPROOM3D_ABSTRACTDESIGNER_H
#define PROPROOM3D_ABSTRACTDESIGNER_H

#include <memory>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Prop;
    class Environment;


    class PROP3D_EXPORT AbstractDesigner
    {
    protected:
        AbstractDesigner() {}

    public:
        virtual ~AbstractDesigner() {}

        virtual void setup() =0;
        virtual void reset() =0;

        virtual std::shared_ptr<Prop> createProp() = 0;

        virtual std::shared_ptr<Environment> createEnvironment() = 0;
    };
}

#endif // PROPROOM3D_ABSTRACTDESIGNER_H
