#ifndef CELLARWORKBENCH_GLSHADER_H
#define CELLARWORKBENCH_GLSHADER_H

#include <string>

#include <GL3/gl3w.h>


#include "../libCellarWorkbench_global.h"



namespace cellar
{
    class CELLAR_EXPORT GlShader
    {
    public:
        GlShader(GLenum type);
        GlShader(GLenum type, const std::string& fileName);
        virtual ~GlShader();

        bool loadFromFile(const std::string& fileName);
        bool loadFromString(const std::string& source);

        GLuint id() const;
        GLenum type() const;


    protected:
        GLuint _id;
        GLenum _type;
        std::string _fileName;

    private:
        GlShader();
        GlShader(const GlShader&);
        GlShader& operator=(const GlShader&);
    };



    // IMPLEMENTATION //
    inline GLuint GlShader::id() const
    {
        return _id;
    }

    inline GLenum GlShader::type() const
    {
        return _type;
    }
}

#endif // CELLARWORKBENCH_GLABSTRACTSHADER_H
