#ifndef PROPROOM2D_INTERFACE_H
#define PROPROOM2D_INTERFACE_H

#include <PropRoom2D/Prop/Shape/AbstractShape.h>


namespace prop2
{
    class PROP2D_EXPORT Interface
    {
    public:
        Interface(const std::shared_ptr<AbstractShape>& inShape,
                  const std::shared_ptr<AbstractShape>& outShape = nullptr);
        ~Interface();

        std::shared_ptr<AbstractShape> inShape;
        std::shared_ptr<AbstractShape> outShape;


    private:
    };



    // IMPLEMENTATION //
}

#endif // PROPROOM2D_INTERFACE_H
