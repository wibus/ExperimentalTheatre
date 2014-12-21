#include "StdPropDesigner.h"

#include <cassert>

#include "../Shape/Circle.h"
#include "../Shape/Polygon.h"
#include "../Costume/CircleCostume.h"
#include "../Costume/PolygonCostume.h"
#include "../Hardware/Hardware.h"
#include "../Hud/TextHud.h"
#include "../Hud/ImageHud.h"


namespace prop2
{
    StdPropDesigner::StdPropDesigner()
    {
    }

    StdPropDesigner::~StdPropDesigner()
    {
    }

    void StdPropDesigner::setup()
    {
    }

    void StdPropDesigner::reset()
    {
    }

    std::shared_ptr<Circle> StdPropDesigner::createCircle()
    {
        std::shared_ptr<Circle> circle( new Circle() );
        circle->setCostume(
            std::shared_ptr<CircleCostume>(new CircleCostume()));
        circle->setHardware(
            std::shared_ptr<Hardware>(new Hardware()));

        return circle;
    }

    std::shared_ptr<Polygon> StdPropDesigner::createPolygon(int nbVertices)
    {
        assert(3 <= nbVertices);
        std::shared_ptr<Polygon> polygon( new Polygon(nbVertices) );
        polygon->setCostume(
            std::shared_ptr<PolygonCostume>(new PolygonCostume(nbVertices)));
        polygon->setHardware(
            std::shared_ptr<Hardware>(new Hardware()));

        return polygon;
    }

    std::shared_ptr<TextHud> StdPropDesigner::createTextHud()
    {
        return std::shared_ptr<TextHud>( new TextHud() );
    }

    std::shared_ptr<ImageHud> StdPropDesigner::createImageHud()
    {
        return std::shared_ptr<ImageHud>( new ImageHud() );
    }
}
