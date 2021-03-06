#ifndef CELLARWORKBENCH_GLSHADERBANK_H
#define CELLARWORKBENCH_GLSHADERBANK_H

#include <vector>
#include <memory>
#include <map>

#include <CellarWorkbench/DesignPattern/Singleton.h>
#include "GlShader.h"


namespace cellar
{
    class CELLAR_EXPORT GlShaderBank : public cellar::Singleton<GlShaderBank>
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

        bool addShaderFromFiles(
                const std::string& shaderName,
                GLenum shaderType,
                const std::vector<std::string>& fileNames);

        bool addShaderFromString(
                const std::string& shaderName,
                GLenum shaderType,
                const std::string& shaderSource);

        bool addShaderFromStrings(
                const std::string& shaderName,
                GLenum shaderType,
                const std::vector<std::string>& shaderSources);

        GlShader& getShader(
                const std::string& shaderName,
                GLenum shaderType);

        GlShader& getShader(
                const std::vector<std::string>& shaderNames,
                GLenum shaderType);

        std::shared_ptr<GlShader> getShaderPtr(
                const std::string& shaderName,
                GLenum shaderType);

        std::shared_ptr<GlShader> getShaderPtr(
                const std::vector<std::string>& shaderNames,
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

    CELLAR_EXPORT GlShaderBank& getGlShaderBank();




    // IMPLEMENTATION //
    inline bool GlShaderBank::isInBank(
            const std::string& shaderName,
            GLenum ) const
    {
        return _shaders.find(shaderName) != _shaders.end();
    }
}

#endif // CELLARWORKBENCH_GLSHADERBANK_H
