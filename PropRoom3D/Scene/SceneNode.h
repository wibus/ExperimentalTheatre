#ifndef PROPROOM3D_SCENENODE_H
#define PROPROOM3D_SCENENODE_H

#include <ios>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{

    class PROP3D_EXPORT SceneNode
    {
    protected:
        SceneNode();

    public:
        virtual ~SceneNode();

        virtual void serialize(std::ostream& stream) const = 0;

        virtual void deserialize(std::istream& stream) = 0;
    };
}

#endif // PROPROOM3D_SCENENODE_H
