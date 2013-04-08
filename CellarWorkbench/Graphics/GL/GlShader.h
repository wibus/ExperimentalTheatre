#ifndef CELLARWORKBENCH_GLSHADER_H
#define CELLARWORKBENCH_GLSHADER_H

#include <string>

#include <GL3/gl3w.h>

#include "libCellarWorkbench_global.h"


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

        GLenum type() const;
        GLuint id() const;
        const std::string& log() const;


    protected:
        GLenum _type;
        GLuint _id;
        std::string _log;
        std::string _fileName;

    private:
        GlShader();
        GlShader(const GlShader&);
        GlShader& operator=(const GlShader&);
    };



    // IMPLEMENTATION //
    inline GLenum GlShader::type() const
    {
        return _type;
    }

    inline GLuint GlShader::id() const
    {
        return _id;
    }

    inline const std::string& GlShader::log() const
    {
        return _log;
    }
}

#endif // CELLARWORKBENCH_GLABSTRACTSHADER_H
