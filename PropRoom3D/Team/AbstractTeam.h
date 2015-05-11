#ifndef PROPROOM3D_ABSTRACTTEAM_H
#define PROPROOM3D_ABSTRACTTEAM_H

#include <vector>
#include <memory>

#include <GLM/glm.hpp>

#include "../libPropRoom3D_global.h"


namespace cellar
{
    class Camera;
}

namespace prop3
{
    class Prop;
    class AbstractDesigner;
    class AbstractArtDirector;
    class AbstractChoreographer;


    class PROP3D_EXPORT AbstractTeam
    {
    private:
        // Removed
        AbstractTeam(const AbstractTeam&) = delete;
        AbstractTeam& operator=(const AbstractTeam&) = delete;


    protected:
        AbstractTeam(AbstractDesigner*  propDesigner,
                     AbstractChoreographer* choreographer);


    public:
        virtual ~AbstractTeam();

        virtual void setup();
        virtual void reset();
        virtual void update(double dt);

        // Team accessors/modifiers
        virtual void addArtDirector(
            const std::shared_ptr<AbstractArtDirector>& artDirector);

        // Factory methods
        virtual std::shared_ptr<Prop> createProp();
        virtual void deleteProp(std::shared_ptr<Prop>& prop);


    private:
        std::unique_ptr<AbstractDesigner> _propDesigner;
        std::unique_ptr<AbstractChoreographer> _choreographer;
        std::vector<std::shared_ptr<AbstractArtDirector>> _artDirectors;
    };
}

#endif // PROPROOM3D_ABSTRACTTEAM_H
