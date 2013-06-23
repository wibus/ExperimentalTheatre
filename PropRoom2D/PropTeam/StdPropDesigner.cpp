#include "StdPropDesigner.h"

#include "Shape/Circle.h"
#include "Costume/CircleCostume.h"
#include "Shape/Polygon.h"
#include "Costume/PolygonCostume.h"

#include "Hud/TextHud.h"
#include "Hud/ImageHud.h"


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
                    std::shared_ptr<CircleCostume>(
                        new CircleCostume()));

        return circle;
    }

    std::shared_ptr<Polygon> StdPropDesigner::createPolygon(int nbVertices)
    {
        std::shared_ptr<Polygon> polygon( new Polygon(nbVertices) );
        polygon->setCostume(
                    std::shared_ptr<PolygonCostume>(
                        new PolygonCostume(nbVertices)));

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
