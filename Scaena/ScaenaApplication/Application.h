#ifndef SCAENA_APPLICATION_H
#define SCAENA_APPLICATION_H

#include <memory>
#include <map>

#include <QObject>

#include <DesignPattern/Singleton.h>
#include <DateAndTime/Timer.h>

#include "libScaena_global.h"
#include "ScaenaException/PlayNotFound.h"
#include "ScaenaException/StageNotFound.h"


class QApplication;


namespace scaena
{
    class AbstractStage;
    class AbstractPlay;

    const std::string NO_STAGE = "";


    class SCAENA_EXPORT Application :
            public QObject,
            public cellar::Singleton<Application>
    {
        Q_OBJECT

        friend class cellar::Singleton<Application>;

    private: Application();
    public:

        QApplication& getQApplication() const;

        void init(int argc, char** argv);

        void setPlay(const std::shared_ptr<AbstractPlay>& play);

        bool addCustomStage(AbstractStage* stage);
        void chooseStage(const std::string& stageName);
        std::string userChooseStage();

        int execute();
        int execute(const std::string& stageId);
        void terminate();

        // Getters
        const std::string&  chosenStageName()   const;
        AbstractStage&      stage()             const; //throw(StageNotFound)
        AbstractPlay&       play()              const; //throw(PlayNotFound)


    signals:        void aboutToQuitSignal();
    private slots:  void aboutToQuitSlot();


    private:
        void setupDefaultStages();
        void findChosenStage();

        int _argc;
        char** _argv;
        std::shared_ptr< QApplication > _qApp;
        std::shared_ptr< AbstractPlay > _play;
        std::shared_ptr< AbstractStage > _chosenStage;
        std::string _chosenStageName;
        std::map< std::string, std::shared_ptr< AbstractStage > > _stages;
        typedef std::map< std::string, std::shared_ptr< AbstractStage > >::iterator StageIt;
        cellar::Timer _totalTime;
    };

    SCAENA_EXPORT Application& getApplication();


    // IMPLEMENTATION //
    inline const std::string& Application::chosenStageName() const
        {return _chosenStageName;}

    inline AbstractStage& Application::stage() const
        {return *_chosenStage;}

    inline AbstractPlay& Application::play() const
        {return *_play;}
}

#endif // SCAENA_SCAENA_APPLICATION_H
