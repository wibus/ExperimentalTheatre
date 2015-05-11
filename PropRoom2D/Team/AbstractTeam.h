#ifndef PROPROOM2D_ABSTRACTTEAM_H
#define PROPROOM2D_ABSTRACTTEAM_H

#include <vector>
#include <memory>

#include <GLM/glm.hpp>

#include "../libPropRoom2D_global.h"


namespace cellar
{
    class Camera;
}

namespace prop2
{
    class AbstractShape;
    class Circle;
    class Polygon;

    class AbstractHud;
    class TextHud;
    class ImageHud;

    class AbstractDesigner;
    class AbstractArtDirector;
    class AbstractChoreographer;


    class PROP2D_EXPORT AbstractTeam
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
        virtual std::shared_ptr<Circle>   createCircle();
        virtual std::shared_ptr<Polygon>  createPolygon(int nbVertices);
        virtual std::shared_ptr<TextHud>  createTextHud();
        virtual std::shared_ptr<ImageHud> createImageHud();

        virtual void deleteCircle(std::shared_ptr<Circle>& circle);
        virtual void deletePolygon(std::shared_ptr<Polygon>& polygon);
        virtual void deleteTextHud(std::shared_ptr<TextHud>& textHud);
        virtual void deleteImageHud(std::shared_ptr<ImageHud>& imageHud);

        // System modifiers
        virtual void setGravity(const glm::dvec2& unitsPerSecondSquared);


    private:
        std::unique_ptr<AbstractDesigner>  _propDesigner;
        std::unique_ptr<AbstractChoreographer> _choreographer;
        std::vector<std::shared_ptr<AbstractArtDirector>> _artDirectors;
    };
}

#endif // PROPROOM2D_ABSTRACTTEAM_H
