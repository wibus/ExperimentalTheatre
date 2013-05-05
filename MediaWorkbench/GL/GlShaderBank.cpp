#include "GlShaderBank.h"
#include "Misc/CellarUtils.h"
#include "Misc/Log.h"

using namespace std;
using namespace cellar;

namespace media
{
    MEDIA_EXPORT GlShaderBank& getGlShaderBank()
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

    bool GlShaderBank::addShaderFromString(
            const std::string& shaderName,
            GLenum shaderType,
            const std::string& shaderSource)
    {
        shared_ptr<GlShader> shader(new GlShader(shaderType));
        shader->loadFromString(shaderSource);

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
            "Implicitly adding : " + shaderName, "GlShaderBank"));

        if(addShaderFromFile(shaderName, shaderType, shaderName))
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
                "Shader deleted : " + shaderName, "GlShaderBank"));

            return true;
        }

        getLog().postMessage(new Message('W', false,
            "Shader not found so not deleted : " + shaderName, "GlShaderBank"));

        return false;
    }
}
