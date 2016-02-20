#ifndef PROPROOM3D_DEBUGRENDERER_H
#define PROPROOM3D_DEBUGRENDERER_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/GL/GlProgram.h>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class StageSet;

    class PROP3D_EXPORT DebugRenderer
    {
    public:
        DebugRenderer();
        virtual ~DebugRenderer();

        virtual void setup();

        virtual void draw(const std::shared_ptr<StageSet>& stageSet);

        virtual void updateView(const glm::dmat4& view);
        virtual void updateProjection(const glm::dmat4& proj);

    private:
        glm::dmat4 _viewMat;
        glm::dmat4 _projMat;
        cellar::GlProgram _lineProgram;
        unsigned int _lineVbo;
        unsigned int _lineVao;
    };
}

#endif // PROPROOM3D_DEBUGRENDERER_H
