#ifndef PROPROOM3D_CONVERGENTFILM_H
#define PROPROOM3D_CONVERGENTFILM_H

#include "AbstractFilm.h"


namespace prop3
{
    class PROP3D_EXPORT ConvergentFilm : public AbstractFilm
    {
    public:
        ConvergentFilm();
        virtual ~ConvergentFilm();

        virtual void clear(const glm::dvec3& color = glm::dvec3(0)) override;

        virtual double compileDivergence() const override;

    protected:
        virtual void setSample(int index, const glm::dvec4& sample) override;
        virtual void addSample(int index, const glm::dvec4& sample) override;

        std::vector<double> _divergenceBuffer;
    };
}

#endif // PROPROOM3D_CONVERGENTFILM_H
