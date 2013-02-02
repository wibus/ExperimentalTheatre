#ifndef CELLARWORKBENCH_PGRID_2D_H
#define CELLARWORKBENCH_PGRID_2D_H

#include "Grid2D.h"


namespace cellar
{
    template <typename T>
    class PGrid2D : public Grid2D<T*>
    {
    public:
        PGrid2D();
        PGrid2D(int width, int height);
        virtual ~PGrid2D();

    private:
        PGrid2D(const PGrid2D<T>& grid);
        PGrid2D<T>& operator =(const PGrid2D<T>& grid);
    };



    // IMPLEMENTATION //
    template <typename T>
    PGrid2D<T>::PGrid2D() :
        Grid2D<T*>()
    {
    }

    template <typename T>
    PGrid2D<T>::PGrid2D(int width, int height) :
        Grid2D<T*>(width, height, nullptr)
    {
    }

    template <typename T>
    PGrid2D<T>::~PGrid2D()
    {
        for(int j=0; j< this->_height; ++j)
            for(int i=0; i< this->_width; ++i)
                delete this->_grid[j][i];
    }
}

#endif // CELLARWORKBENCH_PGRID_2D_H
