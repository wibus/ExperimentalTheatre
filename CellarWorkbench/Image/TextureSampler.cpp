#include "TextureSampler.h"

#include "Texture.h"


namespace cellar
{
    class AbstractWrapper
    {
    public:
        virtual ~AbstractWrapper() {}
        virtual void wrap(
                const Texture& texture,
                int& i, int& j) const = 0;
    };

    class ClampWrapper : public AbstractWrapper
    {
        virtual void wrap(
                const Texture& texture,
                int& i, int& j) const override
        {
            i = glm::clamp(i, 0, texture.width()-1);
            j = glm::clamp(j, 0, texture.height()-1);
        }
    };

    class RepeatWrapper : public AbstractWrapper
    {
        virtual void wrap(
                const Texture& texture,
                int& i, int& j) const override
        {
            i = i % texture.width();
            if(i < 0) i = texture.width()+i;

            j = j % texture.height();
            if(j < 0) j = texture.height()+j;
        }
    };


    class AbstractFilter
    {
    public:
        virtual ~AbstractFilter() {}
        virtual glm::vec3 filter(
            const Texture& texture,
            const AbstractWrapper& wrapper,
            double s, double t) const = 0;
    };

    class NearestFilter : public AbstractFilter
    {
    public:
        virtual glm::vec3 filter(
            const Texture& texture,
            const AbstractWrapper& wrapper,
            double s, double t) const override
        {
            int i = texture.width() * s;
            int j = texture.height() * t;
            wrapper.wrap(texture, i, j);
            return texture.at(i, j);
        }
    };

    class LinearFilter : public AbstractFilter
    {
    public:
        virtual glm::vec3 filter(
            const Texture& texture,
            const AbstractWrapper& wrapper,
            double s, double t) const override
        {
            double i = texture.width() * s;
            double j = texture.height() * t;
            int li = glm::floor(i);
            int lj = glm::floor(j);
            int hi = glm::ceil(i);
            int hj = glm::ceil(j);
            double ai = i - li;
            double aj = j - lj;

            wrapper.wrap(texture, li, lj);
            wrapper.wrap(texture, hi, hj);

            glm::vec3 lilj = texture.at(li, lj);
            glm::vec3 lihj = texture.at(li, hj);
            glm::vec3 hilj = texture.at(hi, lj);
            glm::vec3 hihj = texture.at(hi, hj);

            return glm::mix(
                glm::mix(lilj, lihj, aj),
                glm::mix(hilj, hihj, aj),
                ai);
        }
    };


    TextureSampler::TextureSampler(
            const ESamplerFilter& filter,
            const ESamplerWrapper& wrapper)
    {
        setFilter(filter);
        setWrapper(wrapper);
    }

    TextureSampler::~TextureSampler()
    {

    }

    void TextureSampler::setFilter(const ESamplerFilter& filter)
    {
        _filterType = filter;

        switch(_filterType)
        {
        case ESamplerFilter::NEAREST :
            _filter.reset(new NearestFilter());
            break;
        case ESamplerFilter::LINEAR :
            _filter.reset(new LinearFilter());
            break;
        default:
            _filter.reset();
        }
    }

    void TextureSampler::setWrapper(const ESamplerWrapper& wrapper)
    {
        _wrapperType = wrapper;

        switch(_wrapperType)
        {
        case ESamplerWrapper::CLAMP :
            _wrapper.reset(new ClampWrapper());
            break;
        case ESamplerWrapper::REPEAT :
            _wrapper.reset(new RepeatWrapper());
            break;
        default:
            _wrapper.reset();
        }
    }

    glm::dvec4 TextureSampler::sample(const Texture& texture, double s, double t) const
    {
        return glm::dvec4(_filter->filter(texture, *_wrapper, s, t), 1.0);
    }
}
