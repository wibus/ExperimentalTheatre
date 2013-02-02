#include "GlArtDirector.h"
#include "GlArtDirectorShaders.h"

#include "Shape/Circle.h"
#include "Costume/CircleCostume.h"
#include "Shape/Polygon.h"
#include "Costume/PolygonCostume.h"

#include "Hud/TextHud.h"
#include "Hud/ImageHud.h"

#include <cassert>

#include <fstream>
using namespace std;

#include <GL/glew.h>

#include <Graphics/Image.h>
#include <Graphics/ImageBank.h>
#include <Graphics/GL/GlToolkit.h>
#include <Misc/Log.h>
using namespace cellar;


namespace prop2
{
    bool ShapeOrderer::operator() (AbstractShape* lhs, AbstractShape* rhs)
    {
        return lhs->abstractCostume().depth() <
               rhs->abstractCostume().depth();
    }

    GlArtDirector::GlArtDirector() :
        _shadersInitialized(false),
        _viewportSize(real(1.0), real(1.0)),
        _circles(),
        _polygons(),
        _texts(),
        _images(),
        _fonts(),
        _propVertexShader(new GlShader(GL_VERTEX_SHADER)),
        _hudVertexShader(new GlShader(GL_VERTEX_SHADER)),
        _circleShader(),
        _polygonShader(),
        _textHudShader(),
        _imageHudShader(),
        _circleVao(),
        _polygonVao(),
        _textHudVao(),
        _imageHudVao()
    {
    }

    GlArtDirector::~GlArtDirector()
    {
    }

    void GlArtDirector::setup()
    {
        if(!_shadersInitialized)
        {
            GlInputsOutputs locations;
            locations.setInput(0, "position");
            locations.setInput(1, "texCoord");
            locations.setInput(2, "color");

            _propVertexShader->loadFromString(COMMON_VERTEX_SHADER);
            _hudVertexShader->loadFromString(HUD_VERTEX_SHADER);

            setupCircleShader(locations);
            setupPolygonShader(locations);
            setupTextHudShader(locations);
            setupImageHudShader(locations);

            updateViewportSize(_viewportSize.x(), _viewportSize.y());

            _shadersInitialized = true;
        }

        _circles.clear();
        _polygons.clear();
        _texts.clear();
        _images.clear();
    }

    void GlArtDirector::updateViewportSize(int width, int height)
    {
        _viewportSize(width, height);
        Mat4r projection;
        projection.ortho(0, width, 0, height, -1.0, 1.0);

        _circleShader.pushProgram();
        _circleShader.setMatrix4x4("Projection", projection);
        _circleShader.popProgram();

        _polygonShader.pushProgram();
        _polygonShader.setMatrix4x4("Projection", projection);
        _polygonShader.popProgram();

        _textHudShader.pushProgram();
        _textHudShader.setMatrix4x4("Projection", projection);
        _textHudShader.popProgram();

        _imageHudShader.pushProgram();
        _imageHudShader.setMatrix4x4("Projection", projection);
        _imageHudShader.popProgram();
    }

    void GlArtDirector::draw(real)
    {        
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        for(auto it = _circles.begin(); it != _circles.end(); ++it)
        {
            Circle* circle = (*it).get();
            if(circle->isVisible())
            {
                if(circle->costume()->isOpaque())
                {
                    drawCircle(circle);
                }
                else
                {
                    _tranparentShapes.push(circle);
                }
            }
        }

        for(auto it = _polygons.begin(); it != _polygons.end(); ++it)
        {
            Polygon* polygon = (*it).get();
            if(polygon->isVisible())
            {
                if(polygon->costume()->isOpaque())
                {
                    drawPolygon(polygon);
                }
                else
                {
                    _tranparentShapes.push(polygon);
                }
            }
        }

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        drawOrderedTransparentShapes();

        glDisable(GL_DEPTH_TEST);


        // Draw images hud before text because its usualy what we want
        for(auto it = _images.begin(); it != _images.end(); ++it)
        {
            ImageHud* image = (*it).get();
            if(image->isVisible())
            {
                drawImageHud(image);
            }
        }

        // Draw texts on top of images hud
        for(auto it = _texts.begin(); it != _texts.end(); ++it)
        {
            TextHud* text = (*it).get();
            if(text->isVisible())
            {
                drawTextHud(text);
            }
        }

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

    void GlArtDirector::drawOrderedTransparentShapes()
    {
        while(!_tranparentShapes.empty())
        {
            AbstractShape* shape = _tranparentShapes.top();
            _tranparentShapes.pop();

            switch(shape->propType())
            {
            case PropType::CIRCLE:
                drawCircle(static_cast<Circle*>(shape));
                break;

            case PropType::POLYGON:
                drawPolygon(static_cast<Polygon*>(shape));
                break;

            default:
                assert(false /* Not a shape */);
            }
        }
    }

    void GlArtDirector::drawCircle(Circle* circle)
    {
        _circleShader.pushProgram();
        _circleVao.bind();
        _circleShader.setMatrix3x3("ModelView", circle->transformMatrix());
        _circleShader.setFloat("Depth",         circle->costume()->depth());
        _circleShader.setVec4f("ColorFilter",   circle->costume()->colorFilter());
        _circleShader.setVec2f("TexOffset",     circle->costume()->textureCenter());
        _circleShader.setFloat("TexStretch",    circle->costume()->textureRadius());
        glBindTexture(GL_TEXTURE_2D, GlToolkit::genTextureId(
            getImageBank().getImage(circle->costume()->textureName())));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _circleVao.unbind();
        _circleShader.popProgram();
    }

    void GlArtDirector::drawPolygon(Polygon* polygon)
    {
        _polygonShader.pushProgram();
        _polygonVao.bind();

        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER,
                     2*4*polygon->relVertices().size(),
                     polygon->relVertices().data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("color"));
        glBufferData(GL_ARRAY_BUFFER,
                     4*4*polygon->costume()->verticesColors().size(),
                     polygon->costume()->verticesColors().data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER,
                     2*4*polygon->costume()->verticesTexCoords().size(),
                     polygon->costume()->verticesTexCoords().data(),
                     GL_DYNAMIC_DRAW);

        _polygonShader.setMatrix3x3("ModelView", polygon->transformMatrix());
        _polygonShader.setFloat("Depth",         polygon->costume()->depth());
        _polygonShader.setVec4f("ColorFilter",   polygon->costume()->colorFilter());
        _polygonShader.setVec2f("TexOffset",     Vec2f(real(0.0), real(0.0)));
        _polygonShader.setFloat("TexStretch",    real(1.0));

        glBindTexture(GL_TEXTURE_2D, GlToolkit::genTextureId(
            getImageBank().getImage(polygon->costume()->textureName())));
        glDrawArrays(GL_TRIANGLE_FAN, 0, polygon->relVertices().size());

        _polygonVao.unbind();
        _polygonShader.popProgram();
    }

    void GlArtDirector::drawTextHud(TextHud* text)
    {
        _textHudShader.pushProgram();
        _textHudVao.bind();

        // Find text's font
        auto cit = _fonts.find(text->fontName());
        if(cit == _fonts.end())
        {
            _fonts.insert(make_pair(text->fontName(), GlFont(text->fontName())));
            cit = _fonts.find(text->fontName());
        }
        const GlFont& font = cit->second;

        // Compute texture coords and letters positions in screen coordinate
        vector<Vec2r> positions;
        vector<Vec2r> texCoords;
        const char* chars = text->text().c_str();
        size_t nbChars = text->text().size();

        real left = real(0.0);
        real right;
        real top = text->height();
        const real bottom = real(0.0);

        real texCharWidth;
        real texCharHeight = font.charsHeight();

        for(size_t c=0; c < nbChars; ++c)
        {
            // Texture
            int cid = chars[c] - 32;
            texCharWidth = font.charWidth(cid);
            real s = ((cid%10)/10.0) * 0.9765625; // Magic Number :0 !!!
            real t = ((cid/10)/10.0) * 0.9765625; // Proportion correction
            real ns = s + texCharWidth;
            real nt = t + texCharHeight;

            // Because normaly image origin is at top left and openGL
            // puts it at bottom left, t texture coordinate must inversed
            texCoords.push_back(Vec2r(s,  1-t));
            texCoords.push_back(Vec2r(s,  1-nt));
            texCoords.push_back(Vec2r(ns, 1-nt));
            texCoords.push_back(Vec2r(ns, 1-t));


            // Positions
            real charWidth = (texCharWidth / texCharHeight) * top;
            right = left + charWidth;

            positions.push_back(Vec2r(left,  top));
            positions.push_back(Vec2r(left,  bottom));
            positions.push_back(Vec2r(right, bottom));
            positions.push_back(Vec2r(right, top));

            // Move forward to next character's left
            left = right;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _textHudVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER, 2*4*positions.size(), positions.data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _textHudVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER, 2*4*texCoords.size(), texCoords.data(),
                     GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, font.textureId());

        Vec2r anchorPos = text->handlePosition() +
            getAnchor(text->horizontalAnchor(), text->verticalAnchor());
        _textHudShader.setVec2f("Anchor", anchorPos);
        _textHudShader.setVec4f("ColorFilter", text->color());

        glDrawArrays(GL_QUADS, 0, positions.size());

        _imageHudVao.unbind();
        _imageHudShader.popProgram();
    }

    void GlArtDirector::drawImageHud(ImageHud* image)
    {
        _imageHudShader.pushProgram();
        _imageHudVao.bind();

        glBindBuffer(GL_ARRAY_BUFFER, _imageHudVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER,
                     2*4*image->cornersPositions().size(),
                     image->cornersPositions().data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _imageHudVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER,
                     2*4*image->cornersTexCoords().size(),
                     image->cornersTexCoords().data(),
                     GL_DYNAMIC_DRAW);

        _imageHudShader.setVec2f("Anchor", getAnchor(image->horizontalAnchor(),
                                                     image->verticalAnchor()));
        _imageHudShader.setVec4f("ColorFilter", image->colorFilter());

        glBindTexture(GL_TEXTURE_2D, GlToolkit::genTextureId(
            getImageBank().getImage(image->imageName())));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        _imageHudVao.unbind();
        _imageHudShader.popProgram();
    }

    void GlArtDirector::manageCircle(const std::shared_ptr<Circle>& circle)
    {
        _circles.push_back(circle);
    }

    void GlArtDirector::managePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        _polygons.push_back(polygon);
    }

    void GlArtDirector::manageTextHud(const std::shared_ptr<TextHud>& text)
    {
        _texts.push_back(text);
    }

    void GlArtDirector::manageImageHud(const std::shared_ptr<ImageHud>& image)
    {
        _images.push_back(image);
    }

    void GlArtDirector::unmanageCircle(const std::shared_ptr<Circle>& circle)
    {
        for(auto it = _circles.begin(); it != _circles.end(); ++it)
        {
            if((*it)->id() == circle->id())
            {
                _circles.erase(it);
                return;
            }
        }
    }

    void GlArtDirector::unmanagePolygon(const std::shared_ptr<Polygon>& polygon)
    {
        for(auto it = _polygons.begin(); it != _polygons.end(); ++it)
        {
            if((*it)->id() == polygon->id())
            {
                _polygons.erase(it);
                return;
            }
        }
    }

    void GlArtDirector::unmanageTextHud(const std::shared_ptr<TextHud>& text)
    {
        for(auto it = _texts.begin(); it != _texts.end(); ++it)
        {
            if((*it)->id() == text->id())
            {
                _texts.erase(it);
                return;
            }
        }
    }

    void GlArtDirector::unmanageImageHud(const std::shared_ptr<ImageHud>& image)
    {
        for(auto it = _images.begin(); it != _images.end(); ++it)
        {
            if((*it)->id() == image->id())
            {
                _images.erase(it);
                return;
            }
        }
    }

    void GlArtDirector::setupCircleShader(const GlInputsOutputs& loc)
    {
        shared_ptr<GlShader> circleFrag(new GlShader(GL_FRAGMENT_SHADER));
        circleFrag->loadFromString(CIRCLE_FRAGMENT_SHADER);

        _circleShader.setInAndOutLocations(loc);
        _circleShader.addShader(_propVertexShader);
        _circleShader.addShader(circleFrag);
        _circleShader.link();
        _circleShader.pushProgram();
        _circleShader.setInt("Texture", 0);
        _circleShader.popProgram();

        GlVbo2Df positionBuff;
        positionBuff.attribLocation = _circleShader.getAttributeLocation("position");
        std::vector<Vec2r> positions = {
            Vec2r(-1.0f, -1.0f),
            Vec2r( 1.0f, -1.0f),
            Vec2r( 1.0f,  1.0f),
            Vec2r(-1.0f,  1.0f)};
        positionBuff.dataArray = positions;
        _circleVao.createBuffer("position", positionBuff);

        GlVbo2Df texCoordBuff;
        texCoordBuff.attribLocation = _circleShader.getAttributeLocation("texCoord");
        std::vector<Vec2r> texCoords = {
            Vec2r(-0.5f, -0.5f),
            Vec2r( 0.5f, -0.5f),
            Vec2r( 0.5f,  0.5f),
            Vec2r(-0.5f,  0.5f)};
        texCoordBuff.dataArray = texCoords;
        _circleVao.createBuffer("texCoord", texCoordBuff);
    }

    void GlArtDirector::setupPolygonShader(const GlInputsOutputs& loc)
    {
        shared_ptr<GlShader> polygonFrag(new GlShader(GL_FRAGMENT_SHADER));
        polygonFrag->loadFromString(POLYGON_FRAGMENT_SHADER);

        _polygonShader.setInAndOutLocations(loc);
        _polygonShader.addShader(_propVertexShader);
        _polygonShader.addShader(polygonFrag);
        _polygonShader.link();
        _polygonShader.pushProgram();
        _polygonShader.setInt("Texture", 0);
        _polygonShader.popProgram();

        GlVbo2Df positionBuff;
        positionBuff.attribLocation = _polygonShader.getAttributeLocation("position");
        _polygonVao.createBuffer("position", positionBuff);

        GlVbo2Df texCoordBuff;
        texCoordBuff.attribLocation = _polygonShader.getAttributeLocation("texCoord");
        _polygonVao.createBuffer("texCoord", texCoordBuff);

        GlVbo4Df colorBuff;
        colorBuff.attribLocation = _polygonShader.getAttributeLocation("color");
        _polygonVao.createBuffer("color", colorBuff);
    }

    void GlArtDirector::setupTextHudShader(const GlInputsOutputs& loc)
    {
        shared_ptr<GlShader> hudFrag(new GlShader(GL_FRAGMENT_SHADER));
        hudFrag->loadFromString(HUD_FRAGMENT_SHADER);

        _textHudShader.setInAndOutLocations(loc);
        _textHudShader.addShader(_hudVertexShader);
        _textHudShader.addShader(hudFrag);
        _textHudShader.link();
        _textHudShader.pushProgram();
        _textHudShader.setInt("Texture", 0);
        _textHudShader.popProgram();

        GlVbo2Df positionBuff;
        positionBuff.attribLocation = _textHudShader.getAttributeLocation("position");
        _textHudVao.createBuffer("position", positionBuff);

        GlVbo2Df texCoordBuff;
        texCoordBuff.attribLocation = _textHudShader.getAttributeLocation("texCoord");
        _textHudVao.createBuffer("texCoord", texCoordBuff);
    }

    void GlArtDirector::setupImageHudShader(const GlInputsOutputs& loc)
    {
        shared_ptr<GlShader> hudFrag(new GlShader(GL_FRAGMENT_SHADER));
        hudFrag->loadFromString(HUD_FRAGMENT_SHADER);

        _imageHudShader.setInAndOutLocations(loc);
        _imageHudShader.addShader(_hudVertexShader);
        _imageHudShader.addShader(hudFrag);
        _imageHudShader.link();
        _imageHudShader.pushProgram();
        _imageHudShader.setInt("Texture", 0);
        _imageHudShader.popProgram();

        GlVbo2Df positionBuff;
        positionBuff.attribLocation = _imageHudShader.getAttributeLocation("position");
        _imageHudVao.createBuffer("position", positionBuff);

        GlVbo2Df texCoordBuff;
        texCoordBuff.attribLocation = _imageHudShader.getAttributeLocation("texCoord");
        _imageHudVao.createBuffer("texCoord", texCoordBuff);
    }

    Vec2r GlArtDirector::getAnchor(const HorizontalAnchor::Enum& h,
                                  const VerticalAnchor::Enum&   v)
    {
        return Vec2r(h == HorizontalAnchor::RIGHT ? _viewportSize.x() : 0,
                     v == VerticalAnchor::TOP     ? _viewportSize.y() : 0);
    }

    GlFont::GlFont(const string& name) :
            _name(name),
            _id(0)
        {
            loadFont();
        }

        void GlFont::loadFont()
        {
            //Load font texture
            string fileFullName = "resources/fonts/" + _name;

            Image image = Image(fileFullName+ ".png");
            int size = image.dataSize();
            unsigned char * pixels = image.pixels();
            for(int i=0; i < size; i+=4)
            {
                pixels[i] = 255;
                pixels[i+1] = 255;
                pixels[i+2] = 255;
            }

            // Make font part of the image bank
            getImageBank().addImage("font-"+_name, image);
            _id =  GlToolkit::genTextureId(getImageBank().getImage("font-"+_name));


            //Load font format
            fstream flwFile;
            flwFile.open((fileFullName+".flw").c_str(), ios::binary|ios::in);
            if(!flwFile.fail())
            {
                char lw[96];
                char lh;
                char dimensions;

                //Extracting data
                flwFile.read(&dimensions,1);
                flwFile.read(&lh,1);
                flwFile.read(lw,96);
                flwFile.close();

                //Converting into rigth format
                _charsHeight = lh/pow((double)2,dimensions);
                for(int i=0; i<96; i++)
                        _charsWidth[i] = lw[i]/pow((double)2,dimensions);

                getLog().postMessage(new
                    Message('I', false, fileFullName + ".flw : OK", "Text"));
            }
            else
            {
                for(int i=0; i<96; i++)
                        _charsWidth[i] = 0.05;
                _charsHeight = 0.1;

                getLog().postMessage(new
                    Message('W', false, fileFullName + ".flw : Absent", "Text"));
            }
        }
}
