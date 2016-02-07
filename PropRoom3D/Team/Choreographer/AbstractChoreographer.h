#ifndef PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
#define PROPROOM3D_ABSTRACTCHOREOGRAPHER_H

#include <memory>

#include <GLM/glm.hpp>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class StageSet;


    class PROP3D_EXPORT AbstractChoreographer
    {
    protected:
        AbstractChoreographer() {}

    public:
        virtual ~AbstractChoreographer() {}

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) =0;
        virtual void update(double dt) =0;
        virtual void terminate() =0;
    };
}

#endif // PROPROOM3D_ABSTRACTCHOREOGRAPHER_H
