#include "Stage/AbstractStage.h"

#include "Play/AbstractPlay.h"
#include "Stage/Manager/BeginStepCaller.h"
#include "Stage/Manager/EndStepCaller.h"
#include "Stage/Manager/DrawCaller.h"
#include "Stage/Event/SynchronousKeyboard.h"
#include "Stage/Event/SynchronousMouse.h"

using namespace std;

#include <DateAndTime/CClock.h>
using namespace cellar;

#include <Camera/Camera.h>
using namespace media;

#include <PropTeam/AbstractPropTeam.h>
using namespace prop2;

namespace scaena
{
    AbstractStage::AbstractStage(const string& id) :
        _id(id),
        _play(),
        _camera(new Camera()),
        _propTeam((AbstractPropTeam*)nullptr),
        _synchronousKeyboard(new SynchronousKeyboard()),
        _synchronousMouse(new SynchronousMouse()),
        _updateClock(new CClock()),
        _drawClock(new CClock()),
        _isRunning(false)
    {
    }

    AbstractStage::~AbstractStage()
    {
    }

    void AbstractStage::setPropTeam(prop2::AbstractPropTeam* team)
    {
        _propTeam.reset(team);
    }

    void AbstractStage::update()
    {
        if(_play != 0x0)
        {
            _updateClock->tick();
            StageTime time(_updateClock->secondsPerTick(),
                           _updateClock->totalSeconds());

            BeginStepCaller beginStepCaller( time );
            _play->welcome( beginStepCaller );

            _propTeam->update( time.elapsedTime() );

            EndStepCaller endStepCaller( time );
            _play->welcome( endStepCaller );

            // Set displacement to zero
            _synchronousMouse->setPosition(synchronousMouse().position());
        }
    }

    void AbstractStage::draw()
    {
        if(_play != 0x0)
        {
            _drawClock->tick();
            StageTime time(_drawClock->secondsPerTick(),
                           _drawClock->totalSeconds());

            DrawCaller drawCaller( time );
            _play->welcome( drawCaller );

            _propTeam->draw( time.elapsedTime() );
        }
    }

    void AbstractStage::startPlay()
    {
        if(_play.get() == 0x0)
            throw PlayNotFound("No play to start");

        _propTeam->setup();
        _propTeam->setCamera(*_camera);
        _play->start();
    }

    void AbstractStage::endPlay()
    {
        if(_play.get() == 0x0)
            throw PlayNotFound("No play to end");

        _play->end();
    }
}
