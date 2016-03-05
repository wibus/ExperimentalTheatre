#include "DebugRenderer.h"

#include "Node/StageSet.h"
#include "Node/Debug/DebugLineStrip.h"
#include "Node/Debug/DebugPointCloud.h"


namespace prop3
{
    DebugRenderer::DebugRenderer() :
        _vao(0),
        _vbo(0)
    {

    }

    DebugRenderer::~DebugRenderer()
    {

    }

    void DebugRenderer::setup()
    {
        // Lines
        _lineProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/debugLine.vert");
        _lineProgram.addShader(GL_FRAGMENT_SHADER, ":/Prop3/shaders/debugLine.frag");
        _lineProgram.link();

        // Points
        _pointProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/debugPoint.vert");
        _pointProgram.addShader(GL_FRAGMENT_SHADER, ":/Prop3/shaders/debugPoint.frag");
        _pointProgram.link();


        // Buffers
        glDeleteBuffers(1, &_vbo);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

        glDeleteVertexArrays(1, &_vao);
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_DOUBLE, 0, 0, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void DebugRenderer::draw(const std::shared_ptr<StageSet>& stageSet)
    {
        if(stageSet->debugLines().empty() &&
           stageSet->debugPoints().empty())
            return;

        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        _lineProgram.pushProgram();
        for(const DebugLineStrip& line : stageSet->debugLines())
        {
            _lineProgram.setVec3f("Color", line.color());

            const std::vector<glm::dvec3>& verts = line.vertices();
            GLuint vertSize = sizeof(decltype(verts.front())) * verts.size();
            glBufferData(GL_ARRAY_BUFFER, vertSize, verts.data(), GL_DYNAMIC_DRAW);

            glDrawArrays(GL_LINE_STRIP, 0, verts.size());
        }
        _lineProgram.popProgram();

        glPointSize(8.0f);
        _pointProgram.pushProgram();
        for(const DebugPointCloud& point : stageSet->debugPoints())
        {
            _pointProgram.setVec3f("Color", point.color());

            const std::vector<glm::dvec3>& verts = point.vertices();
            GLuint vertSize = sizeof(decltype(verts.front())) * verts.size();
            glBufferData(GL_ARRAY_BUFFER, vertSize, verts.data(), GL_DYNAMIC_DRAW);

            glDrawArrays(GL_POINTS, 0, verts.size());
        }
        _pointProgram.popProgram();


        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void DebugRenderer::updateView(const glm::dmat4& view)
    {
        _viewMat = view;
        updateMatrices();
    }

    void DebugRenderer::updateProjection(const glm::dmat4& proj)
    {
        _projMat = proj;
        updateMatrices();
    }

    void DebugRenderer::updateMatrices()
    {
        glm::dmat4 pvMat = _projMat * _viewMat;

        _lineProgram.pushProgram();
        _lineProgram.setMat4f("PVM", pvMat);
        _lineProgram.popProgram();

        _pointProgram.pushProgram();
        _pointProgram.setMat4f("PVM", pvMat);
        _pointProgram.popProgram();
    }
}
