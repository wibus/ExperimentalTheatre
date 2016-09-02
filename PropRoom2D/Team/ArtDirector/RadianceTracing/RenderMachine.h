#ifndef PROPROOM2D_RADIANCEPATH_H
#define PROPROOM2D_RADIANCEPATH_H

#include <vector>

#include <CellarWorkbench/GL/GlProgram.h>
#include <CellarWorkbench/GL/GlVao.h>

#include <PropRoom2D/libPropRoom2D_global.h>


namespace prop2
{
    class Mesh;
    class LightPath;


    class PROP2D_EXPORT RenderMachine
    {
    public:
        RenderMachine();
        ~RenderMachine();

        void reset(const Mesh& mesh);

        void traceRadiance(std::vector<LightPath>& paths);

        void renderMeshWireframe();

        void renderIrradiance();

        void clearIrradiance();

    private:
        cellar::GlProgram _fullScreenProgram;
        cellar::GlVao _fullScreenVao;
        GLuint _frameBuffer;
        GLuint _colorBuffer;

        GLuint _triangulationVao;
        GLuint _triangulationVbo;
        GLuint _triangulationEbo;
    };
}

#endif // PROPROOM2D_RADIANCEPATH_H
