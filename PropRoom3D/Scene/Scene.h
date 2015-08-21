#ifndef PROPROOM3D_SCENE_H
#define PROPROOM3D_SCENE_H

#include <string>
#include <vector>
#include <memory>

#include "../Prop/Prop.h"


namespace prop3
{
    class PROP3D_EXPORT Scene
    {
    public :
        Scene();
        virtual ~Scene();


        virtual void makeTraveling(SceneVisitor& visitor);


        virtual void addProp(const std::shared_ptr<Prop>& prop);

        virtual void removeProp(const std::shared_ptr<Prop>& prop);

        const std::vector<std::shared_ptr<Prop>>&  props();


        bool updateTimeStamp();

        TimeStamp timeStamp() const;

        bool sceneChanged() const;


    private:
        std::vector<std::shared_ptr<Prop>> _props;
        TimeStamp _timeStamp;
        bool _sceneChanged;
    };



    // IMPLEMENTATION //
    inline const std::vector<std::shared_ptr<Prop>>& Scene::props()
    {
        return _props;
    }

    inline TimeStamp Scene::timeStamp() const
    {
        return _timeStamp;
    }

    inline bool Scene::sceneChanged() const
    {
        return _sceneChanged;
    }
}

#endif // PROPROOM3D_SCENE_H
