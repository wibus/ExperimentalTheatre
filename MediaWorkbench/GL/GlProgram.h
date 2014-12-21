#ifndef MEDIAWORKBENCH_GLPROGRAM_H
#define MEDIAWORKBENCH_GLPROGRAM_H

#include <string>
#include <set>
#include <map>
#include <stack>
#include <memory>

#include <GL3/gl3w.h>

#include <CellarWorkbench/DataStructure/Vector.h>
#include <CellarWorkbench/DataStructure/Matrix.h>

#include "GlInputsOutputs.h"
#include "GlShader.h"


namespace media
{
    class MEDIA_EXPORT GlProgram
    {
    public:
        GlProgram();
        virtual ~GlProgram();

        GLuint id() const;
        const std::string& log() const;
        const GlInputsOutputs& InOutLocations() const;

        // Build process
        bool setInAndOutLocations(const GlInputsOutputs& inout);
        bool addShader(const std::shared_ptr<GlShader>& shader);
        bool addShader(GLenum shaderType, const std::string& shaderName);
        void clearShaders();
        bool link();

        // Specific program states
        void setTexture(
            GLenum activeTexUnit,
            GLenum target,
            GLuint id);
        void setTexture(
            GLenum activeTexUnit,
            GLenum target,
            const std::string& imgName);


        // Shader programs management
        static void pushProgram(GLuint id);
        static void pushFixedPipelineProgram();
        static unsigned int popProgram();
        static unsigned int currentProgramId();

        void pushProgram();
        bool isCurrentProgram() const;

        // Attributes
        int getAttributeLocation(const std::string& name) const;

        // Uniforms
        int getUniformLocation(const std::string& name) const;

        bool setInt(const std::string& var, int val);
        bool setVec2i(const std::string& var, const cellar::Vec2i& vec);
        bool setVec3i(const std::string& var, const cellar::Vec3i& vec);
        bool setVec4i(const std::string& var, const cellar::Vec4i& vec);

        bool setFloat(const std::string& var, float val);
        bool setVec2f(const std::string& var, const cellar::Vec2f& vec);
        bool setVec3f(const std::string& var, const cellar::Vec3f& vec);
        bool setVec4f(const std::string& var, const cellar::Vec4f& vec);

        bool setMat3f(const std::string& var, const cellar::Mat3f& mat);
        bool setMat4f(const std::string& var, const cellar::Mat4f& mat);

    private:
        void applyState();

        static std::stack<std::pair<GLuint, GlProgram*> > _progStack;

        GLuint _id;
        int _linked;
        std::string _log;
        GlInputsOutputs _inAndOutLocations;
        std::set<std::shared_ptr<GlShader> > _shaders;
        std::map<GLenum, std::pair<GLenum, GLuint> > _textures;
        mutable std::map<std::string, int> _cachedLocations;        
    };



    // IMPLEMENTATION //
    inline GLuint GlProgram::id() const
    {
        return _id;
    }

    inline const std::string& GlProgram::log() const
    {
        return _log;
    }

    inline const GlInputsOutputs& GlProgram::InOutLocations() const
    {
        return _inAndOutLocations;
    }
}

#endif // MEDIAWORKBENCH_GLPROGRAM_H
