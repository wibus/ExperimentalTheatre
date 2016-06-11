#ifndef PROPROOM3D_CONVERGENTFILM_H
#define PROPROOM3D_CONVERGENTFILM_H

#include <queue>

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

        using Film::addSample;
        using Film::pixelSample;
        using Film::pixelPriority;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) override;

        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) override;

        virtual void backupAsReferenceShot() override;

        virtual bool saveReferenceShot(const std::string& name) const override;
        virtual bool loadReferenceShot(const std::string& name) override;
        virtual bool clearReferenceShot() override;

        virtual bool saveRawFilm(const std::string& name) const override;
        virtual bool loadRawFilm(const std::string& name) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;

        virtual bool incomingTileAvailable() const override;
        virtual std::shared_ptr<TileMessage> nextIncomingTile() override;
        void addIncomingTile(const std::shared_ptr<TileMessage>& msg);


    protected:
        virtual void endTileReached() override;
        virtual double pixelDivergence(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual glm::dvec4 pixelSample(int index) const override;
        virtual void addSample(int index, const glm::dvec4& sample) override;

        virtual bool saveContent(
                const std::string& name,
                const std::vector<glm::dvec4>& samples,
                const std::vector<glm::dvec2>& variances) const;
        virtual bool loadContent(
                const std::string& name,
                std::vector<glm::dvec4>& samples,
                std::vector<glm::dvec2>& variances);

        glm::vec3 sampleToColor(const glm::dvec4& sample) const;
        glm::vec3 weightToColor(const glm::dvec4& sample) const;
        glm::vec3 divergenceToColor(double divergence) const;
        glm::vec3 varianceToColor(const glm::dvec2& variance) const;
        glm::vec3 priorityToColor(double priority) const;
        glm::vec3 compatibilityToColor(double compatibility) const;

        double toDivergence(
                const glm::dvec4& sample,
                double variance) const;

        double toPriority(
                const glm::dvec4& sample,
                double variance) const;

        double refShotCompatibility(unsigned int index) const;



        // RGB accumulation and its total weight
        std::vector<glm::dvec4> _sampleBuffer;

        // Variance stabilzes over time
        std::vector<glm::dvec2> _varianceBuffer;

        // Divergence decreases over time
        std::vector<double> _divergenceBuffer;

        // Priority stabilizes over time
        std::vector<double> _priorityBuffer;

        double _perceptibleIntensity;
        double _varianceWeightThreshold;
        double _priorityWeightThreshold;
        double _priorityWeightBias;
        double _priorityScale;

        glm::dvec3 _maxPixelIntensity;

        int _tileCompletedCount;

        // Random distribution
        cellar::LinearRand _linearRand;

        // Pixel Prioritizer
        std::shared_ptr<PixelPrioritizer> _prioritizer;

        std::mutex _tileMsgMutex;
        std::queue<std::shared_ptr<TileMessage>> _tileMsgs;


        struct ReferenceShot
        {
            std::vector<glm::dvec4> sampleBuffer;
            std::vector<glm::dvec2> varianceBuffer;
        } _referenceFilm;


        struct RawPixel
        {
            RawPixel();
            RawPixel(const glm::dvec4& sample, const glm::dvec2& variance);
            void toRaw(glm::dvec4& sample, glm::dvec2& variance) const;

            float var;
            float weight;
            unsigned short vw;
            unsigned short v;
            unsigned short r;
            unsigned short g;
            unsigned short b;

            static const double COLOR_SCALING;
            static const double VARIANCE_SCALING;
            static const double COLOR_DECOMPRESSION;
            static const double VARIANCE_DECOMPRESSION;
        };
        mutable std::vector<RawPixel> _rawPixelPool;
    };
}

#endif // PROPROOM3D_CONVERGENTFILM_H
