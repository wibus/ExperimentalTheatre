#ifndef PROPROOM3D_CONVERGENTFILM_H
#define PROPROOM3D_CONVERGENTFILM_H

#include "Film.h"


namespace prop3
{
    class PROP3D_EXPORT ConvergentFilm : public Film
    {
    public:
        ConvergentFilm();
        virtual ~ConvergentFilm();

        using Film::sample;
        using Film::addSample;
        using Film::pixelPriority;

        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;


    protected:
        virtual void endTileReached() override;
        virtual glm::dvec4 sample(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual void setColor(int index, const glm::dvec3& color) override;
        virtual void addSample(int index, const glm::dvec4& sample) override;


        // Variance stabilzes over time
        std::vector<double> _varianceBuffer;

        // Divergence decreases over time
        std::vector<double> _divergenceBuffer;

        // rgb accumulation and its total weight
        std::vector<glm::dvec4> _weightedColorBuffer;

        int _minThresholdFrameCount;
        int _forceFullFrameCycleCount;
        glm::dvec3 _maxPixelIntensity;
    };
}

#endif // PROPROOM3D_CONVERGENTFILM_H
