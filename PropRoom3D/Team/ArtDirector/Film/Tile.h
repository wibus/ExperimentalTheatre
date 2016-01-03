#ifndef PROPROOM3D_TILE_H
#define PROPROOM3D_TILE_H

#include <mutex>

#include <GLM/glm.hpp>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Film;


    class PROP3D_EXPORT Tile
    {
    public:
        class PROP3D_EXPORT Iterator
        {
        public:
            Iterator(Tile& tile,
                     double threshold,
                     const glm::ivec2& startPos);

            glm::ivec2 position() const;

            Iterator& operator++();

            bool operator==(const Iterator& it) const;
            bool operator!=(const Iterator& it) const;

            void addSample(const glm::dvec4& sample);

        private:
            Tile& _tile;
            double _threshold;
            glm::ivec2 _position;
        };



    public:
        Tile(Film& film,
             double threshold,
             const glm::ivec2& minCorner,
             const glm::ivec2& maxCorner);
        ~Tile();

        const glm::ivec2& minCorner() const;
        const glm::ivec2& maxCorner() const;

        Iterator begin();
        Iterator end();

        void lock();
        void unlock();

        void setColor(const glm::ivec2& position,
                      const glm::dvec3& color);
        void addSample(const glm::ivec2& position,
                       const glm::dvec4& sample);

        void setTilePriority(double priority);
        void setPriorityThreshold(double threshold);

        double tilePriority() const;
        double pixelPriority(const glm::ivec2& position) const;

    protected:
        static const glm::ivec2 END_PIXEL;

        Film& _film;
        std::mutex _mutex;
        Iterator _endIterator;
        double _tilePriority;
        double _priorityThreshold;
        const glm::ivec2 _minCorner;
        const glm::ivec2 _maxCorner;
        const glm::ivec2 _startPix;
    };



    // IMPLEMENTATION //
    inline glm::ivec2 Tile::Iterator::position() const
    {
        return _position;
    }

    inline bool Tile::Iterator::operator==(const Iterator& it) const
    {
        return _position == it._position;
    }

    inline bool Tile::Iterator::operator!=(const Iterator& it) const
    {
        return _position != it._position;
    }

    inline void Tile::Iterator::addSample(const glm::dvec4& sample)
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

    inline double Tile::tilePriority() const
    {
        return _tilePriority;
    }
}

#endif // PROPROOM3D_TILE_H
