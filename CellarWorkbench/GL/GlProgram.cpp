#include "GlProgram.h"

#include <cassert>
#include <iostream>

#include <GLM/gtc/type_ptr.hpp>

#include <GL3/gl3w.h>

#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include "GlShader.h"
#include "GlShaderBank.h"
#include "GlToolkit.h"
#include "../Image/ImageBank.h"

using namespace std;
using namespace cellar;

namespace cellar
{
    stack<pair<GLuint, const GlProgram*> > GlProgram::_progStack =
            stack<pair<GLuint, const GlProgram*> >();

    GlProgram::GlProgram() :
        _id(0),
        _linked(false),
        _inAndOutLocations(),
        _shaders(),
        _textures(),
        _subroutines(),
        _cachedLocations()
    {
    }

    GlProgram::~GlProgram()
    {
        if(_id != 0)
        {
            for (auto s : _shaders)
            {
                glDetachShader( _id, s->id() );
            }

            glDeleteProgram(_id);
        }

        getLog().postMessage(new Message('I', false,
            "OpenGL program (id=" + toString(_id) + ") deleted", "GlProgram"));
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

    bool GlProgram::addShader(GLenum shaderType, const std::vector<std::string>& shaderNames)
    {
        shared_ptr<GlShader> shader =
            getGlShaderBank().getShaderPtr(shaderNames, shaderType);

        return addShader(shader);
    }

    void GlProgram::reset()
    {
        _binary.reset();

        _shaders.clear();
        _textures.clear();
        _subroutines.clear();

        _inAndOutLocations = GlInputsOutputs();
        _cachedLocations.clear();

        if(_id != 0)
        {
            glDeleteProgram(_id);

            getLog().postMessage(new Message('I', false,
                "OpenGL program (id=" + toString(_id) + ") deleted", "GlProgram"));

            _linked = false;

            _id = 0;
        }
    }

    bool GlProgram::link()
    {
        if(_id != 0)
        {
            _binary.reset();
            _cachedLocations.clear();

            glDeleteProgram(_id);
            _linked = false;

            getLog().postMessage(new Message('I', false,
                "OpenGL program (id=" + toString(_id) + ") deleted", "GlProgram"));

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
        string logSupInfo = " (id = " + toString(_id) + " using shader ids = {";
        for (auto s=_shaders.begin(); s != _shaders.end(); ++s)
        {
            glAttachShader( _id, (*s)->id() );
            logSupInfo += toString((*s)->id()) + ' ';
        }
        logSupInfo.back() = '}';
        logSupInfo += ")";

        GlInputsOutputs::IOit inIt = _inAndOutLocations.inputs().begin();
        for(;inIt != _inAndOutLocations.inputs().end(); ++inIt)
            glBindAttribLocation(_id, inIt->first, inIt->second.c_str());

        GlInputsOutputs::IOit outIt = _inAndOutLocations.outputs().begin();
        for(;outIt != _inAndOutLocations.outputs().end(); ++outIt)
            glBindFragDataLocation(_id, outIt->first, outIt->second.c_str());


        glLinkProgram( _id );
        glGetProgramiv(_id, GL_LINK_STATUS, &_linked);

        int infologLength = 0;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infologLength );
        if( infologLength == 0 )
        {
            getLog().postMessage(new Message('I', false,
                "Shader program succefully linked" + logSupInfo, "GlProgram"));
        }
        else
        {
            char* infoLog = new char[infologLength+1];
            int charsWritten  = 0;
            glGetProgramInfoLog(_id, infologLength, &charsWritten, infoLog );

            string log = "Errors in shader program (id): ";
            log.append( toString(_id) + "\n" + infoLog );
            getLog().postMessage(new Message('E', false, log, "GlProgram"));
            delete[] infoLog;
        }

        return _linked;
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

    void GlProgram::setSubroutine(
        GLenum shaderType,
        const std::string& uniformName,
        const std::string& subroutineName)
    {
        GLint location = glGetSubroutineUniformLocation(
            _id, shaderType, uniformName.c_str());

        if(location != -1)
        {
            GLuint index = glGetSubroutineIndex(
                _id, shaderType, subroutineName.c_str());

            if(index != GL_INVALID_INDEX)
            {
                std::vector<GLuint>& subLoc = _subroutines[shaderType];
                if(subLoc.size() <= location)
                    subLoc.resize(location+1);

                subLoc[location] = index;
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Shader stage doesn't provide this subroutine implementation : "
                        + subroutineName,
                    "GlProgram"));
            }
        }
        else
        {
            getLog().postMessage(new Message('W', false,
                "Shader stage doesn't provide this subroutine uniform : "
                    + uniformName,
                "GlProgram"));
        }
    }

    void GlProgram::pushProgram(GLuint id)
    {
        glUseProgram( id );
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
            const GlProgram* prog = _progStack.top().second;
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

    void GlProgram::pushProgram() const
    {
        _progStack.push( make_pair(_id, this) );
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

    bool GlProgram::setInt(const std::string& var, int val) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform1i(loc, val);

        return true;
    }

    bool GlProgram::setVec2i(const std::string& var, const glm::ivec2& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform2iv(loc, 1, glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec3i(const std::string& var, const glm::ivec3& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform3iv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec4i(const std::string& var, const glm::ivec4& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform4iv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setUnsigned(const std::string& var, unsigned int val) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform1ui(loc, val);

        return true;
    }

    bool GlProgram::setVec2u(const std::string& var, const glm::uvec2& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform2uiv(loc, 1, glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec3u(const std::string& var, const glm::uvec3& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform3uiv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec4u(const std::string& var, const glm::uvec4& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform4uiv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setFloat(const std::string& var, float val) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform1f(loc, val);

        return true;
    }

    bool GlProgram::setVec2f(const std::string& var, const glm::vec2& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform2fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec3f(const std::string& var, const glm::vec3& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform3fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setVec4f(const std::string& var, const glm::vec4& vec) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniform4fv(loc, 1,  glm::value_ptr(vec));

        return true;
    }

    bool GlProgram::setMat3f(const std::string& var, const glm::mat3& mat) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

        return true;
    }

    bool GlProgram::setMat4f(const std::string& var, const glm::mat4& mat) const
    {
        if( !isCurrentProgram() )
            return false;

        int loc = getUniformLocation(var);
        if(loc == -1)
            return false;

        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));

        return true;
    }

    const GlProgramBinary& GlProgram::getBinary()
    {
        if(_binary.get() == nullptr)
        {
            _binary.reset(new GlProgramBinary());
            glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (GLint*)(&_binary->format));
            glGetProgramiv(_id, GL_PROGRAM_BINARY_LENGTH, &_binary->length);

            _binary->binary = new GLchar[_binary->length];
            glGetProgramBinary(_id, _binary->length, nullptr,
                               &_binary->format, _binary->binary);
        }

        return *_binary;
    }

    void GlProgram::applyState() const
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

        for(const auto& sub : _subroutines)
        {
            GLint activeSubroutineUniformLocations = 0;
            glGetProgramStageiv(_id, sub.first,
                GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS,
                &activeSubroutineUniformLocations);

            if(activeSubroutineUniformLocations == sub.second.size())
            {
                glUniformSubroutinesuiv(sub.first,
                    sub.second.size(), sub.second.data());
            }
            else
            {
                getLog().postMessage(new Message('E', false,
                    "Shader active subroutine uniform locations for shader type " +
                    to_string(sub.first) + " doesn't match specified one (" +
                    to_string(sub.second.size()) + " != " +
                    to_string(activeSubroutineUniformLocations) + ")",
                    "GlProgram"));
            }
        }
    }
}
