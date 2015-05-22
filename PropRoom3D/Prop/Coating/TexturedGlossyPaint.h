#ifndef PROPROOM3D_TEXTUREDGLOSSYPAINT_H
#define PROPROOM3D_TEXTUREDGLOSSYPAINT_H

#include "Coating.h"


namespace cellar
{
    class Image;
}


namespace prop3
{

    class PROP3D_EXPORT TexturedGlossyPaint :
            public Coating
    {
    public:
        TexturedGlossyPaint(
                const std::string& texName,
                const std::string& glossName,
                const glm::dvec3& defaultColor = glm::dvec3(1.0),
                double defaultGlossiness = 0.0,
                double varnishRefractiveIndex = 1.55);
        virtual ~TexturedGlossyPaint();

        virtual void brdf(
            std::vector<Raycast>& raycasts,
            const RayHitReport& report,
            const std::shared_ptr<Material>& leavedMaterial,
            const std::shared_ptr<Material>& enteredMaterial,
            unsigned int outRayCountHint) const override;

    private:
        cellar::Image& _texture;
        cellar::Image& _glossMap;
        glm::dvec3 _defaultColor;
        double _defaultGlossiness;
        double _varnishRefractiveIndex;
    };
}

#endif // PROPROOM3D_TEXTUREDGLOSSYPAINT_H
