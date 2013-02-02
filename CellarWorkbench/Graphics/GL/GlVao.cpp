#include "GlVao.h"

using namespace std;

namespace cellar
{
    GlVao::GlVao() :
        _id(0),
        _bufferIds()
    {
    }

    GlVao::~GlVao()
    {
        glDeleteVertexArrays(1, &_id);

        for(auto it= _bufferIds.begin(); it != _bufferIds.end(); ++it)
        {
            glDeleteBuffers(1, &(it->second));
        }
    }

    void GlVao::bind() const
    {
        if(_id == 0)
        {
            glGenVertexArrays(1, const_cast<GLuint*>(&_id));
        }

        glBindVertexArray(_id);
    }

    void GlVao::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    GLuint GlVao::deleteBuffer(const std::string& bufferName)
    {
        auto it = _bufferIds.find(bufferName);
        if(it != _bufferIds.end())
        {
            glDeleteBuffers(1, &it->second);
            _bufferIds.erase(it);
        }

        return 0;
    }

}
