#include "DebugRenderer.h"

namespace prop3
{
    DebugRenderer::DebugRenderer()
    {

    }

    DebugRenderer::~DebugRenderer()
    {

    }

    void DebugRenderer::setup()
    {
        _lineProgram.addShader(GL_VERTEX_SHADER, ":/shaders/debugLine.vert");
        _lineProgram.addShader(GL_FRAGMENT_SHADER, ":/shaders/debugLine.frag");
        _lineProgram.link();
    }

    void DebugRenderer::draw(const std::shared_ptr<StageSet>& stageSet)
    {
        _lineProgram.pushProgram();
        glm::dmat4 pvMat = _projMat * _viewMat;
        _lineProgram.setMat4f("PVM", pvMat);
        _lineProgram.popProgram();
    }

    void DebugRenderer::updateView(const glm::dmat4& view)
    {
        _viewMat = view;
    }

    void DebugRenderer::updateProjection(const glm::dmat4& proj)
    {
        _projMat = proj;
    }
}
