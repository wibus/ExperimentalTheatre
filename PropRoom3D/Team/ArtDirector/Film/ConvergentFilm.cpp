#include "ConvergentFilm.h"

namespace prop3
{
    ConvergentFilm::ConvergentFilm() :
        _divergenceBuffer(1, 0.0)
    {

    }

    ConvergentFilm::~ConvergentFilm()
    {

    }

    void ConvergentFilm::clear(const glm::dvec3& color)
    {
        AbstractFilm::clear(color);

        _divergenceBuffer.clear();
        _divergenceBuffer.resize(
            resolution().x * resolution().y,
            0.0);
    }

    double ConvergentFilm::compileDivergence() const
    {
        double meanDivergenceSqr = 0.0;
        size_t buffSize = _divergenceBuffer.size();
        for(size_t i=0; i < buffSize; ++i)
        {
            double divergence = _divergenceBuffer[i];
            meanDivergenceSqr += divergence*divergence;
        }

        meanDivergenceSqr /= double(buffSize);
        double stdDev = glm::sqrt(meanDivergenceSqr);

        return stdDev;
    }

    void ConvergentFilm::setSample(int index, const glm::dvec4& sample)
    {
        _weightedColorBuffer[index] = sample;
        _divergenceBuffer[index] = 0.0;
    }

    void ConvergentFilm::addSample(int index, const glm::dvec4& sample)
    {
        glm::dvec4 oldSample = _weightedColorBuffer[index];
        glm::dvec4 newSample = oldSample + sample;
        _weightedColorBuffer[index] = newSample;

        if(oldSample.w != 0.0)
        {
            double delta = glm::length(
                glm::dvec3(oldSample) / oldSample.w -
                glm::dvec3(newSample) / newSample.w);

            _divergenceBuffer[index] = delta;
        }
    }
}
