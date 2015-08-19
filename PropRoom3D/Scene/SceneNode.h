#ifndef PROPROOM3D_SCENENODE_H
#define PROPROOM3D_SCENENODE_H

#include <vector>
#include <memory>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class SceneVisitor;


    class PROP3D_EXPORT SceneNode
    {
    protected:
        SceneNode();

    public:
        virtual ~SceneNode();

        virtual void accept(SceneVisitor& visitor) = 0;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const;
    };
}

#endif // PROPROOM3D_SCENENODE_H
