#include "AbstractFilm.h"

namespace prop3
{
    AbstractFilm::AbstractFilm() :
        _resolution(1, 1),
        _weightedColorBuffer(1, glm::dvec4(0))
    {

    }

    AbstractFilm::~AbstractFilm()
    {

    }

    void AbstractFilm::resize(const glm::ivec2& resolution)
    {
        if(_resolution != resolution)
        {
            _resolution = resolution;
            clear(glm::dvec3(0.0));
        }
    }

    void AbstractFilm::clear(const glm::dvec3& color)
    {
        _weightedColorBuffer.clear();
        _weightedColorBuffer.resize(
            _resolution.x * _resolution.y,
            glm::dvec4(color, 0.0));
    }

    void AbstractFilm::getColorBuffer(std::vector<glm::vec3>& buff) const
    {
        size_t buffSize = _weightedColorBuffer.size();
        buff.resize(buffSize);

        for(size_t i=0; i < buffSize; ++i)
        {
            const glm::dvec4 colorWeight = _weightedColorBuffer[i];
            if(colorWeight.w != 0.0)
                buff[i] = glm::vec3(colorWeight) / float(colorWeight.w);
            else
                buff[i] = glm::vec3(0.0, 0.0, 0.0);
        }
    }

    void AbstractFilm::mergeFilm(const AbstractFilm& film)
    {
        int buffSize = _resolution.x * _resolution.y;
        for(int i=0; i < buffSize; ++i)
        {
            addSample(i, film._weightedColorBuffer[i]);
        }
    }

    void AbstractFilm::blitFilm(
            const glm::ivec2& origin,
            const AbstractFilm& film)
    {
        glm::ivec2 botBounds = glm::clamp(origin, glm::ivec2(0), _resolution);
        glm::ivec2 topBounds = glm::clamp(origin + film.resolution(), glm::ivec2(0), _resolution);
        for(int thisJ=botBounds.y, otherJ=0; thisJ < topBounds.y; ++thisJ, ++otherJ)
        {
            int otherIndex = otherJ * film.resolution().x;
            int thisIndex = botBounds.x + thisJ * _resolution.x;
            for(int thisI=botBounds.x; thisI < topBounds.x;
                ++thisI, ++thisIndex, ++otherIndex)
            {
                setSample(thisIndex, film.weightedColorBuffer()[otherIndex]);
            }
        }
    }
}
