#include "Application.h"

#include "StageChooserDialog.h"
#include "Stage/QGLStage.h"
#include "Play/AbstractPlay.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QComboBox>
#include <Misc/Log.h>
#include <Misc/CellarUtils.h>

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
        _play(),
        _chosenStage(),
        _chosenStageName(),
        _stages()
    {}

    void Application::init(int argc, char** argv)
    {
        _argc = argc;
        _argv = argv;
        _qApp.reset(new QApplication(_argc, _argv));
        _chosenStageName = NO_STAGE;

        // Init QApplication
        connect(&(*_qApp), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuitSlot()));

        // Setup default stages
        setupDefaultStages();
    }

    void Application::setupDefaultStages()
    {
        /*
        // QGLStage
        QGLStage* qGLStage = new QGLStage();
        _stages.insert(make_pair(qGLStage->id(),shared_ptr<AbstractStage>(qGLStage)));
        */
    }

    QApplication& Application::getQApplication() const
    {
        return *_qApp;
    }

    void Application::setPlay(const std::shared_ptr<AbstractPlay>& play)
    {
        _play = play;
    }

    bool Application::addCustomStage(AbstractStage* stage)
    {
        return _stages.insert(make_pair(stage->id(),shared_ptr<AbstractStage>(stage))).second;
    }

    void Application::chooseStage(const std::string& stageName)
    {
        _chosenStageName = stageName;

        findChosenStage();
        _play->setStage(*_chosenStage);
        _chosenStage->setPlay(_play);
    }

    std::string Application::userChooseStage()
    {
        vector< string > stageNames;
        for(StageIt stageIt = _stages.begin(); stageIt != _stages.end(); ++stageIt)
            stageNames.push_back(stageIt->first);

        StageChooserDialog stageChooser;
        stageChooser.setChoices(stageNames);
        stageChooser.askUser();

        if(stageChooser.userQuit())
        {
            getLog().postMessage(
              new Message('E', true, "User quit stage chooser dialog",
                          "Application"));
            exit(-1);
        }

        _chosenStageName = stageChooser.userFinalChoice();
        chooseStage( _chosenStageName );

        getLog().postMessage(
          new Message('I', true, "User chose stage : " + _chosenStageName,
                      "Application"));


        return _chosenStageName;
    }

    int Application::execute()
    {
        if(_chosenStageName != NO_STAGE)
        {
            return execute(_chosenStageName);
        }

        return execute(userChooseStage());
    }

    int Application::execute(const std::string& stageId)
    {
        chooseStage(stageId);

        if(_qApp == 0x0)
            // Application was not initialized
        {
            getLog().postMessage(
              new Message('E', true,
                "Application::init() need to be called before execute()",
                "Application"));
            return -1;
        }

        if(_play == 0x0)
        {
            getLog().postMessage(
              new Message('E', true,
                "No play or a null pointer was assigned at init()",
                "Application"));
            return -1;
        }

        if(_chosenStage == 0x0)
        {
            getLog().postMessage(
              new Message('E', true,
                "No stage was chosen or a null pointer was assigned",
                "Application"));
            return -1;
        }

        _chosenStage->show();
        _totalTime.start();

        _chosenStage->mainLoop();

        return 0;
    }

    void Application::findChosenStage()
    {
        StageIt stageIt = _stages.find(_chosenStageName);

        if(stageIt == _stages.end())
        {
            getLog().postMessage(
              new Message('E', true, "Invalid stage name : " + _chosenStageName,
                          "Application"));
            exit(-1);
        }

        _chosenStage = stageIt->second;
    }


    void Application::aboutToQuitSlot()
    {
        if(_chosenStage != 0x0)
        {
            getLog().postMessage(new Message('I',false,"Total running time : " +
                                             toString(_totalTime.stop()) +
                                             " seconds", "Application"));
            emit aboutToQuitSignal();
        }
    }

    void Application::terminate()
    {
        if(_chosenStage != 0x0)
        {
            getLog().postMessage( new Message('I', true,
                "The user quit by invoking Application::terminate()",
                "Application"));

            _chosenStage->terminate();
        }
    }
}
