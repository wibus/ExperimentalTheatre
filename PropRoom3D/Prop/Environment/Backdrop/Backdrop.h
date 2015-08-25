#ifndef PROPROOM3D_BACKDROP_H
#define PROPROOM3D_BACKDROP_H

#include <GLM/glm.hpp>

#include "PropRoom3D/StageSet/StageSetNode.h"


namespace prop3
{
    class Ray;
    class RayHitList;


    class PROP3D_EXPORT Backdrop : public StageSetNode
    {
    protected:
        Backdrop(bool isDirectlyVisible = true);

    public:
        virtual ~Backdrop();

        virtual glm::dvec3 raycast(const Ray& ray) const = 0;

        bool isDirectlyVisible() const;

        void setIsDirectlyVisible(bool isDirectlyVisible);

    private:
        bool _isDirectlyVisible;
    };



    // IMPLEMENTATION //
    inline bool Backdrop::isDirectlyVisible() const
    {
        return _isDirectlyVisible;
    }
}

#endif // PROPROOM3D_BACKDROP_H
