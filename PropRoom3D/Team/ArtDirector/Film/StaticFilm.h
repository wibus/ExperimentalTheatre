#ifndef PROPROOM3D_STATICFILM_H
#define PROPROOM3D_STATICFILM_H

#include "Film.h"


namespace prop3
{
    class PROP3D_EXPORT StaticFilm : public Film
    {
    public:
        StaticFilm();
        virtual ~StaticFilm();

        using Film::pixelSample;
        using Film::addSample;
        using Film::pixelPriority;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) override;

        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;


    protected:
        virtual void endTileReached() override;
        virtual double pixelDivergence(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual glm::dvec4 pixelSample(int index) const override;
        virtual void setColor(int index, const glm::dvec3& color) override;
        virtual void addSample(int index, const glm::dvec4& sample) override;

        int _tileCompletedCount;
    };
}

#endif // PROPROOM3D_STATICFILM_H
