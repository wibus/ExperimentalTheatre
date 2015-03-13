#include "GlProgram.h"

#include <cassert>
using namespace std;

#include <GLM/gtc/type_ptr.hpp>

#include <GL3/gl3w.h>

#include <CellarWorkbench/Misc/Log.h>
#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;

#include "GlShader.h"
#include "GlShaderBank.h"
#include "GlToolkit.h"
#include "../Image/ImageBank.h"


namespace media
{
    stack<pair<GLuint, GlProgram*> > GlProgram::_progStack =
            stack<pair<GLuint, GlProgram*> >();

    GlProgram::GlProgram() :
        _id(0),
        _linked(false),
        _log(),
        _inAndOutLocations(),
        _shaders(),
        _textures(),
        _cachedLocations()
    {
    }

    GlProgram::~GlProgram()
    {
        if(_id != 0)
        {
            for (auto s=_shaders.begin(); s != _shaders.end(); ++s)
            {
                glDetachShader( _id, (*s)->id() );
            }

            glDeleteProgram(_id);
        }
    }

    bool GlProgram::setInAndOutLocations(const GlInputsOutputs& inout)
    {
        if(_linked)
            return false;

        _inAndOutLocations = inout;
        return true;
    }

    bool GlProgram::addShader(const std::shared_ptr<GlShader>& shader)
    {
        return _shaders.insert( shader ).second;
    }

    bool GlProgram::addShader(GLenum shaderType,
                              const std::string& shaderName)
    {
        shared_ptr<GlShader> shader =
            getGlShaderBank().getShaderPtr(shaderName, shaderType);

        return addShader(shader);
    }

    void GlProgram::clearShaders()
    {
        _shaders.clear();
    }

    bool GlProgram::link()
    {
        if(_id != 0)
        {
            glDeleteProgram(_id);
            _id = 0;
        }
        _id = glCreateProgram();
        if(_id == 0)
        {
            getLog().postMessage(new Message('I', false,
                "Shader program could not be created", "GlProgram"));
            return false;
        }

        // Attach provided shaders
        string logSupInfo = " (using shaders ids={";
        for (auto s=_shaders.begin(); s != _shaders.end(); ++s)
        {
            glAttachShader( _id, (*s)->id() );
            logSupInfo += toString((*s)->id()) + ' ';
        }
        logSupInfo += "}";

        GlInputsOutputs::IOit inIt = _inAndOutLocations.inputs().begin();
        for(;inIt != _inAndOutLocations.inputs().end(); ++inIt)
            glBindAttribLocation(_id, inIt->first, inIt->second.c_str());

        GlInputsOutputs::IOit outIt = _inAndOutLocations.outputs().begin();
        for(;outIt != _inAndOutLocations.outputs().end(); ++outIt)
            glBindFragDataLocation(_id, outIt->first, outIt->second.c_str());


        glLinkProgram( _id );
        glGetProgramiv(_id, GL_LINK_STATUS, &_linked);


        if ( !_linked )
        {
            int infologLength = 0;
            glGetProgramiv( _id, GL_INFO_LOG_LENGTH, &infologLength );

            char* infoLog = new char[infologLength+1];
            int charsWritten  = 0;
            glGetProgramInfoLog( _id, infologLength, &charsWritten, infoLog );

            _log = "Errors in shader program (id): ";
            _log.append( toString(_id) );
            _log.append("\n");
            _log.append(infoLog);
            getLog().postMessage(new Message('E', false, _log, "GlProgram"));
            delete[] infoLog;

            return false;
        }

        getLog().postMessage(new Message('I', false,
            "Shader program succefully linked" + logSupInfo, "GlProgram"));

        return true;
    }

    void GlProgram::setTexture(
        GLenum activeTexUnit,
        GLenum target,
        GLuint id)
    {
        auto it = _textures.find(activeTexUnit);
        if(it != _textures.end())
        {
            if(id != 0)
            {
                it->second.first = target;
                it->second.second = id;
            }
            else
            {
                _textures.erase(it);
            }
        }
        else
        {
            if(id != 0)
            {
                auto value = make_pair(target, id);
                _textures.insert(make_pair(activeTexUnit, value));
            }
        }
    }

    void GlProgram::setTexture(
        GLenum activeTexUnit,
        GLenum target,
        const std::string& imgName)
    {
        setTexture(
            activeTexUnit,
            target,
            GlToolkit::genTextureId(
                getImageBank().getImage(imgName)
            )
        );
    }

    void GlProgram::pushProgram(GLuint id)
    {
        _progStack.push( make_pair(id, (GlProgram*)nullptr) );
    }

    void GlProgram::pushFixedPipelineProgram()
    {
        pushProgram( 0 );
    }

    unsigned int GlProgram::popProgram()
    {
        assert( !_progStack.empty() );

        _progStack.pop();

        GLuint id;
        if(_progStack.empty())
        {
            glUseProgram( id = 0 );
        }
        else
        {
            id = _progStack.top().first;
            GlProgram* prog = _progStack.top().second;
            if(prog != nullptr)
            {
                prog->applyState();
            }
        }

        return id;
    }

    unsigned int GlProgram::currentProgramId()
    {
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &id);
        return id;
    }

    void GlProgram::pushProgram()
    {
        pushProgram( _id );
        applyState();
    }

    bool GlProgram::isCurrentProgram() const
    {
        return _id == currentProgramId();
    }

    int GlProgram::getAttributeLocation(const std::string& name) const
    {
        int loc;
        map<string, int>::const_iterator locIt = _cachedLocations.find(name);
        if(locIt == _cachedLocations.end())
        {
            loc = glGetAttribLocation(_id, name.c_str());
            if(loc != -1)
            {
                _cachedLocations.insert( make_pair(name, loc) );
            }
        }
        else
        {
            loc = locIt->second;
        }

        return loc;
    }

    int GlProgram::getUniformLocation(const std::string& name) const
    {
        int loc;
        map<string, int>::const_iterator locIt = _cachedLocations.find(name);
        if(locIt == _cachedLocations.end())
        {
            loc = glGetUniformLocation(_id, name.c_str());
            if(loc != -1)
            {
                _cachedLocations.insert( make_pair(name, loc) );
            }
        }
        else
        {
            loc = locIt->second;
        }

        return loc;
    }

    bool GlProgram::setInt(const std::string& var, int val)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform1i(loc, val);

        return true;
    }

    bool GlProgram::setVec2i(const std::string& var, const glm::ivec2& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform2iv(loc, 1, glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec3i(const std::string& var, const glm::ivec3& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform3iv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec4i(const std::string& var, const glm::ivec4& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform4iv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setFloat(const std::string& var, float val)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform1f(loc, val);

        return true;
    }

    bool GlProgram::setVec2f(const std::string& var, const glm::vec2& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform2fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec3f(const std::string& var, const glm::vec3& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform3fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec4f(const std::string& var, const glm::vec4& vec)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform4fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setMat3f(const std::string& var, const glm::mat3& mat)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

        return true;
    }

    bool GlProgram::setMat4f(const std::string& var, const glm::mat4& mat)
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

        return true;
    }

    void GlProgram::applyState()
    {
        // Program
        glUseProgram(_id);


        // Textures
        for(auto it = _textures.begin(); it != _textures.end(); ++it)
        {
            glActiveTexture(it->first);
            glBindTexture(it->second.first, it->second.second);
        }
        glActiveTexture(GL_TEXTURE0);


        // TODO wibus 2013-01-22: FBO

    }
}
