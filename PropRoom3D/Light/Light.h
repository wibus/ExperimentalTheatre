#ifndef PROPROOM3D_LIGHT_H
#define PROPROOM3D_LIGHT_H

#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;

    //
    // Utilities
    //

    // Kevin degree to RGB color value
    // Perfect white is 6600K
    PROP3D_EXPORT glm::dvec3 kelvinToRgb(int kelvin);


    class PROP3D_EXPORT Light : public Node
    {
    protected:
        Light();

    public:
        virtual ~Light();

        virtual std::vector<Raycast> fireRays(unsigned int count) const = 0;

        virtual std::vector<Raycast> fireOn(const glm::dvec3& pos, unsigned int count) const = 0;

    private:
    };
}

#endif // PROPROOM3D_LIGHT_H
