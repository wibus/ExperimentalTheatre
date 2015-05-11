#ifndef CELLARWORKBENCH_PGRID2D_H
#define CELLARWORKBENCH_PGRID2D_H

#include "Grid2D.h"


namespace cellar
{

/// \see Grid2D
/// This implementation of Grid2D take care of deallocating the pointers once
/// the grid is destructed.
template <typename T>
class PGrid2D : public Grid2D<T*>
{
public:
    /// Default constructor
    PGrid2D();

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    /// \note Elements are initialized to nullptr
    PGrid2D(int width, int height);

    /// Destructor
    /// \note Deallocates the elements that are not null
    virtual ~PGrid2D();

private:
    /// Deleted copy constructor
    PGrid2D(const PGrid2D<T>& grid) = delete;

    /// Deleted assignment operator
    PGrid2D<T>& operator =(const PGrid2D<T>& grid) = delete;
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
    for(int j=0; j<this->_height; ++j)
        for(int i=0; i<this->_width; ++i)
            delete this->get(i, j);
}

}

#endif // CELLARWORKBENCH_PGRID2D_H
