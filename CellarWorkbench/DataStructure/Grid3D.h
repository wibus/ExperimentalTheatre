#ifndef CELLARWORKBENCH_GRID3D_H
#define CELLARWORKBENCH_GRID3D_H

#include "Grid2D.h"


namespace cellar
{

/// A dynamic 3D "rectangular" array.
/// See this structure as a stack of Grid2Ds.
/// For 3D grids of pointers with automatic memory deallocation, see PGrid3D.
template <typename T>
class Grid3D
{
public:
    /// Default constructor
    Grid3D();

    /// Copy constructor
    /// \param[in] grid Grid to copy
    Grid3D(const Grid3D<T>& grid);

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    /// \param[in] depth Number of levels in the grid
    Grid3D(int width, int height, int depth);

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    /// \param[in] depth Number of levels in the grid
    /// \param[in] defaulValue Default value for every element
    Grid3D(int width, int height, int depth, const T& defaulVal);

    /// Destructor
    virtual ~Grid3D();

    /// Getter for the width of the grid
    /// \return Grid width
    int getWidth() const;

    /// Getter for the height of the grid
    /// \return Grid height
    int getHeight() const;

    /// Getter for the depth of the grid (number of Grid2Ds in the stack)
    /// \return Grid depth
    int getDepth() const;

    /// Assignement operator overloaded to copy the content of the grid
    /// \param[in] grid Grid to be copied
    /// \return A reference to this object
    Grid3D<T>& operator =(const Grid3D<T>& grid);

    /// Subscript operator overloaded to get a level of the grid
    /// \param[in] z Level index
    /// \return A reference to the specified grid level
    /// \note For access with grid initialization and bound checks,
    ///       use get(x, y, z)
    Grid2D<T>& operator [](int z);

    /// Subscript operator overloaded to get a level of the grid
    /// \param[in] z Level index
    /// \return A const reference to the specified grid level
    /// \note For access with grid initialization and bound checks,
    ///       use get(x, y, z)
    const Grid2D<T>& operator [](int z) const;

    /// Element access with grid initialization and bound checks
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \param[in] z Level index of the element
    /// \return Reference to the specified element
    T& get(int x, int y, int z);

    /// Element access with grid initialization and bound checks
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \param[in] z Level index of the element
    /// \return Const reference to the specified element
    const T& get(int x, int y, int z) const;

    /// Subscript operator overloaded to get an element of the grid
    /// \param[in] pos Row, Column and Level of the element
    /// \return Const reference to the specified element
    /// \note For access with grid initialization and bound checks, use get(pos)
    T& operator [](const glm::ivec3& pos);

    /// Subscript operator overloaded to get an element of the grid
    /// \param[in] pos Row, Column and Level of the element
    /// \return Const reference to the specified element
    /// \note For access with grid initialization and bound checks, use get(pos)
    const T& operator [](const glm::ivec3& pos) const;

    /// Element access with grid initialization and bound checks
    /// \param[in] pos Row, Column and Level of the element
    /// \return Reference to the specified element
    T& get(const glm::ivec3& pos);

    /// Element access with grid initialization and bound checks
    /// \param[in] pos Row, Column and Level of the element
    /// \return Const reference to the specified element
    const T& get(const glm::ivec3& pos) const;

    /// Modification of an element
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \param[in] z Level index of the element
    /// \param[in] value New value of the element
    /// \note Equivalent to "grid[z][y][x] = value;"
    ///       and "grid.get(x, y, z) = value;"
    void set(int x, int y, int z, const T& value);

    /// Modification of an element
    /// \param[in] pos Row, Column and Level of the element
    /// \param[in] value New value of the element
    /// \note Equivalent to "grid.get(pos) = value;"
    void set(const glm::ivec3& pos, const T& value);


protected:
    Grid2D<T>* _grids;
    int _width;
    int _height;
    int _depth;
};



// IMPLEMENTATION //
template<typename T>
Grid3D<T>::Grid3D() :
    _grids(nullptr),
    _width(0),
    _height(0),
    _depth(0)
{
}

template<typename T>
Grid3D<T>::Grid3D(const Grid3D<T>& grid) :
    _grids(new Grid2D<T>[grid.getDepth()]),
    _width(grid.getWidth()),
    _height(grid.getHeight()),
    _depth(grid.getDepth())
{
    for(int z=0; z<_depth; ++z)
        _grids[z] = grid[z];
}

template<typename T>
Grid3D<T>::Grid3D(int width, int height, int depth) :
    _grids(new Grid2D<T>[depth]),
    _width(width),
    _height(height),
    _depth(depth)
{
    for(int z=0; z<_depth; ++z)
        _grids[z] = Grid2D<T>(width, height);
}

template<typename T>
Grid3D<T>::Grid3D(int width, int depth, int height, const T& defaulVal) :
    _grids(new Grid2D<T>[depth]),
    _width(width),
    _height(height),
    _depth(depth)
{
    for(int z=0; z<_depth; ++z)
        _grids[z] = Grid2D<T>(width, height, defaulVal);
}

template<typename T>
Grid3D<T>::~Grid3D()
{
    delete [] _grids;
}


template<typename T>
inline int Grid3D<T>::getWidth() const
{
    return _width;
}

template<typename T>
inline int Grid3D<T>::getHeight() const
{
    return _height;
}

template<typename T>
inline int Grid3D<T>::getDepth() const
{
    return _depth;
}

template<typename T>
Grid3D<T>& Grid3D<T>::operator =(const Grid3D<T>& grid)
{
    if(this != &grid)
    {
        if(_width != grid.getWidth()   ||
           _height != grid.getHeight() ||
           _depth != grid.getDepth())
        {
            this->~Grid3D();
            _width = grid.getWidth();
            _height = grid.getHeight();
            _depth = grid.getDepth();
            _grids = new T[_depth];
        }

        for(int z=0; z<_depth; ++z)
            _grids[z] = grid._grids[z];
    }

    return *this;
}

template<typename T>
inline Grid2D<T>& Grid3D<T>::operator [](int z)
{
    return const_cast<Grid2D<T>&>(const_cast< const Grid3D<T>& >(*this)[z]);
}

template<typename T>
inline const Grid2D<T>& Grid3D<T>::operator [](int z) const
{
    return _grids[z];
}

template<typename T>
inline T& Grid3D<T>::get(int x, int y, int z)
{
    return const_cast<T&>(const_cast< const Grid3D<T>& >(*this).get(x, y, z));
}

template<typename T>
const T& Grid3D<T>::get(int x, int y, int z) const
{
    assert(_grids);
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);
    assert(0 <= z && z < _depth);
    return _grids[z][y][x];
}

template <typename T>
inline T& Grid3D<T>::operator [](const glm::ivec3& pos)
{
    return _grids[pos.z][pos.y][pos.x];
}

template <typename T>
inline const T& Grid3D<T>::operator [](const glm::ivec3& pos) const
{
    return _grids[pos.z][pos.y][pos.x];
}

template<typename T>
inline T& Grid3D<T>::get(const glm::ivec3& pos)
{
    return const_cast<T&>(const_cast< const Grid3D<T>& >(*this).get(pos));
}

template<typename T>
inline const T& Grid3D<T>::get(const glm::ivec3& pos) const
{
    return get(pos.x, pos.y, pos.z);
}

template<typename T>
inline void Grid3D<T>::set(int x, int y, int z, const T& value)
{
    get(x, y, z) = value;
}

template<typename T>
inline void Grid3D<T>::set(const glm::ivec3& pos, const T& value)
{
    set(pos.x, pos.y, pos.z(), value);
}

}

#endif // CELLARWORKBENCH_GRID3D_H
