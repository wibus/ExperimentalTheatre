#include "QGlPostProdUnit.h"


namespace prop3
{
    QGlPostProdUnit::QGlPostProdUnit() :
        _colorBufferTexId(0),
        _fullscreenVao(0),
        _fullscreenVbo(0)
    {

    }

    QGlPostProdUnit::~QGlPostProdUnit()
    {
        glDeleteVertexArrays(1, &_fullscreenVao);
        glDeleteBuffers(1, &_fullscreenVbo);

        _fullscreenVao = 0;
        _fullscreenVbo = 0;
    }

    void QGlPostProdUnit::setColorBufferTexId(unsigned int id)
    {
        _colorBufferTexId = id;
    }

    void QGlPostProdUnit::setup()
    {
        // Post production program
        _postProdProgram.addShader(GL_VERTEX_SHADER, ":/Prop3/shaders/clip_space.vert");
        _postProdProgram.addShader(GL_FRAGMENT_SHADER, ":/Prop3/shaders/post_prod.frag");
        _postProdProgram.link();
        _postProdProgram.pushProgram();
        _postProdProgram.setInt("ImageTex", 0);
        _postProdProgram.popProgram();


        // Full screen triangle VAO
        const float vertices [][3] =  {
            {-1, -1, 1},
            { 3, -1, 1},
            {-1,  3, 1}
        };

        glGenVertexArrays(1, &_fullscreenVao);
        glBindVertexArray(_fullscreenVao);

        glGenBuffers(1, &_fullscreenVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _fullscreenVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void QGlPostProdUnit::execute()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _colorBufferTexId);

        _postProdProgram.pushProgram();

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(_fullscreenVao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        _postProdProgram.popProgram();
    }
}
