#ifndef PROPROOM3D_SCENE_H
#define PROPROOM3D_SCENE_H

#include <string>
#include <vector>
#include <memory>

#include "../Prop/Prop.h"
#include "../Team/AbstractTeam.h"


namespace prop3
{
    class PROP3D_EXPORT Scene
    {
    public :
        Scene();
        virtual ~Scene();

        virtual bool read(const std::string& fileName,
                          const std::shared_ptr<AbstractTeam>& team);

        virtual bool write(const std::string& fileName,
                           const std::shared_ptr<AbstractTeam>& team);

        void clearProps(const std::shared_ptr<AbstractTeam>& team);

        std::shared_ptr<Prop> addProp(const std::shared_ptr<AbstractTeam>& team);

        std::vector<std::shared_ptr<Prop>>&  props();

        virtual void makeTraveling(SceneVisitor& visitor);


    private:
        std::vector<std::shared_ptr<Prop>> _props;
    };



    // IMPLEMENTATION //
    inline std::vector<std::shared_ptr<Prop>>& Scene::props()
    {
        return _props;
    }
}

#endif // PROPROOM3D_SCENE_H
