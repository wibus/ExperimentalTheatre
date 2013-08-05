#ifndef PROPROOM2D_PROPTEAM_H
#define PROPROOM2D_PROPTEAM_H

#include <memory>

#include "libPropRoom2D_global.h"


namespace media
{
class Camera;
}

namespace prop2
{
class AbstractPropDesigner;
class AbstractArtDirector;
class AbstractChoreographer;

class AbstractShape;
class Circle;
class Polygon;

class AbstractHud;
class TextHud;
class ImageHud;


    class PROP2D_EXPORT AbstractPropTeam
    {
    protected:
        AbstractPropTeam(AbstractPropDesigner*  propDesigner,
                         AbstractArtDirector*   artDirector,
                         AbstractChoreographer* choreographer);

    public:
        virtual ~AbstractPropTeam();

        virtual void setup();
        virtual void reset();
        virtual void update(real dt);
        virtual void draw(real dt);

        // Factory methods
        virtual std::shared_ptr<Circle>   createCircle();
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices);
        virtual std::shared_ptr<TextHud>  createTextHud();
        virtual std::shared_ptr<ImageHud> createImageHud();

        virtual void deleteCircle(std::shared_ptr<Circle>& circle);
        virtual void deletePolygon(std::shared_ptr<Polygon>& polygon);
        virtual void deleteTextHud(std::shared_ptr<TextHud>& textHud);
        virtual void deleteImageHud(std::shared_ptr<ImageHud>& imageHud);

        virtual void setCamera(media::Camera& camera);

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

#endif // PROPROOM2D_PROPTEAM_H
