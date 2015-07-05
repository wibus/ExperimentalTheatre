#ifndef PROPROOM3D_SCENEREADER_H
#define PROPROOM3D_SCENEREADER_H

#include <string>
#include <vector>


namespace prop3
{
    class Scene;


    class SceneReader
    {
    public :
        SceneReader();
        virtual ~SceneReader();

        virtual bool read(const std::string& fileName, Scene& scene);


    private:
    };
}

#endif // PROPROOM3D_SCENEREADER_H
