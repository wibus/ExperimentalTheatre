#include "Play.h"

#include <QApplication>

#include <CellarWorkbench/DateAndTime/Clock.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <PropRoom2D/Team/StdTeam.h>
#include <PropRoom3D/Team/StdTeam.h>

#include "Act.h"
#include "View.h"
#include "Character.h"
#include "../StageManagement/Manager/BeginStepCaller.h"
#include "../StageManagement/Manager/EndStepCaller.h"
#include "../StageManagement/Manager/DrawCaller.h"
#include "../StageManagement/Manager/KeyPressDispatcher.h"
#include "../StageManagement/Manager/KeyReleaseDispatcher.h"
#include "../StageManagement/Manager/MouseMoveDispatcher.h"
#include "../StageManagement/Manager/MouseWheelDispatcher.h"
#include "../StageManagement/Manager/MousePressDispatcher.h"
#include "../StageManagement/Manager/MouseReleaseDispatcher.h"
#include "../StageManagement/Event/SynchronousKeyboard.h"
#include "../StageManagement/Event/SynchronousMouse.h"
#include "../StageManagement/Event/KeyboardEvent.h"
#include "../StageManagement/Event/MouseEvent.h"

using namespace std;
using namespace cellar;

namespace scaena
{
    const std::string Play::MAIN_VIEW = "@MAIN_VIEW";
    const int Play::DRAW_EQUALS_UPDATE_TIMESTEP = -2;
    const int Play::DEACTIVATE_AUTOMATIC_REFRESH = 0;
    const int Play::FASTEST_REFRESH_RATE_AVAILABLE = -1;


    Play::Play(const string& id) :
        _isPlaying(false),
        _id(id),
        _fastestDraw(false),
        _fastestUpdate(false),
        _syncDrawWithUpdate(false),
        _mustStartDrawTimer(false),
        _mustStartUpdateTimer(false),
        _synchronousMouse(new SynchronousMouse()),
        _synchronousKeyboard(new SynchronousKeyboard())
    {        
        _drawTimer.setSingleShot(false);
        _updateTimer.setSingleShot(false);
        setDrawRate(DRAW_EQUALS_UPDATE_TIMESTEP);
        setUpdateRate(60);

        QObject::connect(&_drawTimer, &QTimer::timeout, this, &Play::draw);
        QObject::connect(&_updateTimer, &QTimer::timeout, this, &Play::update);
    }

    Play::~Play()
    {

    }

    void Play::start()
    {
        if(_acts.empty())
        {
            getLog().postMessage(new Message('E', true,
                "Your trying to use a play with no act", "AbstractPlay"));

            _isPlaying = false;
        }
        else
        {
            _currentAct = _acts.begin();

            _drawClock.reset();
            _drawClock.start();
            _updateClock.reset();
            _updateClock.start();

            if(_mustStartDrawTimer)
            {
                _drawTimer.start();
            }
            if(_mustStartUpdateTimer)
            {
                _updateTimer.start();
            }

            using std::chrono::high_resolution_clock;
            _lastForcedUpdate = high_resolution_clock::now();

            _propTeam2D.reset(new prop2::StdTeam());
            _propTeam3D.reset(new prop3::StdTeam());

            for(auto& view : _views)
            {
                view->setup(*this);
            }

            _propTeam2D->setup();
            _propTeam3D->setup();

            (*_currentAct)->openCurtain();

            _isPlaying = true;
        }
    }

    void Play::terminate()
    {
        if(_isPlaying)
        {
            _drawClock.stop();
            _updateClock.stop();

            _drawTimer.stop();
            _updateTimer.stop();

            // Reset teams
            _propTeam2D->reset();
            _propTeam3D->reset();

            // Reset play
            (*_currentAct)->closeCurtain();
            _currentAct = _acts.begin();
        }
    }

    void Play::restart()
    {
        terminate();
        start();
    }

    void Play::setDrawRate(int targetedFps)
    {
        _fastestDraw = (targetedFps == FASTEST_REFRESH_RATE_AVAILABLE);
        _syncDrawWithUpdate = (targetedFps == DRAW_EQUALS_UPDATE_TIMESTEP);

        if(targetedFps == DRAW_EQUALS_UPDATE_TIMESTEP)
        {
            _mustStartDrawTimer = false;
            _drawTimer.stop();
        }
        else if(targetedFps == DEACTIVATE_AUTOMATIC_REFRESH)
        {
            _mustStartDrawTimer = false;
            _drawTimer.stop();
        }
        else if(targetedFps == FASTEST_REFRESH_RATE_AVAILABLE)
        {
            _mustStartDrawTimer = true;
            _drawTimer.setSingleShot(false);
            _drawTimer.setInterval(0);

            if(_isPlaying)
                _drawTimer.start();
        }
        else
        {
            _mustStartDrawTimer = true;
            _drawTimer.setSingleShot(false);
            _drawTimer.setInterval(glm::max(0, 1000 / targetedFps));

            if(_isPlaying)
                _drawTimer.start();
        }
    }

    void Play::setUpdateRate(int targetedFps)
    {
        _fastestUpdate = (targetedFps == FASTEST_REFRESH_RATE_AVAILABLE);

        if(targetedFps == DEACTIVATE_AUTOMATIC_REFRESH)
        {
            _mustStartUpdateTimer = false;
            _updateTimer.stop();
        }
        else if(targetedFps == FASTEST_REFRESH_RATE_AVAILABLE)
        {
            _mustStartUpdateTimer = true;
            _updateTimer.setSingleShot(false);
            _updateTimer.setInterval(0);

            if(_isPlaying)
                _updateTimer.start();
        }
        else
        {
            _mustStartUpdateTimer = true;
            _updateTimer.setSingleShot(false);
            _updateTimer.setInterval(glm::max(0, 1000 / targetedFps));

            if(_isPlaying)
                _updateTimer.start();
        }
    }

    bool Play::addView(const std::shared_ptr<View>& view)
    {
        for(auto& v : _views)
        {
            if(v->id() == view->id())
                return false;
        }

        _views.push_back(view);

        return true;
    }

    std::shared_ptr<View> Play::view(const std::string& id) const
    {
        if(!_views.empty())
        {
            if(id == MAIN_VIEW)
                return _views[0];

            for(auto& view : _views)
            {
                if(view->id() == id)
                    return view;
            }
        }

        return std::shared_ptr<View>();
    }

    void Play::drawCharactersFromView(const View& view)
    {
        if(!_isPlaying)
            return ;

        // Draw from view only if it was installed
        for(const auto& v : _views)
        {
            if(v->id() == view.id())
            {
                StageTime time(_drawClock.totalSeconds(),
                               _drawClock.elapsedSeconds(),
                               _drawClock.ticksPerSecond());

                DrawCaller drawCaller(v, time);
                (*_currentAct)->welcome( drawCaller );
                break;
            }
        }
    }

    std::shared_ptr<Act> Play::getAct(const string& id) const
    {
        for(auto& act : _acts)
            if(act->id() == id)
                return act;

        return shared_ptr<Act>();
    }

    bool Play::appendAct(const shared_ptr<Act>& act)
    {
        if(_acts.empty())
            act->setPosition(0);
        else
            act->setPosition(_acts.back()->position() + 1);

        _acts.push_back(act);
        return true;
    }

    bool Play::deleteAct(const string& id)
    {
        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
            if((*actIt)->id() == id)
            {
                _acts.erase(actIt);
                return true;
            }

        return false;
    }

    bool Play::gotoAct(const string& id)
    {
        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
        {
            if((*actIt)->id() == id)
            {
                (*_currentAct)->closeCurtain();
                _currentAct = actIt;
                (*_currentAct)->openCurtain();

                return true;
            }
        }

        return false;
    }

    bool Play::gotoAct(int position)
    {
        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
        {
            if((*actIt)->position() == position)
            {
                (*_currentAct)->closeCurtain();
                _currentAct = actIt;
                (*_currentAct)->openCurtain();

                return true;
            }
        }

        return false;
    }

    const string Play::firstActName() const
    {
        return _acts.front()->id();
    }

    const string Play::previousActName() const
    {
        if(_currentAct == _acts.begin())
        {
            return string("");
        }

        ActIt actIt = _currentAct;
        return (*(--actIt))->id();
    }

    const string Play::currentActName() const
    {
        return (*_currentAct)->id();
    }

    const string Play::nextActName() const
    {
        if(_currentAct != _acts.end())
        {
            return string("");
        }

        ActIt actIt = _currentAct;
        return (*(++actIt))->id();
    }

    const string Play::lastActName() const
    {
        return _acts.back()->id();
    }


    bool Play::keyPressEvent(const KeyboardEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousKeyboard->setIsAsciiPressed(event.getAscii(), true);
            _synchronousKeyboard->setIsNonAsciiPressed(event.getNonAscii(), true);
            KeyPressDispatcher kpVisitor( event );
            (*_currentAct)->welcome( kpVisitor );
            return kpVisitor.eventConsumed();
        }

        return false;
    }

    bool Play::keyReleaseEvent(const KeyboardEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousKeyboard->setIsAsciiPressed(event.getAscii(), false);
            _synchronousKeyboard->setIsNonAsciiPressed(event.getNonAscii(), false);
            KeyReleaseDispatcher krVisitor( event );
            (*_currentAct)->welcome( krVisitor );
            return krVisitor.eventConsumed();
        }

        return false;
    }

    bool Play::mousePressEvent(const MouseEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousMouse->setButtonIsPressed(event.button(), true);
            MousePressDispatcher mpVisitor( event );
            (*_currentAct)->welcome( mpVisitor );
            return mpVisitor.eventConsumed();
        }

        return false;
    }

    bool Play::mouseReleaseEvent(const MouseEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousMouse->setButtonIsPressed(event.button(), false);
            MouseReleaseDispatcher mrVisitor( event );
            (*_currentAct)->welcome( mrVisitor );
            return mrVisitor.eventConsumed();
        }

        return false;
    }

    bool Play::mouseMoveEvent(const MouseEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousMouse->setPosition( event.position() );
            MouseMoveDispatcher mmVisitor( event );
            (*_currentAct)->welcome( mmVisitor );
            return mmVisitor.eventConsumed();
        }

        return false;
    }

    bool Play::mouseWheelEvent(const MouseEvent& event)
    {
        if(_isPlaying)
        {
            _synchronousMouse->setDegreeDelta( event.degreeDelta() );
            MouseWheelDispatcher mmVisitor( event );
            (*_currentAct)->welcome( mmVisitor );
            return mmVisitor.eventConsumed();
        }

        return false;
    }

    void Play::update()
    {
        if(!_isPlaying)
            return;

        _updateClock.tick();
        double dt = _updateClock.elapsedSeconds();
        StageTime time(_updateClock.totalSeconds(),
                       _updateClock.elapsedSeconds(),
                       _updateClock.ticksPerSecond());

        BeginStepCaller beginStepCaller( time );
        (*_currentAct)->welcome( beginStepCaller );

        _propTeam3D->update( dt );
        _propTeam2D->update( dt );

        if(_syncDrawWithUpdate)
        {
            draw();
        }

        EndStepCaller endStepCaller( time );
        (*_currentAct)->welcome( endStepCaller );

        // Set displacement to zero
        _synchronousMouse->setDegreeDelta(0);
        _synchronousMouse->setPosition(
            _synchronousMouse->position());
    }

    void Play::draw()
    {
        if(!_isPlaying)
            return;

        _drawClock.tick();
        double dt = _drawClock.elapsedSeconds();
        StageTime time(_drawClock.totalSeconds(),
                       _drawClock.elapsedSeconds(),
                       _drawClock.ticksPerSecond());

        for(auto& view : _views)
        {
            view->beginDraw( dt );

            DrawCaller drawCaller(view, time);
            (*_currentAct)->welcome( drawCaller );

            view->endDraw( dt );
        }
    }
}
