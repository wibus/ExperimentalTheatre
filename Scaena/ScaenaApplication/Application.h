#ifndef SCAENA_APPLICATION_H
#define SCAENA_APPLICATION_H

#include <memory>
#include <map>

#include <QObject>

#include <CellarWorkbench/DesignPattern/Singleton.h>
#include <CellarWorkbench/DateAndTime/Clock.h>

#include "../libScaena_global.h"


class QApplication;


namespace scaena
{
    class AbstractStage;
    class Play;

    class SCAENA_EXPORT Application :
            public QObject,
            public cellar::Singleton<Application>
    {
        Q_OBJECT

        friend class cellar::Singleton<Application>;

    private:
        Application();

    public:
        void init(int argc, char** argv);

        void setPlay(const std::shared_ptr<Play>& play);

        int execute();
        void terminate();

        // Getters
        QApplication& getQApplication() const;
        Play& play()const;


    signals:
        void aboutToQuitSignal();
    private slots:
        void aboutToQuitSlot();


    private:
        int _argc;
        char** _argv;
        std::shared_ptr< QApplication > _qApp;
        std::shared_ptr< Play > _play;
        cellar::Clock _totalTime;
    };

    SCAENA_EXPORT Application& getApplication();




    // IMPLEMENTATION //
    inline QApplication& Application::getQApplication() const
        {return *_qApp;}

    inline Play& Application::play() const
        {return *_play;}
}

#endif // SCAENA_SCAENA_APPLICATION_H
