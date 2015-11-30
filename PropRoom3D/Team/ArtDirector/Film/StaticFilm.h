#ifndef PROPROOM3D_STATICFILM_H
#define PROPROOM3D_STATICFILM_H

#include "AbstractFilm.h"


namespace prop3
{
    class PROP3D_EXPORT StaticFilm : public AbstractFilm
    {
    public:
        StaticFilm();
        virtual ~StaticFilm();

        virtual double compileDivergence() const override;


    protected:
        virtual void setSample(int index, const glm::dvec4& sample) override;
        virtual void addSample(int index, const glm::dvec4& sample) override;
    };
}

#endif // PROPROOM3D_STATICFILM_H
