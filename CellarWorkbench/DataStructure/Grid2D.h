#ifndef CELLARWORKBENCH_GRID_2D_H
#define CELLARWORKBENCH_GRID_2D_H

#include <cassert>
#include "libCellarWorkbench_global.h"
#include "Vector.h"

namespace cellar
{
    template <typename T>
    class Grid2D
    {
    public:
        Grid2D();
        Grid2D(const Grid2D<T>& grid);
        Grid2D(int width, int height);
        Grid2D(int width, int height, const T& defaulVal);
        virtual ~Grid2D();

        int width() const;
        int height() const;

        Grid2D<T>& operator =(const Grid2D<T>& grid);
        T*       operator [](int y);
        const T* operator [](int y) const;
        T&       get(int x, int y);
        const T& get(int x, int y) const;
        T&       get(const Vec2i& pos);
        const T& get(const Vec2i& pos) const;
        void     set(int x, int y, const T& value);
        void     set(const Vec2i& pos, const T& value);


    protected:
        T** _grid;
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
        _grid(nullptr),
        _width(grid._width),
        _height(grid._height)
    {
        _grid = new T*[_height];

        for(int j=0; j<_height; ++j)
        {
            _grid[j] = new T[_width];
            for(int i=0; i<_width; ++i)
                _grid[j][i] = grid.get(i, j);
        }
    }

    template <typename T>
    Grid2D<T>::Grid2D(int width, int height) :
        _grid(nullptr),
        _width(width),
        _height(height)
    {
        _grid = new T*[_height];

        for(int j=0; j<_height; ++j)
            _grid[j] = new T[_width];
    }

    template <typename T>
    Grid2D<T>::Grid2D(int width, int height, const T& defaultVal) :
        _grid(nullptr),
        _width(width),
        _height(height)
    {
        _grid = new T*[_height];

        for(int j=0; j<_height; ++j)
        {
            _grid[j] = new T[_width];
            for(int i=0; i<_width; ++i)
                _grid[j][i] = defaultVal;
        }
    }

    template <typename T>
    Grid2D<T>::~Grid2D()
    {
        for(int j=0; j<_height; ++j)
            delete [] _grid[j];

        delete [] _grid;
    }

    template <typename T>
    inline int Grid2D<T>::width() const
    {
        return _width;
    }

    template <typename T>
    inline int Grid2D<T>::height() const
    {
        return _height;
    }

    template <typename T>
    Grid2D<T>& Grid2D<T>::operator =(const Grid2D<T>& grid)
    {
        if(this != &grid)
        {
            for(int j=0; j<_height; ++j)
                delete [] _grid[j];
            delete [] _grid;

            _width = grid._width;
            _height = grid._height;

            _grid = new T*[_height];

            for(int j=0; j<_height; ++j)
            {
                _grid[j] = new T[_width];
                for(int i=0; i<_width; ++i)
                    _grid[j][i] = grid.get(i, j);
            }
        }

        return *this;
    }

    template <typename T>
    inline T* Grid2D<T>::operator [](int y)
    {
        assert(_grid);
        return _grid[y];
    }

    template <typename T>
    inline const T* Grid2D<T>::operator [](int y) const
    {
        assert(_grid);
        return _grid[y];
    }

    template <typename T>
    inline T& Grid2D<T>::get(int x, int y)
    {
        assert(_grid);
        return _grid[y][x];
    }

    template <typename T>
    inline const T& Grid2D<T>::get(int x, int y) const
    {
        assert( _grid);
        return _grid[y][x];
    }

    template <typename T>
    inline T& Grid2D<T>::get(const Vec2i &pos)
    {
        return get(pos.x(), pos.y());
    }

    template <typename T>
    inline const T& Grid2D<T>::get(const Vec2i &pos) const
    {
        return get(pos.x(), pos.y());
    }

    template <typename T>
    inline void Grid2D<T>::set(int x, int y, const T& value)
    {
        assert(_grid);
        _grid[y][x] = value;
    }

    template <typename T>
    inline void Grid2D<T>::set(const Vec2i &pos, const T &value)
    {
        set(pos.x(), pos.y(), value);
    }
}

#endif // CELLARWORKBENCH_GRID_2D_H
