#include "Film.h"

#include <algorithm>


namespace prop3
{
    Film::Film() :
        _stateUid(-1),
        _framePassCount(0),
        _frameResolution(1, 1),
        _colorBuffer(1, glm::dvec3(0.0)),
        _colorOutput(ColorOutput::ALBEDO),
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

    void Film::setStateUid(int uid)
    {
        _stateUid = uid;
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

    std::shared_ptr<Tile> Film::getTile(size_t id)
    {
        if(id < _tiles.size())
        {
            return _tiles[id];
        }
        else
        {
            return std::shared_ptr<Tile>();
        }
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

    void Film::waitForFrameCompletion()
    {
        std::unique_lock<std::mutex> lk(_cvMutex);
        _cv.wait(lk, [&](){ return _newFrameCompleted; });
    }

    bool Film::needNewTiles() const
    {
        return true;
    }

    bool Film::incomingTileAvailable() const
    {
        return false;
    }

    std::shared_ptr<TileMessage> Film::nextIncomingTile()
    {
        return std::shared_ptr<TileMessage>();
    }

    std::shared_ptr<TileMessage> Film::nextOutgoingTile()
    {
        return std::shared_ptr<TileMessage>();
    }

    std::shared_ptr<Tile> Film::endTile()
    {
        return _endTile;
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

        // Put center tiles in at the begining to start with them
        glm::dvec2 target = glm::dvec2(_frameResolution) / 2.0 + glm::dvec2(0.5, 0.3);
        std::sort(_tiles.begin(), _tiles.end(), [target]
            (const std::shared_ptr<Tile>& t1, const std::shared_ptr<Tile>& t2){
                return glm::distance(glm::dvec2(t1->minCorner()), target) <
                       glm::distance(glm::dvec2(t2->minCorner()), target);
        });

        for(size_t i=0; i < tileCount; ++i)
            _tiles[i]->setTileId(i);

        _nextTileId = 0;
    }
}
