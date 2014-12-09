#ifndef PROPROOM3D_PROPTEAM_H
#define PROPROOM3D_PROPTEAM_H

#include <memory>

#include <glm/glm.hpp>

#include "libPropRoom3D_global.h"


namespace media
{
class Camera;
}

namespace prop3
{
class AbstractPropDesigner;
class AbstractArtDirector;
class AbstractChoreographer;

class AbstractShape;
class Brep;
class Mesh;

    class PROP3D_EXPORT AbstractPropTeam
    {
    protected:
        AbstractPropTeam(AbstractPropDesigner*  propDesigner,
                         AbstractArtDirector*   artDirector,
                         AbstractChoreographer* choreographer);

    public:
        virtual ~AbstractPropTeam();

        virtual void setup();
        virtual void reset();
        virtual void update(double dt);
        virtual void draw(double dt);

        // Factory methods
        virtual std::shared_ptr<Brep>   createBrep();
        virtual std::shared_ptr<Mesh>   createMesh(int vertexCount);

        virtual void deleteBrep(std::shared_ptr<Brep>& brep);
        virtual void deleteMesh(std::shared_ptr<Mesh>& mesh);

        virtual void setCamera(media::Camera& camera);
        virtual void setGravity(const glm::dvec3& unitsPerSecondSquared);

    protected:
        // Getter for prop team members
        AbstractPropDesigner&  propDesigner() const;
        AbstractArtDirector&   artDirector() const;
        AbstractChoreographer& choreographer() const;

    private:
        // Removed
        AbstractPropTeam(const AbstractPropTeam&);
        AbstractPropTeam& operator=(const AbstractPropTeam&);

    private:
        std::unique_ptr<AbstractPropDesigner>  _propDesigner;
        std::unique_ptr<AbstractArtDirector>   _artDirector;
        std::unique_ptr<AbstractChoreographer> _choreographer;
    };



    // IMPLEMENTATION //
    inline AbstractPropDesigner& AbstractPropTeam::propDesigner() const
    {
        return *_propDesigner;
    }

    inline AbstractArtDirector& AbstractPropTeam::artDirector() const
    {
        return *_artDirector;
    }

    inline AbstractChoreographer& AbstractPropTeam::choreographer() const
    {
        return *_choreographer;
    }
}

#endif // PROPROOM3D_PROPTEAM_H
