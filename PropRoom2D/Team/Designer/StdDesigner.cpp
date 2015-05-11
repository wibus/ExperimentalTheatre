#include "StdDesigner.h"

#include <cassert>

#include "../../Prop/Shape/Circle.h"
#include "../../Prop/Shape/Polygon.h"
#include "../../Prop/Costume/CircleCostume.h"
#include "../../Prop/Costume/PolygonCostume.h"
#include "../../Prop/Hardware/Hardware.h"
#include "../../Prop/Hud/TextHud.h"
#include "../../Prop/Hud/ImageHud.h"


namespace prop2
{
    StdDesigner::StdDesigner()
    {
    }

    StdDesigner::~StdDesigner()
    {
    }

    void StdDesigner::setup()
    {
    }

    void StdDesigner::reset()
    {
    }

    std::shared_ptr<Circle> StdDesigner::createCircle()
    {
        std::shared_ptr<Circle> circle( new Circle() );
        circle->setCostume(
            std::shared_ptr<CircleCostume>(new CircleCostume()));
        circle->setHardware(
            std::shared_ptr<Hardware>(new Hardware()));

        return circle;
    }

    std::shared_ptr<Polygon> StdDesigner::createPolygon(int nbVertices)
    {
        assert(3 <= nbVertices);
        std::shared_ptr<Polygon> polygon( new Polygon(nbVertices) );
        polygon->setCostume(
            std::shared_ptr<PolygonCostume>(new PolygonCostume(nbVertices)));
        polygon->setHardware(
            std::shared_ptr<Hardware>(new Hardware()));

        return polygon;
    }

    std::shared_ptr<TextHud> StdDesigner::createTextHud()
    {
        return std::shared_ptr<TextHud>( new TextHud() );
    }

    std::shared_ptr<ImageHud> StdDesigner::createImageHud()
    {
        return std::shared_ptr<ImageHud>( new ImageHud() );
    }
}
