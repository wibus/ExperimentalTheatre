#include "DebugRenderer.h"

#include "Node/StageSet.h"
#include "Node/Debug/DebugLine.h"


namespace prop3
{
    DebugRenderer::DebugRenderer() :
        _lineVao(0),
        _lineVbo(0)
    {

    }

    DebugRenderer::~DebugRenderer()
    {

    }

    void DebugRenderer::setup()
    {
        _lineProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/debugLine.vert");
        _lineProgram.addShader(GL_FRAGMENT_SHADER, ":/Prop3/shaders/debugLine.frag");
        _lineProgram.link();

        glDeleteBuffers(1, &_lineVbo);
        glGenBuffers(1, &_lineVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glDeleteVertexArrays(1, &_lineVao);
        glGenVertexArrays(1, &_lineVao);
        glBindVertexArray(_lineVao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_DOUBLE, 0, 0, 0);
        glBindVertexArray(0);
    }

    void DebugRenderer::draw(const std::shared_ptr<StageSet>& stageSet)
    {
        if(stageSet->debugLines().empty())
            return;

        _lineProgram.pushProgram();

        glm::dmat4 pvMat = _projMat * _viewMat;
        _lineProgram.setMat4f("PVM", pvMat);

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(_lineVao);
        glBindBuffer(GL_ARRAY_BUFFER, _lineVbo);
        for(const DebugLine& line : stageSet->debugLines())
        {
            _lineProgram.setVec3f("Color", line.color());

            const std::vector<glm::dvec3>& verts = line.vertices();
            GLuint vertSize = sizeof(decltype(verts.front())) * verts.size();

            glBufferData(GL_ARRAY_BUFFER, vertSize, verts.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, verts.size());
        }

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
