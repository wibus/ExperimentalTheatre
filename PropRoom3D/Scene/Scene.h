#ifndef PROPROOM3D_SCENE_H
#define PROPROOM3D_SCENE_H

#include <string>
#include <vector>

#include "Prop/Prop.h"


namespace prop3
{
    class Scene
    {
    public :
        Scene();
        virtual ~Scene();

        virtual bool read(const std::string& fileName);
        virtual bool write(const std::string& fileName) const;
        const std::vector<Prop>&  props() const;


    private:
        std::vector<Prop> _props;
    };



    // IMPLEMENTATION //
    inline const std::vector<Prop>&  Scene::props() const
    {
        return _props;
    }
}

#endif // PROPROOM3D_SCENE_H
