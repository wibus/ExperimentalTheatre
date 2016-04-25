#include "Tile.h"

#include "Film.h"


namespace prop3
{
    const glm::ivec2 Tile::END_PIXEL = glm::ivec2(-1,-1);


    TileIterator::TileIterator(
            Tile& tile,
            double threshold,
            const glm::ivec2& startPos) :
        _tile(tile),
        _threshold(threshold),
        _position(startPos)
    {
    }

    double TileIterator::resquestedSampleWeight() const
    {
        double baseWeight = _tile.pixelPriority(_position) / _threshold;
        return baseWeight * baseWeight;
    }

    TileIterator& TileIterator::operator++()
    {
        if(_position != Tile::END_PIXEL)
        {
            do
            {
                if(++_position.x >= _tile.maxCorner().x)
                {
                    _position.x = _tile.minCorner().x;

                    if(++_position.y >= _tile.maxCorner().y)
                    {
                        _position = Tile::END_PIXEL;
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
        _tileId(-1),
        _endIterator(*this, 0.0, END_PIXEL),
        _minCorner(minCorner),
        _maxCorner(maxCorner),
        _startPix(_minCorner + glm::ivec2(-1, 0)),
        _tilePriority(1.0),
        _priorityThreshold(threshold),
        _divergenceSum(0.0)
    {
        glm::ivec2 tileDim = _maxCorner - _minCorner;
        _pixelCount = tileDim.x * + tileDim.y;
    }

    Tile::~Tile()
    {

    }

    void Tile::setTileId(size_t id)
    {
        _tileId = id;
    }

    TileIterator Tile::begin()
    {
        return ++TileIterator(*this, _priorityThreshold, _startPix);
    }

    TileIterator Tile::end()
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

    glm::dvec4 Tile::pixelSample(int i, int j) const
    {
        return _film.pixelSample(i, j);
    }

    void Tile::addSample(
            int i, int j,
            const glm::dvec4& sample)
    {
        _film.addSample(i, j, sample);
    }

    void Tile::addSample(
            const glm::ivec2& position,
            const glm::dvec4& sample)
    {
        addSample(position.x, position.y, sample);
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
