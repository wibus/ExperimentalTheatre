#include "Interface.h"


namespace prop2
{
    Interface::Interface(
            const std::shared_ptr<AbstractShape>& inShape,
            const std::shared_ptr<AbstractShape>& outShape) :
        inShape(inShape),
        outShape(outShape)
    {

    }

    Interface::~Interface()
    {

    }
}
