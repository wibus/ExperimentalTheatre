#include "AbstractAct.h"

using namespace std;

#include "../Character/AbstractCharacter.h"
#include "../Play/AbstractPlay.h"


namespace scaena
{
    AbstractAct::AbstractAct(AbstractPlay& play, const string& id) :
        _play(play),
        _stage(play.stage()),
        _id(id),
        _position(0),
        _isPlaying(false),
        _characters()
    {}

    AbstractAct::~AbstractAct()
    {}

    void AbstractAct::openCurtain()
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            (*charIt)->enterStage();
    }

    void AbstractAct::closeCurtain()
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            (*charIt)->exitStage();
    }

    void AbstractAct::restart()
    {
        closeCurtain();
        openCurtain();
    }

    bool AbstractAct::addCharacter(const shared_ptr<AbstractCharacter>& character)
    {
        // Checks if character's id is already in _objects
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            if((*charIt)->id() == character->id())
                return false;

        // if not
        _characters.push_back(character);
        _characters.sort();

        if(_isPlaying)
            character->enterStage();

        return true;
    }

    bool AbstractAct::deleteCharacter(const string& id)
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            if((*charIt)->id() == id)
            {
                if(_isPlaying)
                    (*charIt)->exitStage();
                _characters.erase(charIt);
                return true;
            }

        return false;
    }

    AbstractCharacter& AbstractAct::getCharacter(const string& id)
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            if((*charIt)->id() == id)
                return *(*charIt);

        throw CharacterNotFound(id + " is not in this play");
    }

    void AbstractAct::welcome(StageManager& manager)
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            (*charIt)->welcome(manager);
    }
}
