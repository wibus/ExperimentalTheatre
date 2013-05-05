#ifndef SCAENA_ABSTRACT_STAGE_H
#define SCAENA_ABSTRACT_STAGE_H

#include <memory>

#include "libScaena_global.h"
#include "ScaenaException/PlayNotFound.h"


namespace cellar
{
    class AbstractClock;
}

namespace media
{
    class Camera;
}


namespace prop2
{
    class AbstractPropTeam;
}


namespace scaena
{    
    class AbstractPlay;
    class GraphicsTeam;
    class StageManager;
    class StageTime;
    class SynchronousKeyboard;
    class SynchronousMouse;

    class SCAENA_EXPORT AbstractStage
    {
    protected: AbstractStage(const std::string& id);

    public:
        virtual ~AbstractStage();

        // Life cycle
        virtual void mainLoop() =0;
        virtual void update();
        virtual void draw();
        virtual void terminate() =0;

        // Dimensions
        virtual int  width() const =0;
        virtual int  height() const =0;

        virtual void setSize(int w, int h) =0;
        virtual void setWidth(int w) =0;        
        virtual void setHeight(int h) =0;
        virtual void show() =0;
        virtual void setFullScreen(bool full) =0;

        // Timers
        virtual int  updateInterval() const =0;
        virtual int  drawInterval() const =0;
        virtual bool isDrawSynch() const =0;

        virtual void setUpdateInterval(int interval) =0;
        virtual void setDrawInterval(int interval) =0;
        virtual void setDrawSynch(bool isSynch) =0;


        // Getters
        const std::string     id()           const;
        AbstractPlay&         play()         const;
        bool                  isRunning()    const;
        media::Camera&        camera();
        prop2::AbstractPropTeam& propTeam()  const;
        const SynchronousKeyboard& synchronousKeyboard() const;
        const SynchronousMouse&    synchronousMouse() const;


        // Setters
        void setPropTeam(prop2::AbstractPropTeam* team);
        void  setAsRunning(bool run);
        virtual void startPlay(); //throw(PlayNotFound)
        virtual void endPlay();   //throw(PlayNotFound)
        void setPlay(const std::shared_ptr<AbstractPlay>& play);


    protected:
        std::string _id;
        std::shared_ptr<AbstractPlay> _play;
        std::shared_ptr<media::Camera> _camera;
        std::shared_ptr<prop2::AbstractPropTeam> _propTeam;
        std::shared_ptr<SynchronousKeyboard> _synchronousKeyboard;
        std::shared_ptr<SynchronousMouse> _synchronousMouse;
        std::shared_ptr<cellar::AbstractClock> _updateClock;
        std::shared_ptr<cellar::AbstractClock> _drawClock;
        bool _isRunning;
    };


    // IMPLEMENTATION //
    inline const std::string AbstractStage::id() const
        {return _id;}

    inline AbstractPlay& AbstractStage::play() const
        {return *_play;}

    inline bool AbstractStage::isRunning() const
        {return _isRunning;}

    inline media::Camera& AbstractStage::camera()
        {return *_camera;}

    inline prop2::AbstractPropTeam& AbstractStage::propTeam()  const
        {return *_propTeam;}

    inline const SynchronousKeyboard& AbstractStage::synchronousKeyboard() const
        {return *_synchronousKeyboard;}

    inline const SynchronousMouse& AbstractStage::synchronousMouse() const
        {return *_synchronousMouse;}

    inline void AbstractStage::setAsRunning(bool run)
        {_isRunning = run;}

    inline void AbstractStage::setPlay(const std::shared_ptr<AbstractPlay>& play)
        {_play = play;}
}


#endif // SCAENA_ABSTRACT_STAGE_H
