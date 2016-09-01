#ifndef PROPROOM2D_RAYARTDIRECTOR_H
#define PROPROOM2D_RAYARTDIRECTOR_H

#include <vector>
#include <map>

#include <CellarWorkbench/GL/GlProgram.h>
#include <CellarWorkbench/GL/GlVao.h>

#include "AbstractArtDirector.h"



namespace prop2
{
    class PROP2D_EXPORT RayArtDirector : public AbstractArtDirector
    {
    public:
        RayArtDirector();
        virtual ~RayArtDirector();

        virtual void notify(cellar::CameraMsg &msg);

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);
        virtual void update(double dt);

    protected:
        static const int OUT_BOUND_ID;
        static const int FIRST_IN_BOUND_ID;
        static const glm::vec2 SCENE_BOUNDS_EPSILON;

        cellar::GlProgram _fullScreenProgram;
        cellar::GlVao _fullScreenVao;
        GLuint _frameBuffer;
        GLuint _colorBuffer;

        GLuint _triangulationVao;
        GLuint _triangulationVbo;
        GLuint _triangulationEbo;
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_RAYARTDIRECTOR_H
