#ifndef PROPROOM3D_STDCHOREOGRAPHER_H
#define PROPROOM3D_STDCHOREOGRAPHER_H

#include "AbstractChoreographer.h"


namespace prop3
{
    class PROP3D_EXPORT StdChoreographer : public AbstractChoreographer
    {
    public:
        StdChoreographer();
        virtual ~StdChoreographer();

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) override;
        virtual void update(double dt) override;
        virtual void reset() override;

    private:

    };
}

#endif // PROPROOM3D_CPURAYTRACER_H
