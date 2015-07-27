#ifndef PROPROOM3D_SCENEREADER_H
#define PROPROOM3D_SCENEREADER_H

#include <string>
#include <vector>
#include <memory>

#include "libPropRoom3D_global.h"


namespace prop3
{
    class Scene;
    class AbstractTeam;

    class PROP3D_EXPORT SceneReader
    {
    public :
        SceneReader();
        virtual ~SceneReader();

        virtual bool read(const std::string& fileName,
                          const std::shared_ptr<AbstractTeam>& team,
                          Scene& scene);


    private:
    };
}

#endif // PROPROOM3D_SCENEREADER_H
