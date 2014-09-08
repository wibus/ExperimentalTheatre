#ifndef CELLARWORKBENCH_PGRID3D_H
#define CELLARWORKBENCH_PGRID3D_H

#include "Grid3D.h"


namespace cellar
{

/// \see Grid3D
/// This implementation of Grid3D take care of deallocating the pointers once
/// the grid is destructed.
template <typename T>
class PGrid3D : public Grid3D<T*>
{
public:
    /// Default constructor
    PGrid3D();

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    /// \param[in] depth Number of levels in the grid
    PGrid3D(int width, int height, int depth);

    /// Destructor
    /// \note Deallocates the elements that are not null
    virtual ~PGrid3D();

private:
    /// Deleted copy constructor
    PGrid3D(const PGrid3D<T>& grid) = delete;

    /// Deleted assignment operator
    PGrid3D<T>& operator =(const PGrid3D<T>& grid) = delete;
};



// IMPLEMENTATION //
template <typename T>
PGrid3D<T>::PGrid3D() :
    Grid3D<T*>()
{
}

template <typename T>
PGrid3D<T>::PGrid3D(int width, int height, int depth) :
    Grid3D<T*>(width, height, depth, nullptr)
{
}

template <typename T>
PGrid3D<T>::~PGrid3D()
{
    for(int k=0; k < this->_height; ++k)
        for(int j=0; j < this->_depth; ++j)
            for(int i=0; i < this->_width; ++i)
                delete this->get(i, j, k);
}

}

#endif // CELLARWORKBENCH_PGRID3D_H
