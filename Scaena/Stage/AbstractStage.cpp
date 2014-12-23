#include "AbstractStage.h"

using namespace std;

#include <CellarWorkbench/DateAndTime/CClock.h>
using namespace cellar;

#include <MediaWorkbench/Camera/Camera.h>
using namespace media;

#include <PropRoom2D/PropTeam/AbstractPropTeam.h>
#include <PropRoom3D/Team/AbstractTeam.h>
using namespace prop2;
using namespace prop3;

#include "../Play/AbstractPlay.h"
#include "Manager/BeginStepCaller.h"
#include "Manager/EndStepCaller.h"
#include "Manager/DrawCaller.h"
#include "Event/SynchronousKeyboard.h"
#include "Event/SynchronousMouse.h"


namespace scaena
{
    AbstractStage::AbstractStage(const string& id) :
        _id(id),
        _play(),
        _camera(new Camera()),
        _propTeam2D(nullptr),
        _propTeam3D(nullptr),
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

    void AbstractStage::setPropTeam2D(prop2::AbstractPropTeam* team)
    {
        _propTeam2D.reset(team);
    }

    void AbstractStage::setPropTeam3D(prop3::AbstractTeam* team)
    {
        _propTeam3D.reset(team);
    }

    void AbstractStage::update()
    {
        if(_play != 0x0)
        {
            _updateClock->tick();
            StageTime time(_updateClock->totalSeconds(),
                           _updateClock->elapsedSeconds(),
                           _updateClock->ticksPerSecond());

            BeginStepCaller beginStepCaller( time );
            _play->welcome( beginStepCaller );

            _propTeam3D->update( time.elapsedTime() );
            _propTeam2D->update( time.elapsedTime() );

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
            StageTime time(_drawClock->totalSeconds(),
                           _drawClock->elapsedSeconds(),
                           _drawClock->ticksPerSecond());

            _propTeam3D->draw( time.elapsedTime() );

            DrawCaller drawCaller( time );
            _play->welcome( drawCaller );

            _propTeam2D->draw( time.elapsedTime() );
        }
    }

    void AbstractStage::startPlay()
    {
        if(_play.get() == 0x0)
            throw PlayNotFound("No play to start");

        _propTeam3D->setCamera(*_camera);
        _propTeam3D->setup();

        _propTeam2D->setCamera(*_camera);
        _propTeam2D->setup();

        _camera->refresh();

        _play->start();
    }

    void AbstractStage::endPlay()
    {
        if(_play.get() == 0x0)
            throw PlayNotFound("No play to end");

        _play->end();
    }
}
