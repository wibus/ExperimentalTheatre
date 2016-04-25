#ifndef PROPROOM3D_NETWORKFILM_H
#define PROPROOM3D_NETWORKFILM_H

#include <queue>

#include "Film.h"


namespace prop3
{
    class PROP3D_EXPORT NetworkFilm : public Film
    {
    public:
        NetworkFilm();
        virtual ~NetworkFilm();

        using Film::pixelSample;
        using Film::addSample;
        using Film::pixelPriority;

        virtual const std::vector<glm::vec3>& colorBuffer(ColorOutput colorOutput) override;

        virtual void clear(const glm::dvec3& color = glm::dvec3(0),
                           bool hardReset = false) override;

        virtual void backupAsReferenceShot() override;

        virtual bool saveReferenceShot(const std::string& name) const override;
        virtual bool loadReferenceShot(const std::string& name) override;
        virtual bool clearReferenceShot() override;

        virtual bool saveRawFilm(const std::string& name) const override;
        virtual bool loadRawFilm(const std::string& name) override;

        virtual double compileDivergence() const override;

        virtual void tileCompleted(Tile& tile) override;

        virtual std::shared_ptr<TileMessage> nextOutgoingTileMessage() override;
        void addOutgoingTileMessage(const std::shared_ptr<TileMessage>& msg);


    protected:
        virtual void endTileReached() override;
        virtual double pixelDivergence(int index) const override;
        virtual double pixelPriority(int index) const override;
        virtual glm::dvec4 pixelSample(int index) const override;
        virtual void addSample(int index, const glm::dvec4& sample) override;


        std::mutex _tileMsgMutex;
        std::queue<std::shared_ptr<TileMessage>> _tileMsgs;
        std::vector<glm::dvec4> _sampleBuffer;
    };
}

#endif // PROPROOM3D_NETWORKFILM_H
