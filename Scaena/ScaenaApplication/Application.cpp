#include "Application.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QComboBox>
#include <QDir>

#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include "../Play/Play.h"

using namespace std;
using namespace cellar;


namespace scaena
{

    Application& getApplication()
    {
        return *Application::getInstance();
    }

    Application::Application() :
        _argc(0),
        _argv(nullptr),
        _qApp(),
        _play()
    {

    }

    void Application::init(int argc, char** argv)
    {
        _argc = argc;
        _argv = argv;
        _qApp.reset(new QApplication(_argc, _argv));

        // Init QApplication
        connect(&(*_qApp), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuitSlot()));

        QString cwd = QDir::currentPath();
        getLog().postMessage(
          new Message('I', false,"Current working directory is " + cwd.toStdString(),
            "Application"));
    }

    void Application::setPlay(const std::shared_ptr<Play>& play)
    {
        _play = play;
    }

    int Application::execute()
    {
        if(_qApp.get() == nullptr)
            // Application was not initialized
        {
            getLog().postMessage(
              new Message('E', true,
                "Application::init() need to be called before execute()",
                "Application"));
            terminate();
            return -1;
        }

        if(_play.get() == nullptr)
        {
            getLog().postMessage(
              new Message('E', true,
                "No play or a null pointer was assigned at init()",
                "Application"));
            terminate();
            return -1;
        }

        _totalTime.start();

        _play->start();
        if(_play->isPlaying())
        {
            _qApp->exec();
        }
        terminate();

        return 0;
    }

    void Application::aboutToQuitSlot()
    {
        emit aboutToQuitSignal();
    }

    void Application::terminate()
    {
        if(_play.get() != nullptr && _play->isPlaying())
            _play->terminate();

        _play.reset();

        if(_qApp.get() != nullptr)
            _qApp->exit();

        _qApp.reset();

        getLog().postMessage( new Message('I',false,
            "Total running time : " + toString(_totalTime.stop()) + " seconds",
            "Application"));
    }
}
