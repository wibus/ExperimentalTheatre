#ifndef PROPROOM3D_SCENEWRITER_H
#define PROPROOM3D_SCENEWRITER_H

#include <string>
#include <vector>
#include <memory>

#include "libPropRoom3D_global.h"


namespace prop3
{
    class Scene;
    class AbstractTeam;

    class PROP3D_EXPORT SceneWriter
    {
    public :
        SceneWriter();
        virtual ~SceneWriter();

        virtual bool write(const std::string& fileName,
                           const std::shared_ptr<AbstractTeam>& team,
                           const Scene& scene);


    private:
    };
}

#endif // PROPROOM3D_SCENEWRITER_H
