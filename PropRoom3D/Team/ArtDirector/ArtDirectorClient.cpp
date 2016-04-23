#include "ArtDirectorClient.h"

#include "CpuRaytracerEngine.h"
#include "GlPostProdUnit.h"


namespace prop3
{
    ArtDirectorClient::ArtDirectorClient() :
        _postProdUnit(new GlPostProdUnit())
    {
#ifdef NDEBUG
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#else
        _localRaytracer.reset(new CpuRaytracerEngine(8));
#endif
    }

    ArtDirectorClient::~ArtDirectorClient()
    {

    }

    void ArtDirectorClient::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        _stageSet = stageSet;

        RaytracerState::DraftParams draftParams;
        draftParams.sizeRatio = 0;
        draftParams.levelCount = 0;
        draftParams.frameCountPerLevel = 0;
        draftParams.fastDraftEnabled = false;

        _localRaytracer->setup(draftParams);
        _postProdUnit->setup();
    }

    void ArtDirectorClient::update(double dt)
    {

    }

    void ArtDirectorClient::draw(double dt)
    {
        if(dt == FORCE_REFRESH_DT ||
           _localRaytracer->newTileCompleted())
        {
            if(dt == FORCE_REFRESH_DT ||
               raytracerState()->isUpdateEachTileEnabled())
                sendBuffersToGpu();

            if(_localRaytracer->newFrameCompleted())
            {
                if(!raytracerState()->isUpdateEachTileEnabled())
                    sendBuffersToGpu();

                // Let raytracer manage its drafts
                _localRaytracer->manageNextFrame();
            }
        }

        _postProdUnit->execute();
    }

    void ArtDirectorClient::terminate()
    {

    }

    void ArtDirectorClient::resize(int width, int height)
    {
    }

    void ArtDirectorClient::notify(cellar::CameraMsg &msg)
    {

    }

    void ArtDirectorClient::connectToServer()
    {

    }

    void ArtDirectorClient::deconnectFromServer()
    {

    }

    void ArtDirectorClient::setServerIpAddress(const std::string& ip)
    {

    }

    void ArtDirectorClient::setServerTcpPort(const std::string& port)
    {

    }

    std::shared_ptr<RaytracerState> ArtDirectorClient::raytracerState() const
    {
        return _localRaytracer->raytracerState();
    }

    void ArtDirectorClient::sendBuffersToGpu()
    {
        std::string colorOuputType = raytracerState()->colorOutputType();
        Film::ColorOutput colorOutput = Film::ColorOutput::ALBEDO;
        if(colorOuputType == RaytracerState::COLOROUTPUT_WEIGHT)
            colorOutput = Film::ColorOutput::WEIGHT;
        if(colorOuputType == RaytracerState::COLOROUTPUT_DIVERGENCE)
            colorOutput = Film::ColorOutput::DIVERGENCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_VARIANCE)
            colorOutput = Film::ColorOutput::VARIANCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_PRIORITY)
            colorOutput = Film::ColorOutput::PRIORITY;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_REFERENCE)
            colorOutput = Film::ColorOutput::REFERENCE;
        else if(colorOuputType == RaytracerState::COLOROUTPUT_COMPATIBILITY)
            colorOutput = Film::ColorOutput::COMPATIBILITY;

        _postProdUnit->update(*_localRaytracer->film(), colorOutput);
    }
}
