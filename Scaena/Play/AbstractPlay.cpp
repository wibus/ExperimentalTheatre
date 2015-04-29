#include "AbstractPlay.h"

#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include "../Character/AbstractCharacter.h"
#include "../Act/AbstractAct.h"

using namespace std;
using namespace cellar;

namespace scaena
{
    AbstractPlay::AbstractPlay(const string& id) :
        _isPlaying(false),
        _stage(0),
        _id(id),
        _persitentCharacters(),
        _acts(),
        _currentAct()
    {}

    AbstractPlay::~AbstractPlay()
    {}

    void AbstractPlay::setUp()
    {
        loadExternalRessources();
        setUpPersistentCharacters();
        setUpActs();

        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
        {
            (*actIt)->installStageSet();
        }
    }

    void AbstractPlay::start()
    {
        setUp();

        if(_acts.empty())
        {
            getLog().postMessage(new Message('E', true,
                "Your trying to use a play with no act", "AbstractPlay"));

            _isPlaying = false;
        }
        else
        {
            _currentAct = _acts.begin();
            (*_currentAct)->openCurtain();

            _isPlaying = true;
        }
    }

    void AbstractPlay::end()
    {
        _isPlaying = false;

        if(!_acts.empty())
            (*_currentAct)->closeCurtain();

        _persitentCharacters.clear();
    }

    void AbstractPlay::restart()
    {
        if(_isPlaying)
        {
            (*_currentAct)->closeCurtain();
            _currentAct = _acts.begin();
            (*_currentAct)->openCurtain();
        }
    }

    bool AbstractPlay::addPersistentCharacter(const shared_ptr<AbstractCharacter>& character)
    {
        // Checks if character's id is already in _objects
        CharIt charIt;
        for(charIt = _persitentCharacters.begin();
            charIt != _persitentCharacters.end();
            ++charIt)
        {
            if((*charIt)->id() == character->id())
            {
                getLog().postMessage(new Message('E', false,
                    character->id()+" is already a persistent character",
                    "AbstractPlay"));
                return false;
            }
        }

        // if not
        _persitentCharacters.push_back(character);
        _persitentCharacters.sort();

        return true;
    }

    bool AbstractPlay::deletePersistentCharacter(const string& id)
    {
        CharIt charIt;
        for(charIt = _persitentCharacters.begin();
            charIt != _persitentCharacters.end();
            ++charIt)
            if((*charIt)->id() == id)
            {
                (*_currentAct)->deleteCharacter(id);
                _persitentCharacters.erase(charIt);
                return true;
            }

        return false;
    }

    shared_ptr<AbstractCharacter> AbstractPlay::getPersistentCharacter(const string& id)
    {
        CharIt charIt;
        for(charIt = _persitentCharacters.begin();
            charIt != _persitentCharacters.end();
            ++charIt)
            if((*charIt)->id() == id)
                return *charIt;

        throw CharacterNotFound(id + " is not in this play");
    }


    bool AbstractPlay::addActAtPosition(const shared_ptr<AbstractAct>& act, int position)
    {
        // Empty acts case
        if( _acts.empty() )
        {
            act->setPosition(position);
            _acts.push_back(act);
            return true;
        }


        // Check if the position is occupied
        // If not, insert at position specified in parameters
        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
        {
            if((*actIt)->position() > position)
            {
                act->setPosition(position);
                _acts.insert(actIt, act);
                return true;
            }
            else if ((*actIt)->position() == position)
            {
                getLog().postMessage(new Message('E', false,
                    "An act is already at position "+toString(position),
                    "AbstractPlay"));
                return false;
            }
        }

        // Append act
        act->setPosition(position);
        _acts.push_back(act);
        return true;
    }

    bool AbstractPlay::appendAct(const shared_ptr<AbstractAct>& act)
    {
        if(_acts.empty())
            act->setPosition(0);
        else
            act->setPosition(_acts.back()->position() + 1);

        _acts.push_back(act);
        return true;
    }

    bool AbstractPlay::deleteAct(const string& id)
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

    AbstractAct& AbstractPlay::getAct(const string& id)
    {
        ActIt actIt;
        for(actIt = _acts.begin(); actIt != _acts.end(); ++actIt)
            if((*actIt)->id() == id)
                return *(*actIt);

        throw ActNotFound(id + " is not in this play");
    }

    bool AbstractPlay::gotoAct(const string& id)
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

    bool AbstractPlay::gotoAct(int position)
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

    const string AbstractPlay::firstActName() const
    {
        return _acts.front()->id();
    }

    const string AbstractPlay::previousActName() const
    {
        if(_currentAct == _acts.begin())
        {
            return string("");
        }

        ActIt actIt = _currentAct;
        return (*(--actIt))->id();
    }

    const string AbstractPlay::currentActName() const
    {
        return (*_currentAct)->id();
    }

    const string AbstractPlay::nextActName() const
    {
        if(_currentAct != _acts.end())
        {
            return string("");
        }

        ActIt actIt = _currentAct;
        return (*(++actIt))->id();
    }

    const string AbstractPlay::lastActName() const
    {
        return _acts.back()->id();
    }

    void AbstractPlay::welcome(StageManager& manager)
    {
        if(_isPlaying)
            (*_currentAct)->welcome(manager);
    }
}
