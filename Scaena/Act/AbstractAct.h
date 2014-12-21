#ifndef SCAENA_ABSTRACT_ACT_H
#define SCAENA_ABSTRACT_ACT_H

#include <string>
#include <list>
#include <memory>

#include "../ScaenaException/CharacterNotFound.h"


namespace scaena
{
    class AbstractCharacter;
    class AbstractPlay;
    class AbstractStage;
    class StageManager;


    class SCAENA_EXPORT AbstractAct
    {
    protected: AbstractAct(AbstractPlay& play, const std::string& id);

    public:
        virtual ~AbstractAct();

        // Interface to implement
        virtual void installStageSet() =0;


        // Open close
        virtual void openCurtain();
        virtual void closeCurtain();
        virtual void restart();

        // Characters
        virtual bool addCharacter(const std::shared_ptr<AbstractCharacter>& character);
        virtual bool deleteCharacter(const std::string& id);
        virtual AbstractCharacter& getCharacter(const std::string& id); //throw(CharacterNotFound)

        // Attributs getters
        AbstractPlay&  play()    const;
        AbstractStage& stage()   const;
        const std::string id()   const;
        int position()           const;
        bool isPlaying()         const;

        // Attributs setters
        void setPosition(int pos);
        void isPlaying(bool playing);

        // Visitors
        void welcome(StageManager& manager);

        // Operators
        bool operator< (const AbstractAct& act) const;
        bool operator== (const AbstractAct& act) const;


    protected :
        AbstractPlay& _play;
        AbstractStage& _stage;
        std::string _id;
        int _position;
        bool _isPlaying;
        std::list< std::shared_ptr<AbstractCharacter> > _characters;
        typedef std::list< std::shared_ptr<AbstractCharacter> >::iterator CharIterator;
    };



    // IMPLEMENTATION //
    inline AbstractPlay& AbstractAct::play() const
        {return _play;}

    inline AbstractStage& AbstractAct::stage() const
        {return _stage;}

    inline const std::string AbstractAct::id() const
        {return _id;}

    inline int AbstractAct::position() const
        {return _position;}

    inline bool AbstractAct::isPlaying() const
        {return _isPlaying;}

    inline void AbstractAct::setPosition(int pos)
        {_position = pos;}

    inline void AbstractAct::isPlaying(bool playing)
        {_isPlaying = playing;}

    inline bool AbstractAct::operator< (const AbstractAct& act) const
        {return _position < act._position;}

    inline bool AbstractAct::operator== (const AbstractAct& act) const
        {return _id == act._id;}
}

#endif // SCAENA_ABSTRACT_ACT_H
