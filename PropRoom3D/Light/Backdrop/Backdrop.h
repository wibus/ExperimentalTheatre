#ifndef PROPROOM3D_BACKDROP_H
#define PROPROOM3D_BACKDROP_H

#include <GLM/glm.hpp>

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;
    class Raycast;
    class RayHitList;


    class PROP3D_EXPORT Backdrop : public Node
    {
    protected:
        Backdrop(bool isDirectlyVisible = true);

    public:
        virtual ~Backdrop();

        virtual glm::dvec3 raycast(const Raycast& ray, bool directView) const = 0;

        virtual std::vector<Raycast> fireRays(unsigned int count) const = 0;

        virtual std::vector<Raycast> fireOn(const glm::dvec3& pos, unsigned int count) const = 0;

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
