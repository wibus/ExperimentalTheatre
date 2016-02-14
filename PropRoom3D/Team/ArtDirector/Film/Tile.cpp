#include "Tile.h"

#include "Film.h"


namespace prop3
{
    const glm::ivec2 Tile::END_PIXEL = glm::ivec2(-1,-1);


    Tile::Iterator::Iterator(
            Tile& tile,
            double threshold,
            const glm::ivec2& startPos) :
        _tile(tile),
        _threshold(threshold),
        _position(startPos)
    {
    }

    Tile::Iterator& Tile::Iterator::operator++()
    {
        if(_position != END_PIXEL)
        {
            do
            {
                if(++_position.x >= _tile.maxCorner().x)
                {
                    _position.x = _tile.minCorner().x;

                    if(++_position.y >= _tile.maxCorner().y)
                    {
                        _position = END_PIXEL;
                        return *this;
                    }
                }
            }
            while(_threshold > _tile.pixelPriority(_position));
        }

        return *this;
    }

    Tile::Tile(Film& film,
               double threshold,
               const glm::ivec2& minCorner,
               const glm::ivec2& maxCorner) :
        _film(film),
        _endIterator(*this, 0.0, END_PIXEL),
        _minCorner(minCorner),
        _maxCorner(maxCorner),
        _startPix(_minCorner + glm::ivec2(-1, 0)),
        _tilePriority(1.0),
        _priorityThreshold(threshold),
        _divergenceSum(0.0)
    {
    }

    Tile::~Tile()
    {

    }

    Tile::Iterator Tile::begin()
    {
        return ++Iterator(*this, _priorityThreshold, _startPix);
    }

    Tile::Iterator Tile::end()
    {
        return _endIterator;
    }

    void Tile::lock()
    {
        _mutex.lock();
    }

    void Tile::unlock()
    {
        _film.tileCompleted(*this);
        _mutex.unlock();
    }

    void Tile::setTilePriority(double priority)
    {
        _tilePriority = priority;
    }

    void Tile::setPriorityThreshold(double threshold)
    {
        _priorityThreshold = threshold;
    }

    void Tile::setColor(
            const glm::ivec2& position,
            const glm::dvec3& color)
    {
        _film.setColor(position, color);
    }

    void Tile::addSample(
            const glm::ivec2& position,
            const glm::dvec4& sample)
    {
        _film.addSample(position, sample);
    }

    double Tile::pixelPriority(const glm::ivec2& position) const
    {
        return _film.pixelPriority(position);
    }

    void Tile::setDivergenceSum(double sum)
    {
        _divergenceSum = sum;
    }
}
