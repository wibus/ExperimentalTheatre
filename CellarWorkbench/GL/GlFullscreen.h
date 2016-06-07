#ifndef CELLARWORKBENCH_GLFULLSCREEN_H
#define CELLARWORKBENCH_GLFULLSCREEN_H

#include <GL3/gl3w.h>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    class CELLAR_EXPORT GlFullscreen
    {
    public:
        GlFullscreen();
        virtual ~GlFullscreen();

        virtual void init();
        virtual void draw() const;

        GLuint vao() const;
        GLuint vbo() const;

    protected:
        // Override to add vertex arrays
        virtual void setupVao();

        GLuint _vao;
        GLuint _vbo;
    };



    // IMPLEMENTATION //
    inline GLuint GlFullscreen::vao() const
    {
        return _vao;
    }

    inline GLuint GlFullscreen::vbo() const
    {
        return _vbo;
    }
}

#endif // CELLARWORKBENCH_GLFULLSCREEN_H
