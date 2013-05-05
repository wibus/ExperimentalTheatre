#include "GlShader.h"

#include <GL3/gl3w.h>
#include "Misc/CellarUtils.h"
#include "Misc/Log.h"

using namespace std;
using namespace cellar;


namespace media
{
    GlShader::GlShader(GLenum type) :
        _type(type),
        _id(0),
        _log(),
        _fileName("")
    {
    }

    GlShader::GlShader(GLenum type,
                       const std::string& fileName) :
        _type(type),
        _id(0),
        _log(),
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
    }

    bool GlShader::loadFromFile(const std::string& fileName)
    {
        _fileName = fileName;
        return loadFromString( fileToString(fileName) );

        int pom = printHello();
    }

    bool GlShader::loadFromString(const std::string& source)
    {
        string infoName = "unknown type shader";
        switch(_type)
        {
        case GL_VERTEX_SHADER   : infoName = "vertex shader";   break;
        case GL_FRAGMENT_SHADER : infoName = "fragment shader"; break;
        default: break;
        }

        string shaderSrc;
        if(_fileName.empty())
            shaderSrc = "in-source";
        else
            shaderSrc = _fileName;

        infoName = "(file = " + shaderSrc + ") " + infoName;

        if(source.empty())
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

        infoName = "(id = " + toString(_id) + ") " + infoName;

        const char* ctexte = source.c_str();
        glShaderSource( _id, 1, &ctexte, NULL );
        glCompileShader( _id );

        int compilationStatus;
        glGetShaderiv(_id, GL_COMPILE_STATUS, &compilationStatus);

        if ( !compilationStatus )
        {
            int infologLength = 0;
            glGetShaderiv( _id, GL_INFO_LOG_LENGTH, &infologLength );

            char* infoLog = new char[infologLength+1];
            int charsWritten  = 0;

            glGetShaderInfoLog( _id, infologLength, &charsWritten, infoLog );

            _log = "Errors in shader: ";
            _log.append(infoName);
            _log.append("\n");
            _log.append(infoLog);

            getLog().postMessage(new Message('E', false, _log, "GlShader"));
            delete[] infoLog;

            return false;
        }

        getLog().postMessage(new Message('I', false,
            infoName + " successfully compiled", "GlShader"));

        return true;
    }
}
