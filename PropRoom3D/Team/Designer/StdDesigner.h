#ifndef PROPROOM3D_STDDESIGNER_H
#define PROPROOM3D_STDDESIGNER_H

#include "AbstractDesigner.h"


namespace prop3
{
    class PROP3D_EXPORT StdDesigner : public AbstractDesigner
    {
    public:
        StdDesigner();
        virtual ~StdDesigner();

        virtual void setup();
        virtual void reset();

        virtual std::shared_ptr<Prop>   createProp();
    };
}

#endif // PROPROOM3D_STDDESIGNER_H
