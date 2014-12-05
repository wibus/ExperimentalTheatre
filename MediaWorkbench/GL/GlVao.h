#ifndef MEDIAWORKBENCH_GLVAO_H
#define MEDIAWORKBENCH_GLVAO_H

#include <string>
#include <vector>
#include <map>

#include <GL3/gl3w.h>

#include "libMediaWorkbench_global.h"
#include "DataStructure/Vector.h"


namespace media
{
    template <typename ELEM_T, int NB_COMP, GLenum COMP_T>
    struct GlVbo
    {
        GlVbo();

        typedef ELEM_T  ElementType;
        static const int    NB_COMPONENTS  = NB_COMP;
        static const GLenum COMPONENT_TYPE = COMP_T;

        GLenum target;
        GLenum usage;
        int attribLocation;
        std::vector<ELEM_T> dataArray;
    };

    typedef GlVbo<cellar::Vec2i, 2, GL_INT>   GlVbo2Di;
    typedef GlVbo<cellar::Vec2f, 2, GL_FLOAT> GlVbo2Df;
    typedef GlVbo<cellar::Vec3i, 3, GL_INT>   GlVbo3Di;
    typedef GlVbo<cellar::Vec3f, 3, GL_FLOAT> GlVbo3Df;
    typedef GlVbo<cellar::Vec4i, 4, GL_INT>   GlVbo4Di;
    typedef GlVbo<cellar::Vec4f, 4, GL_FLOAT> GlVbo4Df;


    class MEDIA_EXPORT GlVao
    {
    public:
        GlVao();
        virtual ~GlVao();

        GLuint id() const;
        GLuint bufferId(const std::string& bufferName) const;
        int nbBuffers() const;

        void bind() const;
        static void unbind();

        template <typename ELEM_T, int NB_COMP, GLenum COMP_T>
        GLuint createBuffer(const std::string& bufferName,
                            const GlVbo<ELEM_T, NB_COMP, COMP_T>& buff);
        GLuint deleteBuffer(const std::string& bufferName);

    private:
        GLuint _id;
        std::map<std::string, GLuint> _bufferIds;
    };



    // IMPLEMENTATION //
    template <typename T, int NB_COMP, GLenum COMP_T>
    GlVbo<T, NB_COMP, COMP_T>::GlVbo() :
        target(GL_ARRAY_BUFFER),
        usage(GL_STATIC_DRAW),
        attribLocation(0),
        dataArray()
    {
    }

    inline GLuint GlVao::id() const
    {
        return _id;
    }

    inline GLuint GlVao::bufferId(const std::string& bufferName) const
    {
        auto it = _bufferIds.find(bufferName);

        if(it == _bufferIds.end())
            return 0;

        return it->second;
    }

    inline int GlVao::nbBuffers() const
    {
        return static_cast<int>(_bufferIds.size());
    }

    template <typename ELEM_T, int NB_COMP, GLenum COMP_T>
    GLuint GlVao::createBuffer(const std::string& bufferName,
                               const GlVbo<ELEM_T, NB_COMP, COMP_T>& buff)
    {
        deleteBuffer(bufferName);

        bind();

        _bufferIds.insert( make_pair(bufferName, (GLuint) 0) );

        glGenBuffers(1,
                     &_bufferIds[bufferName]);

        glBindBuffer(buff.target,
                     _bufferIds[bufferName]);

        glBufferData(buff.target,
                     sizeof(ELEM_T) * buff.dataArray.size(),
                     buff.dataArray.data(),
                     buff.usage);

        glEnableVertexAttribArray(buff.attribLocation);

        glVertexAttribPointer(buff.attribLocation,
                              buff.NB_COMPONENTS,
                              buff.COMPONENT_TYPE,
                              0, 0, 0);

        unbind();

        return _bufferIds[bufferName];
    }
}

#endif // MEDIAWORKBENCH_GLVAO_H
