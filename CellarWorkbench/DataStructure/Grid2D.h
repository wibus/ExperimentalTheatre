#ifndef CELLARWORKBENCH_GRID2D_H
#define CELLARWORKBENCH_GRID2D_H

#include <cassert>

#include <GLM/glm.hpp>


namespace cellar
{

/// A dynamic 2D rectangular array.
/// For 2D grids of pointers with automatic memory deallocation, see PGrid2D.
template <typename T>
class Grid2D
{
public:
    /// Default constructor
    Grid2D();

    /// Copy constructor
    /// \param[in] grid Grid to copy
    Grid2D(const Grid2D<T>& grid);

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    Grid2D(int width, int height);

    /// Constructor
    /// \param[in] width Number of columns in the grid
    /// \param[in] height Number of rows in the grid
    /// \param[in] defaulValue Default value for every element
    Grid2D(int width, int height, const T& defaulValue);

    /// Destructor
    virtual ~Grid2D();

    /// Getter for the width of the grid
    /// \return Grid width
    int getWidth() const;

    /// Getter for the height of the grid
    /// \return Grid height
    int getHeight() const;

    /// Assignement operator overloaded to copy the content of the grid
    /// \param[in] grid Grid to be copied
    /// \return A reference to this object
    Grid2D<T>& operator =(const Grid2D<T>& grid);

    /// Subscript operator overloaded to get a line of the grid
    /// \param[in] y Line index
    /// \return A pointer to the first element of the line
    /// \note For access with grid initialization and bound checks, use get(x, y)
    T* operator [](int y);

    /// Subscript operator overloaded to get a line of the grid
    /// \param[in] y Line index
    /// \return A pointer to constant values to the first element of the line
    /// \note For access with grid initialization and bound checks, use get(x, y)
    const T* operator [](int y) const;

    /// Element access with grid initialization and bound checks
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \return Reference to the specified element
    T& get(int x, int y);

    /// Element access with grid initialization and bound checks
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \return Const reference to the specified element
    const T& get(int x, int y) const;

    /// Subscript operator overloaded to get an element of the grid
    /// \param[in] pos Row and Column of the element
    /// \return Const reference to the specified element
    /// \note For access with grid initialization and bound checks, use get(pos)
    T& operator [](const glm::ivec2& pos);

    /// Subscript operator overloaded to get an element of the grid
    /// \param[in] pos Row and Column of the element
    /// \return Const reference to the specified element
    /// \note For access with grid initialization and bound checks, use get(pos)
    const T& operator [](const glm::ivec2& pos) const;

    /// Element access with grid initialization and bound checks
    /// \param[in] pos Row and Column of the element
    /// \return Reference to the specified element
    T& get(const glm::ivec2& pos);

    /// Element access with grid initialization and bound checks (const version)
    /// \param[in] pos Row and Column of the element
    /// \return Const reference to the specified element
    const T& get(const glm::ivec2& pos) const;

    /// Modification of an element
    /// \param[in] x Column index of the element
    /// \param[in] y Row index of the element
    /// \param[in] value New value of the element
    /// \note Equivalent to "grid[y][x] = value;" and "grid.get(x, y) = value;"
    void set(int x, int y, const T& value);

    /// Modification of an element
    /// \param[in] pos Row and Column of the element
    /// \param[in] value New value of the element
    /// \note Equivalent to "grid.get(pos) = value;"
    void set(const glm::ivec2& pos, const T& value);


protected:
    T* _grid;
    int _width;
    int _height;
};



// IMPLEMENTATION //
template <typename T>
Grid2D<T>::Grid2D() :
    _grid(nullptr),
    _width(0),
    _height(0)
{
}

template <typename T>
Grid2D<T>::Grid2D(const Grid2D<T>& grid) :
    _grid(new T[grid.getWidth() * grid.getHeight()]),
    _width(grid._width),
    _height(grid._height)
{
    for(int j=0; j<_height; ++j)
        for(int i=0; i<_width; ++i)
            set(i, j, grid.get(i, j));
}

template <typename T>
Grid2D<T>::Grid2D(int width, int height) :
    _grid(new T[width * height]),
    _width(width),
    _height(height)
{

}

template <typename T>
Grid2D<T>::Grid2D(int width, int height, const T& defaulValue) :
    _grid(new T[width * height]),
    _width(width),
    _height(height)
{
    for(int j=0; j<_height; ++j)
        for(int i=0; i<_width; ++i)
            set(i, j, defaulValue);
}

template <typename T>
Grid2D<T>::~Grid2D()
{
    delete [] _grid;
    _grid = nullptr;
}

template <typename T>
inline int Grid2D<T>::getWidth() const
{
    return _width;
}

template <typename T>
inline int Grid2D<T>::getHeight() const
{
    return _height;
}

template <typename T>
Grid2D<T>& Grid2D<T>::operator =(const Grid2D<T>& grid)
{
    if(this != &grid)
    {
        if(_width != grid.getWidth() ||
           _height != grid.getHeight())
        {
            this->~Grid2D();
            _width = grid.getWidth();
            _height = grid.getHeight();
            _grid = new T[_width * _height];
        }

        for(int j=0; j<_height; ++j)
            for(int i=0; i<_width; ++i)
                set(i, j, grid.get(i, j));
    }

    return *this;
}

template <typename T>
inline T* Grid2D<T>::operator [](int y)
{
    return const_cast<T*>(const_cast< const Grid2D<T>& >(*this)[y]);
}

template <typename T>
inline const T* Grid2D<T>::operator [](int y) const
{
    return &_grid[y*_width];
}

template <typename T>
inline T& Grid2D<T>::get(int x, int y)
{
    return const_cast<T&>(const_cast< const Grid2D<T>& >(*this).get(x, y));
}

template <typename T>
const T& Grid2D<T>::get(int x, int y) const
{
    assert(_grid);
    assert(0 <= x && x < _width);
    assert(0 <= y && y < _height);
    return (*this)[y][x];
}

template <typename T>
inline T& Grid2D<T>::operator [](const glm::ivec2& pos)
{
    return _grid[pos.y][pos.x];
}

template <typename T>
inline const T& Grid2D<T>::operator [](const glm::ivec2& pos) const
{
    return _grid[pos.y][pos.x];
}

template <typename T>
inline T& Grid2D<T>::get(const glm::ivec2 &pos)
{
    return get(pos.x, pos.y);
}

template <typename T>
inline const T& Grid2D<T>::get(const glm::ivec2 &pos) const
{
    return get(pos.x, pos.y);
}

template <typename T>
inline void Grid2D<T>::set(int x, int y, const T& value)
{
    (*this).get(x, y) = value;
}

template <typename T>
inline void Grid2D<T>::set(const glm::ivec2 &pos, const T &value)
{
    set(pos.x, pos.y, value);
}
}

#endif // CELLARWORKBENCH_GRID2D_H
