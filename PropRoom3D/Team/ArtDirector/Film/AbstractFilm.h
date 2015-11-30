#ifndef PROPROOM3D_ABSTRACTFILM_H
#define PROPROOM3D_ABSTRACTFILM_H

#include <vector>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class PROP3D_EXPORT AbstractFilm
    {
    public:
        AbstractFilm();
        virtual ~AbstractFilm();

        int width() const;
        int height() const;
        glm::ivec2 resolution() const;
        virtual void resize(int width, int height);
        virtual void resize(const glm::ivec2& resolution);

        virtual void clear(const glm::dvec3& color = glm::dvec3(0));

        const std::vector<glm::dvec4>& weightedColorBuffer() const;

        virtual void getColorBuffer(std::vector<glm::vec3>& buff) const;

        virtual double compileDivergence() const = 0;

        void addSample(int i, int j, const glm::dvec4& sample);

        void addSample(const glm::ivec2& position, const glm::dvec4& sample);

        virtual void mergeFilm(const AbstractFilm& film);

        virtual void blitFilm(const glm::ivec2& origin, const AbstractFilm& film);


    protected:
        virtual void setSample(int index, const glm::dvec4& sample) = 0;
        virtual void addSample(int index, const glm::dvec4& sample) = 0;

        glm::ivec2 _resolution;
        std::vector<glm::dvec4> _weightedColorBuffer;
    };



    // IMPLEMENTATION //
    inline int AbstractFilm::width() const
    {
        return _resolution.x;
    }

    inline int AbstractFilm::height() const
    {
        return _resolution.y;
    }

    inline glm::ivec2 AbstractFilm::resolution() const
    {
        return _resolution;
    }

    inline void AbstractFilm::resize(int width, int height)
    {
        resize(glm::ivec2(width, height));
    }

    inline const std::vector<glm::dvec4>& AbstractFilm::weightedColorBuffer() const
    {
        return _weightedColorBuffer;
    }

    inline void AbstractFilm::addSample(
            int i, int j, const glm::dvec4& sample)
    {
        int index = i + j * _resolution.x;
        addSample(index, sample);
    }

    inline void AbstractFilm::addSample(
            const glm::ivec2& position, const glm::dvec4& sample)
    {
        int index = position.x + position.y * _resolution.x;
        addSample(index, sample);
    }
}

#endif // PROPROOM3D_ABSTRACTFILM_H
