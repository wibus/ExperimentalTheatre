#include "Backdrop.h"


namespace prop3
{
    Backdrop::Backdrop(bool isDirectlyVisible) :
        _isDirectlyVisible(isDirectlyVisible)
    {

    }

    Backdrop::~Backdrop()
    {

    }

    void Backdrop::setIsDirectlyVisible(bool isDirectlyVisible)
    {
        _isDirectlyVisible = isDirectlyVisible;

        stampCurrentUpdate();
    }
}
