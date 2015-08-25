#ifndef PROPROOM3D_STAGESETVISITOR_H
#define PROPROOM3D_STAGESETVISITOR_H

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    // Props
    class Prop;

    // Implicit Surfaces
    class Surface;
    class SurfaceGhost;
    class SurfaceInverse;
    class SurfaceOr;
    class SurfaceAnd;
    class Plane;
    class PlaneTexture;
    class Quadric;
    class Sphere;

    // Materials
    class Material;
    class Air;
    class Fog;
    class Concrete;
    class Glass;
    class Metal;

    // Coatings
    class Coating;
    class NoCoating;
    class FlatPaint;
    class GlossyPaint;
    class TexturedFlatPaint;
    class TexturedGlossyPaint;

    // Environments
    class Environment;

    // Backdrops
    class Backdrop;
    class ProceduralSun;


    class PROP3D_EXPORT StageSetVisitor
    {
    protected:
        StageSetVisitor();

    public:
        virtual ~StageSetVisitor();

        // Props
        virtual void visit(Prop& node);

        // Implicit Surfaces
        virtual void visit(SurfaceGhost& node);
        virtual void visit(SurfaceInverse& node);
        virtual void visit(SurfaceOr& node);
        virtual void visit(SurfaceAnd& node);
        virtual void visit(Plane& node);
        virtual void visit(PlaneTexture& node);
        virtual void visit(Quadric& node);
        virtual void visit(Sphere& node);

        // Materials
        virtual void visit(Air& node);
        virtual void visit(Fog& node);
        virtual void visit(Concrete& node);
        virtual void visit(Glass& node);
        virtual void visit(Metal& node);

        // Coatings
        virtual void visit(NoCoating& node);
        virtual void visit(FlatPaint& node);
        virtual void visit(GlossyPaint& node);
        virtual void visit(TexturedFlatPaint& node);
        virtual void visit(TexturedGlossyPaint& node);

        // Environments
        virtual void visit(Environment& node);

        // Backdrops
        virtual void visit(ProceduralSun& node);
    };
}

#endif // PROPROOM3D_STAGESETVISITOR_H
