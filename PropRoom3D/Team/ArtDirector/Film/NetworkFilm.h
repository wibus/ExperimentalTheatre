#ifndef PROPROOM3D_NETWORKFILM_H
#define PROPROOM3D_NETWORKFILM_H

#include <queue>
#include <chrono>

#include "Film.h"


namespace prop3
{
    class PROP3D_EXPORT NetworkFilm : public Film
    {
    public:
        NetworkFilm();
        virtual ~NetworkFilm();

        using Film::addSample;
        using Film::pixelSample;
        using Film::pixelPriority;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) override;

        virtual void backupAsReferenceShot() override;

        virtual bool saveReferenceShot(const std::string& name) const override;
        virtual bool loadReferenceShot(const std::string& name) override;
        virtual bool clearReferenceShot() override;

        virtual bool saveRawFilm(const std::string& name) const override;
        virtual bool loadRawFilm(const std::string& name) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;
        virtual void rewindTiles() override;

        virtual std::shared_ptr<TileMessage> nextOutgoingTile() override;
        void addOutgoingTile(const std::shared_ptr<TileMessage>& msg);


    protected:
        virtual void resetFilmState() override;
        virtual void clearBuffers(const glm::dvec3& color) override;
        virtual void endTileReached() override;
        virtual double pixelDivergence(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual glm::dvec4 pixelSample(int index) const override;
        virtual void addSample(int index, const glm::dvec4& sample) override;


        std::mutex _tileMsgMutex;
        std::queue<std::shared_ptr<TileMessage>> _tileMsgs;
        std::vector<glm::dvec4> _sampleBuffer;

        bool _bandwithOptimized;
        double _initialPixelPriority;
        double _currentPixelPriority;
        double _maxDataRateAvailable;
        size_t _cumulatedTileByteCount;
        size_t _cumulatedByteCountThreshold;
        std::chrono::high_resolution_clock::time_point _startTime;
    };
}

#endif // PROPROOM3D_NETWORKFILM_H
