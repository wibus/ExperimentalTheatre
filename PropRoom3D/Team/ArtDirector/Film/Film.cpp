#include "Film.h"


namespace prop3
{
    Film::Film() :
        _framePassCount(0),
        _frameResolution(1, 1),
        _colorBuffer(1, glm::dvec3(0.0)),
        _newTileCompleted(false),
        _newFrameCompleted(false),
        _priorityThreshold(0.0),
        _tilesResolution(16, 16),
        _nextTileId(0),
        _endTile(nullptr)
    {

    }

    Film::~Film()
    {

    }

    void Film::resizeFrame(const glm::ivec2& resolution)
    {
        if(_frameResolution != resolution)
        {
            _frameResolution = resolution;

            buildTiles();
            clear(glm::dvec3(0.0), true);
        }
    }

    void Film::resizeTiles(const glm::ivec2& resolution)
    {
        if(_tilesResolution != resolution)
        {
            _tilesResolution = resolution;

            buildTiles();
            clear(glm::dvec3(0.0), false);
        }
    }

    bool Film::newTileCompleted()
    {
        bool is = _newTileCompleted;
        _newTileCompleted = false;
        return is;
    }

    bool Film::newFrameCompleted()
    {
        bool is = _newFrameCompleted;
        _newFrameCompleted = false;
        return is;
    }

    void Film::mergeFilm(const Film& film)
    {
        size_t tileId = _tiles.size();
        while(tileId > 0)
        {
            std::shared_ptr<Tile> tile = _tiles[--tileId];

            tile->lock();

            const glm::ivec2 minCorner = tile->minCorner();
            const glm::ivec2 maxCorner = tile->maxCorner();
            for(int j=minCorner.y; j < maxCorner.y; ++j)
            {
                for(int i=minCorner.x; i < maxCorner.x; ++i)
                {
                    int index = j*frameWidth() + i;
                    addSample(index, film.pixelSample(index));
                }
            }

            tile->unlock();
        }

        _newTileCompleted = true;
        _newFrameCompleted = true;
    }

    std::shared_ptr<Tile> Film::nextTile()
    {
        std::lock_guard<std::mutex> lk(_tilesMutex);

        while(_nextTileId < _tiles.size())
        {
            std::shared_ptr<Tile> tile = _tiles[_nextTileId++];

            if(_nextTileId >= _tiles.size())
            {
                ++_framePassCount;
                endTileReached();
            }

            if(tile->tilePriority() >= _priorityThreshold)
            {
                tile->setPriorityThreshold(_priorityThreshold);
                return tile;
            }
        }

        return endTile();
    }

    std::shared_ptr<Tile> Film::endTile()
    {
        return _endTile;
    }

    void Film::waitFrameCompletion()
    {
        std::unique_lock<std::mutex> lk(_cvMutex);
        _cv.wait(lk, [&](){ return _newFrameCompleted; });
    }

    void Film::buildTiles()
    {
        std::lock_guard<std::mutex> lk(_tilesMutex);

        size_t columnCount = glm::ceil(frameWidth() / double(tilesWidth()));
        size_t rowCount = glm::ceil(frameHeight() / double(tilesHeight()));
        size_t tileCount = columnCount * rowCount;

        _tiles.clear();
        _tiles.reserve(tileCount);
        for(int j=0; j < rowCount; ++j)
        {
            for(int i=0; i < columnCount; ++i)
            {
                glm::ivec2 minCorner(i*tilesWidth(), j*tilesHeight());
                glm::ivec2 maxCorner(
                    glm::min((i+1)*tilesWidth(), frameWidth()),
                    glm::min((j+1)*tilesHeight(), frameHeight()));

                _tiles.push_back(std::make_shared<Tile>(
                    *this, _priorityThreshold, minCorner, maxCorner));
            }
        }

        _nextTileId = 0;
    }
}
