#ifndef PROPROOM3D_SCENE_H
#define PROPROOM3D_SCENE_H

#include <string>
#include <vector>

#include "Prop/Prop.h"
#include "Team/AbstractTeam.h"


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
                           const std::shared_ptr<AbstractTeam>& team) const;

        std::vector<Prop>&  props();


    private:
        std::vector<Prop> _props;
    };



    // IMPLEMENTATION //
    inline std::vector<Prop>& Scene::props()
    {
        return _props;
    }
}

#endif // PROPROOM3D_SCENE_H
