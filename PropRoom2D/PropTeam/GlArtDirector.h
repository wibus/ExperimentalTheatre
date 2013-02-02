#ifndef PROPROOM2D_GLARTDIRECTOR_H
#define PROPROOM2D_GLARTDIRECTOR_H

#include "libPropRoom2D_global.h"
#include "AbstractArtDirector.h"

#include <vector>
#include <queue>
#include <map>

#include <Graphics/GL/GlProgram.h>
#include <Graphics/GL/GlVao.h>


namespace prop2
{
class AbstractShape;

    class PROP2D_EXPORT ShapeOrderer
    {
    public: bool operator() (AbstractShape* lhs, AbstractShape* rhs);
    };

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

        virtual void setup();
        virtual void draw(real dt);
        virtual void updateViewportSize(int width, int height);

        virtual void manageCircle(const std::shared_ptr<Circle>& circle);
        virtual void managePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void manageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void manageImageHud(const std::shared_ptr<ImageHud>& image);

        virtual void unmanageCircle(const std::shared_ptr<Circle>& circle);
        virtual void unmanagePolygon(const std::shared_ptr<Polygon>& polygon);
        virtual void unmanageTextHud(const std::shared_ptr<TextHud>& text);
        virtual void unmanageImageHud(const std::shared_ptr<ImageHud>& image);

    protected:
        virtual void drawOrderedTransparentShapes();
        virtual void drawCircle(Circle* circle);
        virtual void drawPolygon(Polygon* polygon);
        virtual void drawTextHud(TextHud* text);
        virtual void drawImageHud(ImageHud* image);

        virtual void setupCircleShader(const cellar::GlInputsOutputs& loc);
        virtual void setupPolygonShader(const cellar::GlInputsOutputs& loc);
        virtual void setupTextHudShader(const cellar::GlInputsOutputs& loc);
        virtual void setupImageHudShader(const cellar::GlInputsOutputs& loc);

        virtual Vec2r getAnchor(const HorizontalAnchor::Enum& h,
                               const VerticalAnchor::Enum&   v);


        bool _shadersInitialized;
        Vec2r _viewportSize;
        std::vector< std::shared_ptr<Circle> >  _circles;
        std::vector< std::shared_ptr<Polygon> > _polygons;
        std::vector< std::shared_ptr<TextHud> >  _texts;
        std::vector< std::shared_ptr<ImageHud> > _images;
        std::map<std::string, GlFont> _fonts;

        std::priority_queue< AbstractShape*,
                             std::vector<AbstractShape*>,
                             ShapeOrderer > _tranparentShapes;

        std::shared_ptr<cellar::GlShader> _propVertexShader;
        std::shared_ptr<cellar::GlShader> _hudVertexShader;
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
