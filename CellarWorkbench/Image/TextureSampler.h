#ifndef CELLARWORKBENCH_TEXTURESAMPLER_H
#define CELLARWORKBENCH_TEXTURESAMPLER_H

#include <memory>

#include <GLM/glm.hpp>

#include <CellarWorkbench/libCellarWorkbench_global.h>


namespace cellar
{
    class Texture;
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


    class CELLAR_EXPORT TextureSampler
    {
    public:
        TextureSampler(const ESamplerFilter& filter = ESamplerFilter::NEAREST,
                     const ESamplerWrapper& wrapper = ESamplerWrapper::CLAMP);
        ~TextureSampler();

        void setFilter(const ESamplerFilter& filter);
        void setWrapper(const ESamplerWrapper& wrapper);

        cellar::ESamplerFilter filter() const;
        cellar::ESamplerWrapper wrapper() const;

        glm::dvec4 sample(const Texture& image, double s, double t) const;
        glm::dvec4 sample(const Texture& image, const glm::dvec2& pos) const;


    private:
        ESamplerFilter _filterType;
        ESamplerWrapper _wrapperType;
        std::unique_ptr<AbstractFilter> _filter;
        std::unique_ptr<AbstractWrapper> _wrapper;
    };



    // IMPLEMENTATION //
    inline cellar::ESamplerFilter TextureSampler::filter() const
    {
        return _filterType;
    }

    inline cellar::ESamplerWrapper TextureSampler::wrapper() const
    {
        return _wrapperType;
    }

    inline glm::dvec4 TextureSampler::sample(
            const Texture& image,
            const glm::dvec2& pos) const
    {
        return sample(image, pos.s, pos.t);
    }
}

#endif // CELLARWORKBENCH_TEXTURESAMPLER_H
