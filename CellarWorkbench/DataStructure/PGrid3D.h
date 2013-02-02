#ifndef CELLARWORKBENCH_PGRID3D_H
#define CELLARWORKBENCH_PGRID3D_H

#include "Grid3D.h"


namespace cellar
{
    template <typename T>
    class PGrid3D : public Grid3D<T*>
    {
    public:
        PGrid3D();
        PGrid3D(int width, int depth, int height);
        virtual ~PGrid3D();

    private:
        PGrid3D(const PGrid3D<T>& grid);
        PGrid3D<T>& operator =(const PGrid3D<T>& grid);
    };



    // IMPLEMENTATION //
    template <typename T>
    PGrid3D<T>::PGrid3D() :
        Grid3D<T*>()
    {
    }

    template <typename T>
    PGrid3D<T>::PGrid3D(int width, int depth, int height) :
        Grid3D<T*>(width, depth, height, nullptr)
    {
    }

    template <typename T>
    PGrid3D<T>::~PGrid3D()
    {
        for(int k=0; k < this->_height; ++k)
            for(int j=0; j < this->_depth; ++j)
                for(int i=0; i < this->_width; ++i)
                    delete this->_grids[k][j][i];
    }
}

#endif // CELLARWORKBENCH_PGRID3D_H
