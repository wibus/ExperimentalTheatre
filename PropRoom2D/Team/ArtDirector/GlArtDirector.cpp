#include "GlArtDirector.h"

#include <cassert>

#include <algorithm>

#include <GLM/gtc/matrix_transform.hpp>

#include <GL3/gl3w.h>

#include <QFile>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/GL/GlToolkit.h>
#include <CellarWorkbench/Image/Image.h>
#include <CellarWorkbench/Image/ImageBank.h>
#include <CellarWorkbench/Misc/Log.h>

#include "../../Prop/Shape/Circle.h"
#include "../../Prop/Shape/Polygon.h"
#include "../../Prop/Costume/CircleCostume.h"
#include "../../Prop/Costume/PolygonCostume.h"
#include "../../Prop/Hud/TextHud.h"
#include "../../Prop/Hud/ImageHud.h"

using namespace std;
using namespace cellar;


namespace prop2
{
    bool DepthOrderer(const std::shared_ptr<AbstractShape>& lhs,
                      const std::shared_ptr<AbstractShape>& rhs)
    {
        return lhs->abstractCostume().depth() <
               rhs->abstractCostume().depth();
    }

    GlArtDirector::GlArtDirector() :
        _circles(),
        _polygons(),
        _texts(),
        _images(),
        _fonts(),
        _shadersInitialized(false),
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

    void GlArtDirector::notify(cellar::CameraMsg &msg)
    {
        if(!_shadersInitialized)
            return;

        if(msg.change == CameraMsg::EChange::VIEWPORT)
        {
            glm::mat4 hudProjectionMatrix =
                    glm::ortho(0.0f, (float) camera()->viewport().x,
                               0.0f, (float) camera()->viewport().y,
                               -1.0f, 1.0f);

            _textHudShader.pushProgram();
            _textHudShader.setMat4f("Projection", hudProjectionMatrix);
            _textHudShader.popProgram();

            _imageHudShader.pushProgram();
            _imageHudShader.setMat4f("Projection", hudProjectionMatrix);
            _imageHudShader.popProgram();
        }

        if(msg.change == CameraMsg::EChange::PROJECTION ||
           msg.change == CameraMsg::EChange::VIEW)
        {
            glm::vec2 zoom = glm::dvec2(1, 1);
            glm::mat4 shapeViewProjMatrix =
                    camera()->projectionMatrix() *
                    camera()->viewMatrix();

            _circleShader.pushProgram();
            _circleShader.setMat4f("Projection", shapeViewProjMatrix);
            _circleShader.setVec2f("Zoom", zoom);
            _circleShader.popProgram();

            _polygonShader.pushProgram();
            _polygonShader.setMat4f("Projection", shapeViewProjMatrix);
            _polygonShader.setVec2f("Zoom", zoom);
            _polygonShader.popProgram();
        }
    }

    void GlArtDirector::setup()
    {
        if(!_shadersInitialized)
        {
            GlInputsOutputs locations;
            locations.setInput(0, "position");
            locations.setInput(1, "texCoord");
            locations.setInput(2, "color");

            setupCircleShader(locations);
            setupPolygonShader(locations);
            setupTextHudShader(locations);
            setupImageHudShader(locations);

            _shadersInitialized = true;
            camera()->refresh();
        }
    }

    void GlArtDirector::reset()
    {
        _circles.clear();
        _polygons.clear();
        _texts.clear();
        _images.clear();
    }

    void GlArtDirector::draw(double)
    {
        int nbShapes = static_cast<int>(_circles.size() + _polygons.size());
        int nbHuds = static_cast<int>(_images.size() + _texts.size());
        if(nbShapes == 0 && nbHuds == 0)
            return;

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(nbShapes != 0)
        {
            // Depth sort shapes
            std::vector< std::shared_ptr<AbstractShape> > shapes(nbShapes);

            int shapeIdx = 0;
            for(const auto& c : _circles)
                shapes[shapeIdx++] = c;
            for(const auto& p : _polygons)
                shapes[shapeIdx++] = p;

            std::sort(shapes.begin(), shapes.end(), DepthOrderer);


            // Draw shapes under HUDs
            for(int i=0; i < nbShapes; ++i)
            {
                const std::shared_ptr<AbstractShape>& shape = shapes[i];
                switch(shape->propType())
                {
                case EPropType::CIRCLE:
                    drawCircle(static_pointer_cast<Circle>(shape));
                    break;

                case EPropType::POLYGON:
                    drawPolygon(static_pointer_cast<Polygon>(shape));
                    break;

                default:
                    assert(false /* Not a shape */);
                }
            }
        }

        if(nbHuds != 0)
        {
            glDisable(GL_MULTISAMPLE);

            // Draw images HUD before text because its usualy what we want
            for(const auto& i : _images)
                if(i->isVisible())
                    drawImageHud(i);

            // Draw texts on top of images HUD
            for(const auto& t : _texts)
                if(t->isVisible())
                    drawTextHud(t);

            glEnable(GL_MULTISAMPLE);
        }

        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

    void GlArtDirector::update(double dt)
    {

    }

    void GlArtDirector::drawCircle(const std::shared_ptr<Circle>& circle)
    {
        glDisable(GL_MULTISAMPLE);

        _circleShader.pushProgram();
        _circleShader.setMat3f("ModelView",   circle->transformMatrix());
        _circleShader.setFloat("Depth",       circle->costume()->depth());
        _circleShader.setVec4f("ColorFilter", circle->costume()->colorFilter());
        _circleShader.setVec2f("TexOffset",   circle->costume()->textureCenter());
        _circleShader.setFloat("TexStretch",  circle->costume()->textureRadius());
        _circleShader.setFloat("Radius",      circle->radius());
        _circleVao.bind();
        glBindTexture(GL_TEXTURE_2D, GlToolkit::genTextureId(
            getImageBank().getImage(circle->costume()->textureName())));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        _circleVao.unbind();
        _circleShader.popProgram();

        glEnable(GL_MULTISAMPLE);
    }

    void GlArtDirector::drawPolygon(const std::shared_ptr<Polygon>& polygon)
    {
        _polygonShader.pushProgram();
        _polygonShader.setMat3f("ModelView",    polygon->transformMatrix());
        _polygonShader.setFloat("Depth",        polygon->costume()->depth());
        _polygonShader.setVec4f("ColorFilter",  polygon->costume()->colorFilter());
        _polygonShader.setVec2f("TexOffset",    glm::vec2(0.0f, 0.0f));
        _polygonShader.setFloat("TexStretch",   1.0f);

        _polygonVao.bind();
        int nbVert = polygon->nbVertices();
        int posSize = nbVert * sizeof(polygon->relVertices().front());
        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER,
                     posSize,
                     polygon->relVertices().data(),
                     GL_DYNAMIC_DRAW);

        int colSize = nbVert * sizeof(polygon->costume()->verticesColors().front());
        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("color"));
        glBufferData(GL_ARRAY_BUFFER,
                     colSize,
                     polygon->costume()->verticesColors().data(),
                     GL_DYNAMIC_DRAW);

        int texSize = nbVert * sizeof(polygon->costume()->verticesTexCoords().front());
        glBindBuffer(GL_ARRAY_BUFFER, _polygonVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER,
                     texSize,
                     polygon->costume()->verticesTexCoords().data(),
                     GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, GlToolkit::genTextureId(
            getImageBank().getImage(polygon->costume()->textureName())));

        if(polygon->isConcave())
        {
            glDisable(GL_CULL_FACE);
            glEnable(GL_STENCIL_TEST);
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glStencilFunc(GL_NEVER, 0, 1);
            glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
            glClearStencil(0);

            glDrawArrays(GL_TRIANGLE_FAN, 0, nbVert);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glStencilFunc(GL_EQUAL, 1, 1);
            glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
            glEnable(GL_CULL_FACE);
        }

        glDrawArrays(GL_TRIANGLE_FAN, 0, nbVert);

        if(polygon->isConcave())
        {
            glDisable(GL_STENCIL_TEST);
        }

        _polygonVao.unbind();
        _polygonShader.popProgram();
    }

    void GlArtDirector::drawTextHud(const std::shared_ptr<TextHud>& text)
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
        vector<glm::dvec2> positions;
        vector<glm::dvec2> texCoords;
        const char* chars = text->text().c_str();
        size_t nbChars = text->text().size();

        double left = double(0.0);
        double right;
        double top = text->height();
        const double bottom = double(0.0);

        double texCharWidth;
        double texCharHeight = font.charsHeight();

        for(size_t c=0; c < nbChars; ++c)
        {
            // Texture
            int cid = chars[c] - 32;
            texCharWidth = font.charWidth(cid);
            double s = ((cid%10)/10.0);
            double t = ((cid/10)/10.0) + 0.004;
            double ns = s + texCharWidth;
            double nt = t + texCharHeight;

            // Because normaly image origin is at top left and openGL
            // puts it at bottom left, t texture coordinate must inversed
            texCoords.push_back(glm::dvec2(s,  1-t));
            texCoords.push_back(glm::dvec2(s,  1-nt));
            texCoords.push_back(glm::dvec2(ns, 1-nt));
            texCoords.push_back(glm::dvec2(ns, 1-nt));
            texCoords.push_back(glm::dvec2(ns, 1-t));
            texCoords.push_back(glm::dvec2(s,  1-t));


            // Positions
            double charWidth = (texCharWidth / texCharHeight) * top;
            right = left + charWidth;

            positions.push_back(glm::dvec2(left,  top));
            positions.push_back(glm::dvec2(left,  bottom));
            positions.push_back(glm::dvec2(right, bottom));
            positions.push_back(glm::dvec2(right, bottom));
            positions.push_back(glm::dvec2(right, top));
            positions.push_back(glm::dvec2(left,  top));

            // Move forward to next character's left
            left = right;
        }


        int posSize = sizeof(positions.front()) * positions.size();
        glBindBuffer(GL_ARRAY_BUFFER, _textHudVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER, posSize, positions.data(), GL_DYNAMIC_DRAW);

        int texSize = sizeof(texCoords.front()) * texCoords.size();
        glBindBuffer(GL_ARRAY_BUFFER, _textHudVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER, texSize, texCoords.data(), GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, font.textureId());

        glm::dvec2 anchorPos = text->handlePosition() +
            getAnchor(text->horizontalAnchor(), text->verticalAnchor());
        _textHudShader.setVec2f("Anchor", anchorPos);
        _textHudShader.setVec4f("ColorFilter", text->color());

        glDrawArrays(GL_TRIANGLES, 0, (int)positions.size());

        _imageHudVao.unbind();
        _imageHudShader.popProgram();
    }

    void GlArtDirector::drawImageHud(const std::shared_ptr<ImageHud>& image)
    {
        _imageHudShader.pushProgram();
        _imageHudVao.bind();

        const auto& cornerPos = image->cornersPositions();
        int posSize = sizeof(cornerPos.front()) * cornerPos.size();
        glBindBuffer(GL_ARRAY_BUFFER, _imageHudVao.bufferId("position"));
        glBufferData(GL_ARRAY_BUFFER, posSize, cornerPos.data(), GL_DYNAMIC_DRAW);

        const auto& cornerTex = image->cornersTexCoords();
        int texSize = sizeof(cornerTex.front()) * cornerTex.size();
        glBindBuffer(GL_ARRAY_BUFFER, _imageHudVao.bufferId("texCoord"));
        glBufferData(GL_ARRAY_BUFFER, texSize, cornerTex.data(), GL_DYNAMIC_DRAW);

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
        _circleShader.setInAndOutLocations(loc);
        _circleShader.addShader(GL_VERTEX_SHADER, ":/shaders/ShapeVertex.vert");
        _circleShader.addShader(GL_FRAGMENT_SHADER, ":/shaders/CircleFragment.frag");
        _circleShader.link();
        _circleShader.pushProgram();
        _circleShader.setInt("Texture", 0);
        _circleShader.popProgram();

        GlVbo2Dd positionBuff;
        positionBuff.attribLocation = _circleShader.getAttributeLocation("position");
		positionBuff.dataArray.push_back(glm::dvec2(-1.0f, -1.0f));
        positionBuff.dataArray.push_back(glm::dvec2( 1.0f, -1.0f));
        positionBuff.dataArray.push_back(glm::dvec2( 1.0f,  1.0f));
		positionBuff.dataArray.push_back(glm::dvec2(-1.0f,  1.0f));
        _circleVao.createBuffer("position", positionBuff);

        GlVbo2Dd texCoordBuff;
        texCoordBuff.attribLocation = _circleShader.getAttributeLocation("texCoord");
        texCoordBuff.dataArray.push_back(glm::dvec2(-0.5f, -0.5f));
        texCoordBuff.dataArray.push_back(glm::dvec2( 0.5f, -0.5f));
        texCoordBuff.dataArray.push_back(glm::dvec2( 0.5f,  0.5f));
        texCoordBuff.dataArray.push_back(glm::dvec2(-0.5f,  0.5f));
        _circleVao.createBuffer("texCoord", texCoordBuff);
    }

    void GlArtDirector::setupPolygonShader(const GlInputsOutputs& loc)
    {
        _polygonShader.setInAndOutLocations(loc);
        _polygonShader.addShader(GL_VERTEX_SHADER, ":/shaders/ShapeVertex.vert");
        _polygonShader.addShader(GL_FRAGMENT_SHADER, ":/shaders/PolygonFragment.frag");
        _polygonShader.link();
        _polygonShader.pushProgram();
        _polygonShader.setInt("Texture", 0);
        _polygonShader.popProgram();

        GlVbo2Dd positionBuff;
        positionBuff.attribLocation = _polygonShader.getAttributeLocation("position");
        _polygonVao.createBuffer("position", positionBuff);

        GlVbo2Dd texCoordBuff;
        texCoordBuff.attribLocation = _polygonShader.getAttributeLocation("texCoord");
        _polygonVao.createBuffer("texCoord", texCoordBuff);

        GlVbo4Dd colorBuff;
        colorBuff.attribLocation = _polygonShader.getAttributeLocation("color");
        _polygonVao.createBuffer("color", colorBuff);
    }

    void GlArtDirector::setupTextHudShader(const GlInputsOutputs& loc)
    {
        _textHudShader.setInAndOutLocations(loc);
        _textHudShader.addShader(GL_VERTEX_SHADER, ":/shaders/HudVertex.vert");
        _textHudShader.addShader(GL_FRAGMENT_SHADER, ":/shaders/HudFragment.frag");
        _textHudShader.link();
        _textHudShader.pushProgram();
        _textHudShader.setInt("Texture", 0);
        _textHudShader.popProgram();

        GlVbo2Dd positionBuff;
        positionBuff.attribLocation = _textHudShader.getAttributeLocation("position");
        _textHudVao.createBuffer("position", positionBuff);

        GlVbo2Dd texCoordBuff;
        texCoordBuff.attribLocation = _textHudShader.getAttributeLocation("texCoord");
        _textHudVao.createBuffer("texCoord", texCoordBuff);
    }

    void GlArtDirector::setupImageHudShader(const GlInputsOutputs& loc)
    {
        _imageHudShader.setInAndOutLocations(loc);
        _imageHudShader.addShader(GL_VERTEX_SHADER, ":/shaders/HudVertex.vert");
        _imageHudShader.addShader(GL_FRAGMENT_SHADER, ":/shaders/HudFragment.frag");
        _imageHudShader.link();
        _imageHudShader.pushProgram();
        _imageHudShader.setInt("Texture", 0);
        _imageHudShader.popProgram();

        GlVbo2Dd positionBuff;
        positionBuff.attribLocation = _imageHudShader.getAttributeLocation("position");
        _imageHudVao.createBuffer("position", positionBuff);

        GlVbo2Dd texCoordBuff;
        texCoordBuff.attribLocation = _imageHudShader.getAttributeLocation("texCoord");
        _imageHudVao.createBuffer("texCoord", texCoordBuff);
    }

    glm::dvec2 GlArtDirector::getAnchor(const EHorizontalAnchor& h,
                                   const EVerticalAnchor&   v)
    {
        return glm::dvec2(h == EHorizontalAnchor::RIGHT ? camera()->viewport().x : 0,
                          v == EVerticalAnchor::TOP     ? camera()->viewport().y : 0);
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
        string fileFullName = ":/fonts/" + _name;

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
        glBindTexture(GL_TEXTURE_2D, _id);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);


        //Load font format
        _charsHeight = 0.1f;
        for(int i=0; i<96; i++)
                _charsWidth[i] = 0.05f;
    }
}
