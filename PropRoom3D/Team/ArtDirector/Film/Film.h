#ifndef PROPROOM3D_FILM_H
#define PROPROOM3D_FILM_H

#include <vector>
#include <memory>
#include <condition_variable>

#include <GLM/glm.hpp>

#include "Tile.h"


namespace prop3
{
    class TileMessage;


    class PROP3D_EXPORT Film
    {
    public:
        enum class ColorOutput {ALBEDO, WEIGHT, DIVERGENCE, VARIANCE, PRIORITY, REFERENCE, COMPATIBILITY};

        Film();
        virtual ~Film();

        int stateUid() const;
        void setStateUid(int uid);

        int frameWidth() const;
        int frameHeight() const;
        glm::ivec2 frameResolution() const;
        void resizeFrame(int frameWidth, int frameHeight);
        virtual void resizeFrame(const glm::ivec2& resolution);

        size_t tileCount() const;
        int tilesWidth() const;
        int tilesHeight() const;
        glm::ivec2 tilesResolution() const;
        void resizeTiles(int tilesWidth, int tilesHeight);
        virtual void resizeTiles(const glm::ivec2& resolution);


        const std::vector<float>& depthBuffer() const;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) = 0;


        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) = 0;

        virtual void backupAsReferenceShot() = 0;

        virtual bool saveReferenceShot(const std::string& name) const = 0;
        virtual bool loadReferenceShot(const std::string& name) = 0;
        virtual bool clearReferenceShot() = 0;

        virtual bool saveRawFilm(const std::string& name) const = 0;
        virtual bool loadRawFilm(const std::string& name) = 0;

        virtual double compileDivergence() const = 0;

        double pixelDivergence(int i, int j) const;
        double pixelDivergence(const glm::ivec2& position) const;

        double pixelPriority(int i, int j) const;
        double pixelPriority(const glm::ivec2& position) const;

        double priorityThreshold() const;

        double sampleMultiplicity() const;



        // Samples
        glm::dvec4 pixelSample(int i, int j) const;
        glm::dvec4 pixelSample(const glm::ivec2& position) const;

        void addSample(int i, int j, const glm::dvec4& sample);
        void addSample(const glm::ivec2& position, const glm::dvec4& sample);


        // Tiles
        bool newTileCompleted();
        bool newFrameCompleted();

        virtual std::shared_ptr<Tile> getTile(size_t id);
        virtual std::shared_ptr<Tile> nextTile();
        virtual std::shared_ptr<Tile> endTile();

        void waitForFrameCompletion();
        virtual bool needNewTiles() const;
        virtual void tileCompleted(Tile& tile) = 0;

        virtual bool incomingTileAvailable() const;
        virtual std::shared_ptr<TileMessage> nextIncomingTile();
        virtual std::shared_ptr<TileMessage> nextOutgoingTile();


    protected:
        virtual void endTileReached() = 0;
        virtual double pixelDivergence(int index) const = 0;
        virtual double pixelPriority(int index) const = 0;
        virtual glm::dvec4 pixelSample(int index) const = 0;
        virtual void addSample(int index, const glm::dvec4& sample) = 0;

        virtual void buildTiles();

        int _stateUid;

        size_t _framePassCount;
        glm::ivec2 _frameResolution;
        std::vector<glm::vec3> _colorBuffer;
        std::vector<float> _depthBuffer;
        ColorOutput _colorOutput;

        std::mutex _cvMutex;
        std::condition_variable _cv;

        double _priorityThreshold;
        double _sampleMultiplicity;
        std::mutex _tilesMutex;
        glm::ivec2 _tilesResolution;

        int _nextTileId;
        std::shared_ptr<Tile> _endTile;
        std::vector<std::shared_ptr<Tile>> _tiles;

        bool _newTileCompleted;
        bool _newFrameCompleted;
    };



    // IMPLEMENTATION //
    inline int Film::stateUid() const
    {
        return _stateUid;
    }

    inline int Film::frameWidth() const
    {
        return _frameResolution.x;
    }

    inline int Film::frameHeight() const
    {
        return _frameResolution.y;
    }

    inline glm::ivec2 Film::frameResolution() const
    {
        return _frameResolution;
    }

    inline void Film::resizeFrame(int width, int height)
    {
        resizeFrame(glm::ivec2(width, height));
    }

    inline size_t Film::tileCount() const
    {
        return _tiles.size();
    }

    inline int Film::tilesWidth() const
    {
        return _tilesResolution.x;
    }

    inline int Film::tilesHeight() const
    {
        return _tilesResolution.y;
    }

    inline glm::ivec2 Film::tilesResolution() const
    {
        return _tilesResolution;
    }

    inline void Film::resizeTiles(int width, int height)
    {
        resizeTiles(glm::ivec2(width, height));
    }

    inline const std::vector<float>& Film::depthBuffer() const
    {
        return _depthBuffer;
    }

    inline double Film::pixelDivergence(int i, int j) const
    {
        int index = i + j * _frameResolution.x;
        return pixelDivergence(index);
    }

    inline double Film::pixelDivergence(const glm::ivec2& position) const
    {
        return pixelDivergence(position.x, position.y);
    }

    inline double Film::pixelPriority(int i, int j) const
    {
        int index = i + j * _frameResolution.x;
        return pixelPriority(index);
    }

    inline double Film::pixelPriority(const glm::ivec2& position) const
    {
        return pixelPriority(position.x, position.y);
    }

    inline double Film::priorityThreshold() const
    {
        return _priorityThreshold;
    }

    inline double Film::sampleMultiplicity() const
    {
        return _sampleMultiplicity;
    }

    inline glm::dvec4 Film::pixelSample(int i, int j) const
    {
        int index = i + j * _frameResolution.x;
        return pixelSample(index);
    }

    inline glm::dvec4 Film::pixelSample(const glm::ivec2& position) const
    {
        return pixelSample(position.x, position.y);
    }

    inline void Film::addSample(
            int i, int j, const glm::dvec4& sample)
    {
        int index = i + j * _frameResolution.x;
        addSample(index, sample);
    }

    inline void Film::addSample(
            const glm::ivec2& position,
            const glm::dvec4& sample)
    {
        addSample(position.x, position.y, sample);
    }
}

#endif // PROPROOM3D_FILM_H
