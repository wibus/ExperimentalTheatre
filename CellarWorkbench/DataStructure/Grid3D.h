#ifndef CELLARWORKBENCH_GRID3D_H
#define CELLARWORKBENCH_GRID3D_H

#include "Grid2D.h"
#include "Vector3D.h"

namespace cellar
{
    template <typename T>
    class Grid3D
    {
    public:
        Grid3D();
        Grid3D(const Grid3D<T>& grid);
        Grid3D(int width, int depth, int height);
        Grid3D(int width, int depth, int height, const T& defaulVal);
        virtual ~Grid3D();

        int width() const;
        int depth() const;
        int height() const;

        Grid3D<T>& operator =(const Grid3D<T>& grid);
        Grid2D<T>&       operator [](int z);
        const Grid2D<T>& operator [](int z) const;
        T&       get(int x, int y, int z);
        const T& get(int x, int y, int z) const;
        T&       get(const Vector3D<int>& pos);
        const T& get(const Vector3D<int>& pos) const;
        void     set(int x, int y, int z, const T& value);
        void     set(const Vector3D<int>& pos, const T& value);


    protected:
        Grid2D<T>* _grids;
        int _width;
        int _depth;
        int _height;
    };



    // IMPLEMENTATION //
    template<typename T>
    Grid3D<T>::Grid3D() :
        _grids(0x0),
        _width(0),
        _depth(0),
        _height(0)
    {
    }

    template<typename T>
    Grid3D<T>::Grid3D(const Grid3D<T>& grid) :
        _grids(0x0),
        _width(grid.width()),
        _depth(grid.depth()),
        _height(grid.height())
    {
        _grids = new Grid2D<T>[_height];
        for(int z=0; z<_height; ++z)
            _grids[z] = grid._grids[z];
    }

    template<typename T>
    Grid3D<T>::Grid3D(int width, int depth, int height) :
        _grids(0x0),
        _width(width),
        _depth(depth),
        _height(height)
    {
        _grids = new Grid2D<T>[_height];
        for(int z=0; z<_height; ++z)
            _grids[z] = Grid2D<T>(width, depth);
    }

    template<typename T>
    Grid3D<T>::Grid3D(int width, int depth, int height, const T& defaulVal) :
        _grids(0x0),
        _width(width),
        _depth(depth),
        _height(height)
    {
        _grids = new Grid2D<T>[_height];
        for(int z=0; z<_height; ++z)
            _grids[z] = Grid2D<T>(width, depth, defaulVal);
    }

    template<typename T>
    Grid3D<T>::~Grid3D()
    {
        delete [] _grids;
    }


    template<typename T>
    inline int Grid3D<T>::width() const
    {
        return _width;
    }

    template<typename T>
    inline int Grid3D<T>::depth() const
    {
        return _depth;
    }

    template<typename T>
    inline int Grid3D<T>::height() const
    {
        return _height;
    }


    template<typename T>
    Grid3D<T>& Grid3D<T>::operator =(const Grid3D<T>& grid)
    {
        if(this != &grid)
        {
            delete [] _grids;

            _width = grid._width;
            _depth = grid._depth;
            _height = grid._height;

            _grids = new Grid2D<T>[_height];
            for(int z=0; z<_height; ++z)
                _grids[z] = grid._grids[z];
        }

        return *this;
    }

    template<typename T>
    Grid2D<T>& Grid3D<T>::operator [](int z)
    {
        return _grids[z];
    }

    template<typename T>
    const Grid2D<T>& Grid3D<T>::operator [](int z) const
    {
        return _grids[z];
    }

    template<typename T>
    T& Grid3D<T>::get(int x, int y, int z)
    {
        return _grids[z][y][x];
    }

    template<typename T>
    const T& Grid3D<T>::get(int x, int y, int z) const
    {
        return _grids[z][y][x];
    }

    template<typename T>
    T& Grid3D<T>::get(const Vector3D<int>& pos)
    {
        return _grids[pos.z()][pos.x()][pos.y()];
    }

    template<typename T>
    const T& Grid3D<T>::get(const Vector3D<int>& pos) const
    {
        return _grids[pos.z()][pos.x()][pos.y()];
    }

    template<typename T>
    void Grid3D<T>::set(int x, int y, int z, const T& value)
    {
        _grids[z][y][x] = value;
    }

    template<typename T>
    void Grid3D<T>::set(const Vector3D<int>& pos, const T& value)
    {
        _grids[pos.z()][pos.x()][pos.y()] = value;
    }
}

#endif // CELLARWORKBENCH_GRID3D_H
