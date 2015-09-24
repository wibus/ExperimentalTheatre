#ifndef CELLARWORKBENCH_IMAGESAMPLER_H
#define CELLARWORKBENCH_IMAGESAMPLER_H

#include <memory>

#include <GLM/glm.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    class Image;
    class AbstractFilter;
    class AbstractWrapper;


    enum class ESamplerFilter
    {
        NEAREST,
        LINEAR
    };

    enum class ESamplerWrapper
    {
        CLAMP,
        REPEAT
    };


    class CELLAR_EXPORT ImageSampler
    {
    public:
        ImageSampler(const ESamplerFilter& filter = ESamplerFilter::NEAREST,
                     const ESamplerWrapper& wrapper = ESamplerWrapper::CLAMP);
        ~ImageSampler();

        void setFilter(const ESamplerFilter& filter);
        void setWrapper(const ESamplerWrapper& wrapper);

        cellar::ESamplerFilter filter() const;
        cellar::ESamplerWrapper wrapper() const;

        glm::dvec4 sample(const Image& image, double s, double t) const;
        glm::dvec4 sample(const Image& image, const glm::dvec2& pos) const;


    private:
        ESamplerFilter _filterType;
        ESamplerWrapper _wrapperType;
        std::unique_ptr<AbstractFilter> _filter;
        std::unique_ptr<AbstractWrapper> _wrapper;
    };



    // IMPLEMENTATION //
    inline cellar::ESamplerFilter ImageSampler::filter() const
    {
        return _filterType;
    }

    inline cellar::ESamplerWrapper ImageSampler::wrapper() const
    {
        return _wrapperType;
    }

    inline glm::dvec4 ImageSampler::sample(
            const Image& image,
            const glm::dvec2& pos) const
    {
        return sample(image, pos.s, pos.t);
    }
}

#endif // CELLARWORKBENCH_IMAGESAMPLER_H
