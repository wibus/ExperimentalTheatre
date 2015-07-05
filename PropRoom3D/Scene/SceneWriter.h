#ifndef PROPROOM3D_SCENEWRITER_H
#define PROPROOM3D_SCENEWRITER_H

#include <string>
#include <vector>


namespace prop3
{
    class Scene;


    class SceneWriter
    {
    public :
        SceneWriter();
        virtual ~SceneWriter();

        virtual bool write(const std::string& fileName, const Scene& scene);


    private:
    };
}

#endif // PROPROOM3D_SCENEWRITER_H
