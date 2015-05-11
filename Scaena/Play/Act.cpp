#include "Act.h"

using namespace std;

#include "Character.h"
#include "Play.h"


namespace scaena
{
    Act::Act(Play& play, const string& id) :
        _play(play),
        _id(id),
        _position(0),
        _isPlaying(false)
    {

    }

    Act::~Act()
    {

    }

    void Act::openCurtain()
    {
        if(!_isPlaying)
        {
            _isPlaying = true;
            CharIterator charIt = _characters.begin();
            for(; charIt != _characters.end(); ++charIt)
                (*charIt)->enterStage();
        }
    }

    void Act::closeCurtain()
    {
        if(_isPlaying)
        {
            _isPlaying = false;
            CharIterator charIt = _characters.begin();
            for(; charIt != _characters.end(); ++charIt)
                (*charIt)->exitStage();
        }
    }

    void Act::restart()
    {
        closeCurtain();
        openCurtain();
    }

    std::shared_ptr<Character> Act::getCharacter(const string& id) const
    {
        auto charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            if((*charIt)->id() == id)
                return *charIt;

        return std::shared_ptr<Character>();
    }

    bool Act::addCharacter(const shared_ptr<Character>& character)
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

    bool Act::deleteCharacter(const string& id)
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

    void Act::welcome(StageManager& manager)
    {
        CharIterator charIt = _characters.begin();
        for(; charIt != _characters.end(); ++charIt)
            (*charIt)->welcome(manager);
    }
}
