#include "GlFullscreen.h"

namespace cellar
{
    GlFullscreen::GlFullscreen() :
        _vao(0),
        _vbo(0)
    {

    }

    GlFullscreen::~GlFullscreen()
    {

    }

    void GlFullscreen::init()
    {
        setupVao();
    }

    void GlFullscreen::draw() const
    {
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void GlFullscreen::setupVao()
    {
        if(_vao != 0)
        {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
        if(_vbo != 0)
        {
            glDeleteBuffers(1, &_vbo);
            _vbo = 0;
        }

        GLfloat backTriangle[] = {
            -1.0f, -1.0f,
             3.0f, -1.0f,
            -1.0f,  3.0f
        };

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(backTriangle), backTriangle, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }
}
