#include "ImageSampler.h"

#include "Image.h"


namespace cellar
{
    class AbstractWrapper
    {
    public:
        virtual ~AbstractWrapper() {}
        virtual void wrap(
                const Image& image,
                int& i, int& j) const = 0;
    };

    class ClampWrapper : public AbstractWrapper
    {
        virtual void wrap(
                const Image& image,
                int& i, int& j) const override
        {
            i = glm::clamp(i, 0, image.width()-1);
            j = glm::clamp(j, 0, image.height()-1);
        }
    };

    class RepeatWrapper : public AbstractWrapper
    {
        virtual void wrap(
                const Image& image,
                int& i, int& j) const override
        {
            i = i % image.width();
            if(i < 0) i = image.width()+i;

            j = j % image.height();
            if(j < 0) j = image.height()+j;
        }
    };


    class AbstractFilter
    {
    public:
        virtual ~AbstractFilter() {}
        virtual glm::dvec4 filter(
            const Image& image,
            const AbstractWrapper& wrapper,
            double s, double t) const = 0;

    protected:
        inline glm::dvec4 get(const Image& image, int i, int j) const
        {
            const unsigned char* pix = image.pixel(i, j);
            return glm::dvec4(pix[0], pix[1], pix[2], pix[3]) / 255.0;
        }
    };

    class NearestFilter : public AbstractFilter
    {
    public:
        virtual glm::dvec4 filter(
            const Image& image,
            const AbstractWrapper& wrapper,
            double s, double t) const override
        {
            int i = image.width() * s;
            int j = image.height() * t;
            wrapper.wrap(image, i, j);
            return get(image, i, j);
        }
    };

    class LinearFilter : public AbstractFilter
    {
    public:
        virtual glm::dvec4 filter(
            const Image& image,
            const AbstractWrapper& wrapper,
            double s, double t) const override
        {
            double i = image.width() * s;
            double j = image.height() * t;
            int li = int(i);
            int lj = int(j);
            int hi = li+1;
            int hj = lj+1;
            double ai = i - li;
            double aj = j - lj;

            wrapper.wrap(image, li, lj);
            wrapper.wrap(image, hi, hj);

            glm::dvec4 lilj = get(image, li, lj);
            glm::dvec4 lihj = get(image, li, hj);
            glm::dvec4 hilj = get(image, hi, lj);
            glm::dvec4 hihj = get(image, hi, hj);

            return glm::mix(
                glm::mix(lilj, lihj, aj),
                glm::mix(hilj, hihj, aj),
                ai);
        }
    };


    ImageSampler::ImageSampler(
            const ESamplerFilter& filter,
            const ESamplerWrapper& wrapper)
    {
        setFilter(filter);
        setWrapper(wrapper);
    }

    ImageSampler::~ImageSampler()
    {

    }

    void ImageSampler::setFilter(const ESamplerFilter& filter)
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

    void ImageSampler::setWrapper(const ESamplerWrapper& wrapper)
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

    glm::dvec4 ImageSampler::sample(const Image& image, double s, double t) const
    {
        return _filter->filter(image, *_wrapper, s, t);
    }
}
