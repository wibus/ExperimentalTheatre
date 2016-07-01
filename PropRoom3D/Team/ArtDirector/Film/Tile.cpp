#include "Tile.h"

#include "Film.h"


namespace prop3
{
    const glm::ivec2 Tile::END_PIXEL = glm::ivec2(-1,-1);


    TileIterator::TileIterator(
            Tile& tile,
            const glm::ivec2& startPos) :
        _tile(tile),
        _position(startPos)
    {
    }

    double TileIterator::sampleWeight() const
    {
        return _tile.pixelPriority(_position)
                / _tile.priorityThreshold();
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
            while(_tile.priorityThreshold() > _tile.pixelPriority(_position));
        }

        return *this;
    }

    Tile::Tile(Film& film,
               const glm::ivec2& minCorner,
               const glm::ivec2& maxCorner) :
        _film(film),
        _tileId(-1),
        _endIterator(*this, END_PIXEL),
        _minCorner(minCorner),
        _maxCorner(maxCorner),
        _startPix(_minCorner + glm::ivec2(-1, 0)),
        _tilePriority(1.0),
        _divergence(0.0)
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
        return ++TileIterator(*this, _startPix);
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
        _mutex.unlock();
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

    double Tile::priorityThreshold() const
    {
        return _film.priorityThreshold();
    }

    double Tile::sampleMultiplicity() const
    {
        return _film.sampleMultiplicity();
    }

    void Tile::setTilePriority(double priority)
    {
        _tilePriority = priority;
    }

    void Tile::setDivergence(double div)
    {
        _divergence = div;
    }
}
