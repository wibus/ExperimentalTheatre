#ifndef PROPROOM3D_ABSTRACTTEAM_H
#define PROPROOM3D_ABSTRACTTEAM_H

#include <memory>

#include <GLM/glm.hpp>

#include "../libPropRoom3D_global.h"


namespace media
{
    class Camera;
}

namespace prop3
{
    class AbstractDesigner;
    class AbstractArtDirector;
    class AbstractChoreographer;

class Prop;

    class PROP3D_EXPORT AbstractTeam
    {
    protected:
        AbstractTeam(AbstractDesigner*  propDesigner,
                         AbstractArtDirector*   artDirector,
                         AbstractChoreographer* choreographer);

    public:
        virtual ~AbstractTeam();

        virtual void setup();
        virtual void reset();
        virtual void update(double dt);
        virtual void draw(double dt);

        // Factory methods
        virtual std::shared_ptr<Prop>   createProp();
        virtual void deleteProp(std::shared_ptr<Prop>& prop);

        virtual void setCamera(media::Camera& camera);
        virtual void setGravity(const glm::dvec3& unitsPerSecondSquared);

    protected:
        // Getter for prop team members
        AbstractDesigner&  propDesigner() const;
        AbstractArtDirector&   artDirector() const;
        AbstractChoreographer& choreographer() const;

    private:
        // Removed
        AbstractTeam(const AbstractTeam&);
        AbstractTeam& operator=(const AbstractTeam&);

    private:
        std::unique_ptr<AbstractDesigner>  _propDesigner;
        std::unique_ptr<AbstractArtDirector>   _artDirector;
        std::unique_ptr<AbstractChoreographer> _choreographer;
    };



    // IMPLEMENTATION //
    inline AbstractDesigner& AbstractTeam::propDesigner() const
    {
        return *_propDesigner;
    }

    inline AbstractArtDirector& AbstractTeam::artDirector() const
    {
        return *_artDirector;
    }

    inline AbstractChoreographer& AbstractTeam::choreographer() const
    {
        return *_choreographer;
    }
}

#endif // PROPROOM3D_ABSTRACTTEAM_H
