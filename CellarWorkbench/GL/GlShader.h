#ifndef CELLARWORKBENCH_GLSHADER_H
#define CELLARWORKBENCH_GLSHADER_H

#include <string>
#include <vector>

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
        bool loadFromFiles(const std::vector<std::string>& fileNames);
        bool loadFromString(const std::string& source);
        bool loadFromStrings(const std::vector<std::string>& sources);

        GLuint id() const;
        GLenum type() const;


        static std::string implicitName(const std::string& fileName,
                                        const GLenum& shaderType);
        static std::string implicitName(const std::vector<std::string>& fileNames,
                                        const GLenum& shaderType);

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
