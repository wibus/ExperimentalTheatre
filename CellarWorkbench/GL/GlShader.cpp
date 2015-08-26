#include "GlShader.h"

#include <GL3/gl3w.h>

#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

using namespace std;
using namespace cellar;


namespace cellar
{
    GlShader::GlShader(GLenum type) :
        _id(0),
        _type(type),
        _fileName("")
    {
    }

    GlShader::GlShader(GLenum type,
                       const std::string& fileName) :
        _id(0),
        _type(type),
        _fileName("")
    {
        loadFromFile(fileName);
    }

    GlShader::~GlShader()
    {
        if(_id != 0)
        {
            glDeleteShader(_id);
        }

        getLog().postMessage(new Message('I', false,
            "OpenGL shader (id=" + toString(_id) + ") deleted", "GlShader"));
    }

    bool GlShader::loadFromFile(const std::string& fileName)
    {
        _fileName = fileName;
        return loadFromString( fileToString(fileName) );
    }

    bool GlShader::loadFromFiles(const std::vector<std::string>& fileNames)
    {
        _fileName = implicitName(fileNames, _type);

        std::vector<std::string> sources;
        for(const auto& file : fileNames)
        {
            sources.push_back( fileToString(file) );
            if(sources.back().empty())
                sources.pop_back();
        }

        return loadFromStrings( sources );
    }

    bool GlShader::loadFromString(const std::string& source)
    {
        if(source.empty())
            return loadFromStrings({ });
        else
            return loadFromStrings({ source });
    }

    bool GlShader::loadFromStrings(const std::vector<std::string>& sources)
    {
        string shaderType = "unknown type shader";
        switch(_type)
        {
        case GL_VERTEX_SHADER   : shaderType = "vertex shader";   break;
        case GL_FRAGMENT_SHADER : shaderType = "fragment shader"; break;
        case GL_COMPUTE_SHADER  : shaderType = "compute shader";  break;
        default: break;
        }

        string shaderSrc;
        if(_fileName.empty())
            shaderSrc = "in-source";
        else
            shaderSrc = _fileName;

        string infoName = "'" + shaderSrc + "' " + shaderType;

        if(sources.empty())
        {
            getLog().postMessage(new Message('E', false,
                infoName + " was absent or empty", "GlShader"));
            return false;
        }

        if(_id != 0)
        {
            glDeleteShader(_id);
            _id = 0;
        }

        _id = glCreateShader(_type);

        if(_id == 0)
        {
            getLog().postMessage(new Message('E', false,
                infoName + " could not be created", "GlShader"));
            return false;
        }

        string shaderId = "(id = " + toString(_id) + ")";

        const char** ctexts = new const char*[sources.size()];
        for(int i=0; i < sources.size(); ++i)
            ctexts[i] = sources[i].c_str();

        glShaderSource( _id, sources.size(), ctexts, NULL );
        glCompileShader( _id );

        delete[] ctexts;


        int compilationStatus;
        glGetShaderiv(_id, GL_COMPILE_STATUS, &compilationStatus);


        int infologLength = 0;
        glGetShaderiv( _id, GL_INFO_LOG_LENGTH, &infologLength );
        if ( infologLength > 1 )
            {
            char* infoLog = new char[infologLength+1];
            int charsWritten  = 0;

            glGetShaderInfoLog( _id, infologLength, &charsWritten, infoLog );

            string log = "Errors in shader: ";
            log.append(infoName + " " + shaderId + "\n");
            log.append(infoLog);

            getLog().postMessage(new Message('E', false, log, "GlShader"));
            delete[] infoLog;

            return compilationStatus;
        }

        getLog().postMessage(new Message('I', false,
            infoName + " successfully compiled " + shaderId, "GlShader"));

        return compilationStatus;
    }

    std::string GlShader::implicitName(
            const std::string& fileName,
            const GLenum& shaderType)
    {
        return fileName + "(type=" + to_string(shaderType) + ")";
    }

    std::string GlShader::implicitName(
            const std::vector<std::string>& fileNames,
            const GLenum& shaderType)
    {
        std::string shaderName;
        for(const auto& name : fileNames)
            shaderName += name + ";";
        shaderName.pop_back();

        return implicitName(shaderName, shaderType);
    }
}
