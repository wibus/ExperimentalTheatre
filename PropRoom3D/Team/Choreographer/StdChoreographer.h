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

        virtual void setup();
        virtual void reset();
        virtual void update(double dt);

        virtual void setGravity(const glm::dvec3& unitsPerSecondSquared);

        virtual void manageProp(const std::shared_ptr<Prop>& prop);
        virtual void unmanageProp(const std::shared_ptr<Prop>& prop);

    private:

    };
}

#endif // PROPROOM3D_CPURAYTRACER_H
