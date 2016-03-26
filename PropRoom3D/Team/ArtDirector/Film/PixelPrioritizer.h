#ifndef PROPROOM3D_PIXELPRIORITIZER_H
#define PROPROOM3D_PIXELPRIORITIZER_H

#include "Film.h"


namespace prop3
{
    class ConvergentFilm;

    class PROP3D_EXPORT PixelPrioritizer
    {
    public:
        PixelPrioritizer();
        virtual ~PixelPrioritizer();

        virtual void reset(
                const glm::ivec2& frameResolution,
                unsigned int blurWidth,
                double blurVariance);

        virtual bool launchPrioritization(
                ConvergentFilm& film);

        virtual double maxFramePriority() const;

    protected:
        virtual void computeGaussLine(
                std::vector<double>& gauss,
                unsigned int width,
                double var);

    private:
        std::vector<double> _tmpBuff;
        std::vector<double> _gauss;
        double _frameMaxPriority;
    };
}

#endif //PROPROOM3D_PIXELPRIORITIZER_H
