#ifndef SCAENA_PLAY_H
#define SCAENA_PLAY_H

#include <list>
#include <string>
#include <memory>
#include <chrono>

#include <QTimer>
#include <QObject>

#include <CellarWorkbench/DateAndTime/Clock.h>

#include "../libScaena_global.h"


namespace prop2
{
    class AbstractTeam;
}

namespace prop3
{
    class AbstractTeam;
}

namespace scaena
{
    class Act;
    class View;
    class MouseEvent;
    class KeyboardEvent;
    class SynchronousMouse;
    class SynchronousKeyboard;


    class SCAENA_EXPORT Play : public QObject
    {
        Q_OBJECT

    public:
        Play(const std::string& id);
        virtual ~Play();

        // Life cycle
        virtual void start();
        virtual void terminate();
        virtual void restart();

        // Refresh Rates
        void setDrawRate(int targetedFps);
        void setUpdateRate(int targetedFps);

        // Views
        static const std::string MAIN_VIEW;
        virtual bool addView(const std::shared_ptr<View>& view);
        virtual std::shared_ptr<View> view(const std::string& id = MAIN_VIEW) const;

        // Acts
        virtual std::shared_ptr<Act> getAct(const std::string& id) const;
        virtual bool appendAct(const std::shared_ptr<Act>& act);
        virtual bool deleteAct(const std::string& id);

        virtual bool gotoAct(const std::string& id);
        virtual bool gotoAct(int position);

        virtual const std::string firstActName()    const;
        virtual const std::string currentActName()  const;
        virtual const std::string lastActName()     const;
        virtual const std::string previousActName() const;
        virtual const std::string nextActName()     const;

        // Prompts
        virtual bool keyPressEvent(const KeyboardEvent& event);
        virtual bool keyReleaseEvent(const KeyboardEvent& event);
        virtual bool mousePressEvent(const MouseEvent& event);
        virtual bool mouseReleaseEvent(const MouseEvent& event);
        virtual bool mouseMoveEvent(const MouseEvent& event);
        virtual bool mouseWheelEvent(const MouseEvent& event);

        // Getters
        bool isPlaying() const;
        const std::string id() const;
        const std::shared_ptr<Act>& currentAct() const;
        const std::shared_ptr<prop3::AbstractTeam>& propTeam3D() const;
        const std::shared_ptr<prop2::AbstractTeam>& propTeam2D() const;
        const std::shared_ptr<SynchronousMouse>& synchronousMouse() const;
        const std::shared_ptr<SynchronousKeyboard>& synchronousKeyboard() const;

        // Operators
        bool operator== (const Play& play) const;


        static const int DRAW_EQUALS_UPDATE_TIMESTEP;
        static const int DEACTIVATE_AUTOMATIC_REFRESH;
        static const int FASTEST_REFRESH_RATE_AVAILABLE;


    protected slots:
        virtual void update();
        virtual void draw();


    protected:
        std::string _id;
        bool _isPlaying;

        QTimer _drawTimer;
        QTimer _updateTimer;
        cellar::Clock _drawClock;
        cellar::Clock _updateClock;

        bool _fastestDraw;
        bool _fastestUpdate;
        bool _syncDrawWithUpdate;
        bool _mustStartDrawTimer;
        bool _mustStartUpdateTimer;
        std::chrono::high_resolution_clock::time_point _lastForcedUpdate;

        std::shared_ptr<prop2::AbstractTeam> _propTeam2D;
        std::shared_ptr<prop3::AbstractTeam> _propTeam3D;

        std::shared_ptr<SynchronousMouse> _synchronousMouse;
        std::shared_ptr<SynchronousKeyboard> _synchronousKeyboard;

        std::vector<std::shared_ptr<View>> _views;

        std::list< std::shared_ptr<Act> > _acts;
        typedef std::list< std::shared_ptr<Act> >::iterator ActIt;
        ActIt _currentAct;
    };



    // IMPLEMENTATION //
    inline bool Play::isPlaying() const
        {return _isPlaying;}

    inline const std::string Play::id() const
        {return _id;}

    inline const std::shared_ptr<Act>&  Play::currentAct() const
        {return *_currentAct;}

    inline bool Play::operator== (const Play& play) const
        {return _id == play._id;}

    inline const std::shared_ptr<prop3::AbstractTeam>& Play::propTeam3D() const
        {return _propTeam3D;}

    inline const std::shared_ptr<prop2::AbstractTeam>& Play::propTeam2D() const
        {return _propTeam2D;}

    inline const std::shared_ptr<SynchronousMouse>& Play::synchronousMouse() const
        {return _synchronousMouse;}

    inline const std::shared_ptr<SynchronousKeyboard>& Play::synchronousKeyboard() const
        {return _synchronousKeyboard;}
}

#endif // SCAENA_PLAY_H
