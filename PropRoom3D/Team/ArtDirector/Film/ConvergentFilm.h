#ifndef PROPROOM3D_CONVERGENTFILM_H
#define PROPROOM3D_CONVERGENTFILM_H

#include <CellarWorkbench/Misc/Distribution.h>

#include "Film.h"


namespace prop3
{
    class PixelPrioritizer;

    class PROP3D_EXPORT ConvergentFilm : public Film
    {
        friend class PixelPrioritizer;

    public:
        ConvergentFilm();
        virtual ~ConvergentFilm();

        using Film::pixelSample;
        using Film::addSample;
        using Film::pixelPriority;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) override;

        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) override;

        virtual void backupAsReferenceShot() override;

        virtual bool saveReferenceShot(const std::string& name) override;

        virtual bool loadReferenceShot(const std::string& name) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;


    protected:
        virtual void endTileReached() override;
        virtual double pixelDivergence(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual glm::dvec4 pixelSample(int index) const override;
        virtual void setColor(int index, const glm::dvec3& color) override;
        virtual void addSample(int index, const glm::dvec4& sample) override;

        glm::vec3 sampleToColor(const glm::dvec4& sample) const;
        glm::vec3 weightToColor(const glm::dvec4& sample) const;
        glm::vec3 divergenceToColor(double divergence) const;
        glm::vec3 varianceToColor(const glm::dvec2& variance) const;
        glm::vec3 priorityToColor(double priority) const;
        glm::vec3 compatibilityToColor(double compatibility) const;

        double toDivergence(
                const glm::dvec2& variance,
                const glm::dvec3& color,
                double weight) const;

        double refShotCompatibility(unsigned int index) const;



        // RGB accumulation and its total weight
        std::vector<glm::dvec4> _sampleBuffer;

        // Variance stabilzes over time
        std::vector<glm::dvec2> _varianceBuffer;

        // Divergence decreases over time
        std::vector<double> _divergenceBuffer;

        // Priority stabilizes over time
        std::vector<double> _priorityBuffer;

        double _varianceWeightThreshold;
        int _prioritySpanCycleCount;
        double _priorityWeightThreshold;
        double _priorityWeightBias;
        double _priorityScale;

        glm::dvec3 _maxPixelIntensity;

        // Random distribution
        cellar::LinearRand _linearRand;

        // Pixel Prioritizer
        std::shared_ptr<PixelPrioritizer> _prioritizer;

        struct ReferenceShot
        {
            std::vector<glm::dvec4> sampleBuffer;
            std::vector<glm::dvec2> varianceBuffer;
        } _referenceFilm;
    };
}

#endif // PROPROOM3D_CONVERGENTFILM_H
