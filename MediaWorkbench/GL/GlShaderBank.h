#ifndef MEDIAWORKBENCH_GLSHADERBANK_H
#define MEDIAWORKBENCH_GLSHADERBANK_H

#include <memory>
#include <map>

#include <CellarWorkbench/DesignPattern/Singleton.h>
#include "GlShader.h"


namespace media
{
    class MEDIA_EXPORT GlShaderBank : public cellar::Singleton<GlShaderBank>
    {
        friend class cellar::Singleton<GlShaderBank>;
        GlShaderBank();

    public:

        bool isInBank(
                const std::string& shaderName,
                GLenum shaderType) const;

        bool addShaderFromFile(
                const std::string& shaderName,
                GLenum shaderType,
                const std::string& fileName);

        bool addShaderFromString(
                const std::string& shaderName,
                GLenum shaderType,
                const std::string& shaderSource);

        GlShader& getShader(
                const std::string& shaderName,
                GLenum shaderType);

        std::shared_ptr<GlShader> getShaderPtr(
                const std::string& shaderName,
                GLenum shaderType);

        bool deleteShader(
                const std::string& shaderName,
                GLenum shaderType);

    private:
        bool addShader(
                const std::string& shaderName,
                GLenum shaderType,
                const std::shared_ptr<GlShader>& shader);

        std::map<std::string, std::shared_ptr< GlShader > > _shaders;
    };

    MEDIA_EXPORT GlShaderBank& getGlShaderBank();




    // IMPLEMENTATION //
    inline bool GlShaderBank::isInBank(
            const std::string& shaderName,
            GLenum ) const
    {
        return _shaders.find(shaderName) != _shaders.end();
    }
}

#endif // MEDIAWORKBENCH_GLSHADERBANK_H
