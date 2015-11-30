#include "StaticFilm.h"

namespace prop3
{
    StaticFilm::StaticFilm()
    {

    }

    StaticFilm::~StaticFilm()
    {

    }

    double StaticFilm::compileDivergence() const
    {
        return 1.0;
    }

    void StaticFilm::setSample(int index, const glm::dvec4& sample)
    {
        _weightedColorBuffer[index] = sample;
    }

    void StaticFilm::addSample(int index, const glm::dvec4& sample)
    {
        _weightedColorBuffer[index] += sample;
    }
}
