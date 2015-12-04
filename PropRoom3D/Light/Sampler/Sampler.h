#ifndef PROPROOM3D_SAMPLER_H
#define PROPROOM3D_SAMPLER_H

#include <PropRoom3D/Node/Node.h>


namespace prop3
{
    class Raycast;


    class PROP3D_EXPORT Sampler : public Node
    {
    protected:
        Sampler();

    public:
        virtual ~Sampler();

        virtual double area() const = 0;

        virtual double hitProbability(const Raycast& ray) const = 0;
    };
}

#endif // PROPROOM3D_SAMPLER_H
