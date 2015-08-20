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
        Scene(const std::shared_ptr<AbstractTeam>& team);
        virtual ~Scene();

        virtual bool load(const std::string& fileName, bool clearScene = true);

        virtual bool save(const std::string& fileName, bool prettyPrint = true);

        virtual void deserialize(const std::string& stream, bool clearScene = true);

        virtual std::string serialize();


        void clearProps();

        std::shared_ptr<Prop> createProp();

        std::vector<std::shared_ptr<Prop>>&  props();

        std::shared_ptr<AbstractTeam> team() const;

        virtual void makeTraveling(SceneVisitor& visitor);


    private:
        std::shared_ptr<AbstractTeam> _team;
        std::vector<std::shared_ptr<Prop>> _props;
    };



    // IMPLEMENTATION //
    inline std::vector<std::shared_ptr<Prop>>& Scene::props()
    {
        return _props;
    }

    inline std::shared_ptr<AbstractTeam> Scene::team() const
    {
        return _team;
    }
}

#endif // PROPROOM3D_SCENE_H
