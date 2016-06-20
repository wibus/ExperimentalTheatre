#ifndef PROPROOM3D_TILE_H
#define PROPROOM3D_TILE_H

#include <mutex>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Film;
    class Tile;

    class PROP3D_EXPORT TileIterator
    {
    public:
        TileIterator(Tile& tile,
            const glm::ivec2& startPos);

        glm::ivec2 position() const;
        double sampleWeight() const;
        double sampleMultiplicity() const;

        TileIterator& operator++();

        bool operator==(const TileIterator& it) const;
        bool operator!=(const TileIterator& it) const;

        void addSample(const glm::dvec4& sample);

    private:
        Tile& _tile;
        glm::ivec2 _position;
    };

    class PROP3D_EXPORT Tile
    {
    public:
        Tile(Film& film,
             const glm::ivec2& minCorner,
             const glm::ivec2& maxCorner);
        ~Tile();

        const glm::ivec2& minCorner() const;
        const glm::ivec2& maxCorner() const;
        unsigned int pixelCount() const;

        void setTileId(size_t id);
        size_t tileId() const;

        TileIterator begin();
        TileIterator end();

        void lock();
        void unlock();

        glm::dvec4 pixelSample(int i, int j) const;
        void addSample(int i, int j,
                       const glm::dvec4& sample);
        void addSample(const glm::ivec2& position,
                       const glm::dvec4& sample);

        double pixelPriority(const glm::ivec2& position) const;

        double priorityThreshold() const;

        double sampleMultiplicity() const;

        void setTilePriority(double priority);
        double tilePriority() const;

        void setDivergence(double div);
        double divergence() const;

        static const glm::ivec2 END_PIXEL;

    protected:
        Film& _film;
        size_t _tileId;
        std::mutex _mutex;
        TileIterator _endIterator;
        const glm::ivec2 _minCorner;
        const glm::ivec2 _maxCorner;
        const glm::ivec2 _startPix;
        unsigned int _pixelCount;
        double _tilePriority;
        double _divergence;
    };



    // IMPLEMENTATION //
    inline glm::ivec2 TileIterator::position() const
    {
        return _position;
    }

    inline double TileIterator::sampleMultiplicity() const
    {
        return _tile.sampleMultiplicity();
    }

    inline bool TileIterator::operator==(const TileIterator& it) const
    {
        return _position == it._position;
    }

    inline bool TileIterator::operator!=(const TileIterator& it) const
    {
        return _position != it._position;
    }

    inline void TileIterator::addSample(const glm::dvec4& sample)
    {
        _tile.addSample(_position, sample);
    }

    inline const glm::ivec2& Tile::minCorner() const
    {
        return _minCorner;
    }

    inline const glm::ivec2& Tile::maxCorner() const
    {
        return _maxCorner;
    }

    inline unsigned int Tile::pixelCount() const
    {
        return _pixelCount;
    }

    inline size_t Tile::tileId() const
    {
        return _tileId;
    }

    inline double Tile::tilePriority() const
    {
        return _tilePriority;
    }

    inline double Tile::divergence() const
    {
        return _divergence;
    }
}

#endif // PROPROOM3D_TILE_H
