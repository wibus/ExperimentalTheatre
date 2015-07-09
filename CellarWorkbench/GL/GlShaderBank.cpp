#include "GlShaderBank.h"

using namespace std;


#include <CellarWorkbench/Misc/Log.h>
using namespace cellar;


namespace cellar
{
    GlShaderBank& getGlShaderBank()
    {
        return *GlShaderBank::getInstance();
    }

    GlShaderBank::GlShaderBank() :
        _shaders()
    {

    }

    bool GlShaderBank::addShaderFromFile(
            const std::string& shaderName,
            GLenum shaderType,
            const std::string& fileName)
    {
        shared_ptr<GlShader> shader(new GlShader(shaderType));
        shader->loadFromFile(fileName);

        return addShader(shaderName, shaderType, shader);
    }

    bool GlShaderBank::addShaderFromFiles(
            const std::string& shaderName,
            GLenum shaderType,
            const std::vector<std::string>& fileNames)
    {
        shared_ptr<GlShader> shader(new GlShader(shaderType));
        shader->loadFromFiles(fileNames);

        return addShader(shaderName, shaderType, shader);
    }

    bool GlShaderBank::addShaderFromString(
            const std::string& shaderName,
            GLenum shaderType,
            const std::string& shaderSource)
    {
        shared_ptr<GlShader> shader(new GlShader(shaderType));
        shader->loadFromString(shaderSource);

        return addShader(shaderName, shaderType, shader);
    }

    bool GlShaderBank::addShaderFromStrings(
            const std::string& shaderName,
            GLenum shaderType,
            const std::vector<std::string>& shaderSources)
    {
        shared_ptr<GlShader> shader(new GlShader(shaderType));
        shader->loadFromStrings(shaderSources);

        return addShader(shaderName, shaderType, shader);
    }

    bool GlShaderBank::addShader(
            const std::string& shaderName,
            GLenum,
            const std::shared_ptr<GlShader>& shader)
    {
        auto pairNameShader = make_pair(shaderName, shader);

        if(_shaders.insert( pairNameShader ).second)
        {
            getLog().postMessage(new Message('I', false,
                "'" + shaderName + "' successfully added to shader bank", "GlShaderBank"));

            return true;
        }

        getLog().postMessage(new Message('W', false,
            "An shader named '" + shaderName + "' is already in the bank", "GlShaderBank"));

        return false;
    }

    GlShader& GlShaderBank::getShader(
            const std::string& shaderName,
            GLenum shaderType)
    {
        return *getShaderPtr(shaderName, shaderType);
    }

    GlShader& GlShaderBank::getShader(
            const std::vector<std::string>& shaderNames,
            GLenum shaderType)
    {
        return *getShaderPtr(shaderNames, shaderType);
    }

    std::shared_ptr<GlShader> GlShaderBank::getShaderPtr(
            const std::string& shaderName,
            GLenum shaderType)
    {
        auto it = _shaders.find(shaderName);

        if(it != _shaders.end())
        {
            return it->second;
        }

        getLog().postMessage(new Message('I', false,
            "Implicitly adding : '" + shaderName + "'", "GlShaderBank"));

        if(addShaderFromFile(shaderName, shaderType, shaderName))
        {
            return getShaderPtr(shaderName, shaderType);
        }

        return getShaderPtr("blank", shaderType);
    }

    std::shared_ptr<GlShader> GlShaderBank::getShaderPtr(
            const std::vector<std::string>& shaderNames,
            GLenum shaderType)
    {
        std::string shaderName = GlShader::implicitName(shaderNames);

        auto it = _shaders.find(shaderName);

        if(it != _shaders.end())
        {
            return it->second;
        }

        getLog().postMessage(new Message('I', false,
            "Implicitly adding : '" + shaderName + "'", "GlShaderBank"));

        if(addShaderFromFiles(shaderName, shaderType, shaderNames))
        {
            return getShaderPtr(shaderName, shaderType);
        }

        return getShaderPtr("blank", shaderType);
    }

    bool GlShaderBank::deleteShader(
            const std::string& shaderName,
            GLenum)
    {
		size_t nbRemoved = _shaders.erase(shaderName);

        if(nbRemoved)
        {
            getLog().postMessage(new Message('I', false,
                "Shader deleted : '" + shaderName + "'", "GlShaderBank"));

            return true;
        }

        getLog().postMessage(new Message('W', false,
            "Shader not found so not deleted : '" + shaderName + "'", "GlShaderBank"));

        return false;
    }
}
