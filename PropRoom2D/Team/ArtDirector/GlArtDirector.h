#ifndef PROPROOM2D_GLARTDIRECTOR_H
#define PROPROOM2D_GLARTDIRECTOR_H

#include <vector>
#include <map>

#include <CellarWorkbench/GL/GlProgram.h>
#include <CellarWorkbench/GL/GlVao.h>

#include "AbstractArtDirector.h"



namespace prop2
{
    class PROP2D_EXPORT GlFont
    {
    public:
        GlFont(const std::string& name);

        bool operator==(const GlFont& font) const;
        bool operator!=(const GlFont& font) const;

        std::string name() const;
        unsigned int textureId() const;
        float charsHeight() const;
        float charWidth(unsigned char id) const;


    private:
        void loadFont();

        std::string _name;
        unsigned int _id;
        float _charsHeight;
        float _charsWidth[96];
    };

    class PROP2D_EXPORT GlArtDirector : public AbstractArtDirector
    {
    public:
        GlArtDirector();
        virtual ~GlArtDirector();

        virtual void notify(cellar::CameraMsg &msg);

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);
        virtual void update(double dt);

    protected:
        virtual void drawCircle(const std::shared_ptr<Circle>& circle);
        virtual void drawPolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void drawTextHud(const std::shared_ptr<TextHud>& text);
        virtual void drawImageHud(const std::shared_ptr<ImageHud>& image);

        virtual void setupCircleShader(const cellar::GlInputsOutputs& loc);
        virtual void setupPolygonShader(const cellar::GlInputsOutputs& loc);
        virtual void setupTextHudShader(const cellar::GlInputsOutputs& loc);
        virtual void setupImageHudShader(const cellar::GlInputsOutputs& loc);

        virtual glm::dvec2 getAnchor(const EHorizontalAnchor& h,
                                     const EVerticalAnchor&   v);

        std::map<std::string, GlFont> _fonts;

        bool _shadersInitialized;
        cellar::GlProgram _circleShader;
        cellar::GlProgram _polygonShader;
        cellar::GlProgram _textHudShader;
        cellar::GlProgram _imageHudShader;

        cellar::GlVao _circleVao;
        cellar::GlVao _polygonVao;
        cellar::GlVao _textHudVao;
        cellar::GlVao _imageHudVao;
    };



    // IMPLEMENTATION //
    inline bool GlFont::operator==(const GlFont& font) const
        {return (_name == font._name);}

    inline bool GlFont::operator!=(const GlFont& font) const
        {return (_name != font._name);}

    inline std::string GlFont::name() const
        {return  _name;}

    inline unsigned int GlFont::textureId() const
        {return _id;}

    inline float GlFont::charsHeight() const
        {return _charsHeight;}

    inline float GlFont::charWidth(unsigned char id) const
        {return _charsWidth[id];}
}

#endif // PROPROOM2D_GLARTDIRECTOR_H
