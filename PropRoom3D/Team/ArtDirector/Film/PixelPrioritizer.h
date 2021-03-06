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

        virtual void reset(const glm::ivec2& frameResolution);

        virtual void launchPrioritization(
                ConvergentFilm& film);

        virtual void displayPrioritization(
                ConvergentFilm& film);

        virtual double averagePriority() const;
        virtual double priorityThreshold() const;

        static const int KERNEL_WIDTH = 5;

    private:
        std::vector<double> _varBuff;
        std::vector<double> _tmpBuff;
        double _frameAvrgPriority;
    };
}

#endif //PROPROOM3D_PIXELPRIORITIZER_H
