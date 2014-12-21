#ifndef SCAENA_ABSTRACT_PLAY_H
#define SCAENA_ABSTRACT_PLAY_H

#include <list>
#include <string>
#include <memory>

#include "../ScaenaException/ActNotFound.h"
#include "../ScaenaException/CharacterNotFound.h"


namespace scaena
{
    class AbstractCharacter;
    class AbstractAct;
    class AbstractStage;    
    class StageManager;


    class SCAENA_EXPORT AbstractPlay
    {
    protected:
        AbstractPlay(const std::string& id);

        // Interface to implement
        virtual void loadExternalRessources() =0;
        virtual void setUpPersistentCharacters() =0;
        virtual void setUpActs() =0;

    public:
        virtual ~AbstractPlay();

        // Life cycle
        virtual void setUp(); // Template method that calls loadExtR, setP, setA
        virtual void start();
        virtual void end();
        virtual void restart();

        // Persistent Characters
        virtual bool addPersistentCharacter(const std::shared_ptr<AbstractCharacter>& character);
        virtual bool deletePersistentCharacter(const std::string& id);
        virtual std::shared_ptr<AbstractCharacter> getPersistentCharacter(const std::string& id); //throw (CharacterNotFound)

        // Acts
        virtual bool addActAtPosition(const std::shared_ptr<AbstractAct>& act, int position);
        virtual bool appendAct(const std::shared_ptr<AbstractAct>& act);
        virtual bool deleteAct(const std::string& id);
        virtual AbstractAct& getAct(const std::string& id); //throw(ActNotFound)

        virtual bool gotoAct(const std::string& id);
        virtual bool gotoAct(int position);

        virtual const std::string firstActName()    const;
        virtual const std::string currentActName()  const;
        virtual const std::string lastActName()     const;
        virtual const std::string previousActName() const;
        virtual const std::string nextActName()     const;


        // Visitor
        virtual void welcome(StageManager& manager);


        // Getters
        const std::string id() const;
        AbstractStage& stage() const;
        AbstractAct& currentAct() const;

        // Setters
        void setStage(AbstractStage& stage);        

        // Operators
        bool operator== (const AbstractPlay& play) const;


    protected:
        bool _isPlaying;
        AbstractStage* _stage;
        std::string _id;
        std::list< std::shared_ptr<AbstractCharacter> > _persitentCharacters;
        typedef std::list< std::shared_ptr<AbstractCharacter> >::iterator CharIt;
        std::list< std::shared_ptr<AbstractAct> > _acts;
        typedef std::list< std::shared_ptr<AbstractAct> >::iterator ActIt;
        ActIt _currentAct;
    };



    // IMPLEMENTATION //
    inline const std::string AbstractPlay::id() const
        {return _id;}

    inline AbstractStage& AbstractPlay::stage() const
        {return *_stage;}

    inline AbstractAct& AbstractPlay::currentAct() const
    {return *(*_currentAct);}

    inline void AbstractPlay::setStage(AbstractStage& stage)
        {_stage = &stage;}

    inline bool AbstractPlay::operator== (const AbstractPlay& play) const
        {return _id == play._id;}
}

#endif // SCAENA_ABSTRACT_PLAY_H
