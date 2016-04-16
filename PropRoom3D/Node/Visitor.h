#ifndef PROPROOM3D_STAGESETVISITOR_H
#define PROPROOM3D_STAGESETVISITOR_H

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    // Stage set
    class StageSet;
    class StageZone;

    // Props
    class Prop;

    // Bounds
    class Bounds;

    // Surfaces
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
    class Disk;

    // Materials
    class Material;
    class UniformStdMaterial;

    // Coatings
    class Coating;
    class EmissiveCoating;
    class UniformStdCoating;
    class TexturedStdCoating;

    // Lights
    class LightBulb;
    class CircularLight;
    class SphericalLight;

    // Backdrops
    class Backdrop;
    class ProceduralSun;


    class PROP3D_EXPORT Visitor
    {
    protected:
        Visitor();

    public:
        virtual ~Visitor();

        // Stage
        virtual void visit(StageSet& node);
        virtual void visit(StageZone& node);

        // Props
        virtual void visit(Prop& node);

        // Surfaces
        virtual void visit(Bounds& node);

        // Surfaces
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
        virtual void visit(Disk& node);

        // Materials
        virtual void visit(UniformStdMaterial& node);

        // Coatings
        virtual void visit(EmissiveCoating& node);
        virtual void visit(UniformStdCoating& node);
        virtual void visit(TexturedStdCoating& node);

        // Lights
        virtual void visit(CircularLight& node);
        virtual void visit(SphericalLight& node);

        // Backdrops
        virtual void visit(ProceduralSun& node);
    };
}

#endif // PROPROOM3D_STAGESETVISITOR_H
