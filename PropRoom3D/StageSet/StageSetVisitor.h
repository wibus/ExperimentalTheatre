#ifndef PROPROOM3D_STAGESETVISITOR_H
#define PROPROOM3D_STAGESETVISITOR_H

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    // Props
    class Prop;

    // Implicit Surfaces
    class Surface;
    class SurfaceShell;
    class SurfaceGhost;
    class SurfaceInverse;
    class SurfaceOr;
    class SurfaceAnd;
    class Box;
    class BoxTexture;
    class Plane;
    class PlaneTexture;
    class Quadric;
    class Sphere;

    // Materials
    class Material;
    class UniformStdMaterial;

    // Coatings
    class Coating;
    class UniformStdCoating;
    class TexturedStdCoating;

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
        virtual void visit(SurfaceShell& node);
        virtual void visit(SurfaceGhost& node);
        virtual void visit(SurfaceInverse& node);
        virtual void visit(SurfaceOr& node);
        virtual void visit(SurfaceAnd& node);
        virtual void visit(Box& node);
        virtual void visit(BoxTexture& node);
        virtual void visit(Plane& node);
        virtual void visit(PlaneTexture& node);
        virtual void visit(Quadric& node);
        virtual void visit(Sphere& node);

        // Materials
        virtual void visit(UniformStdMaterial& node);

        // Coatings
        virtual void visit(UniformStdCoating& node);
        virtual void visit(TexturedStdCoating& node);

        // Environments
        virtual void visit(Environment& node);

        // Backdrops
        virtual void visit(ProceduralSun& node);
    };
}

#endif // PROPROOM3D_STAGESETVISITOR_H
