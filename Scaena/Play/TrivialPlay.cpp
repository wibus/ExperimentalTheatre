#include "TrivialPlay.h"


namespace scaena
{
    TrivialPlay::TrivialPlay(
            const std::string& id,
            const std::shared_ptr<AbstractCharacter>& character) :
        SingleActPlay(id),
        _singletonCharacter(character)
    {
    }

    TrivialPlay::~TrivialPlay()
    {

    }

    // Characters
    bool TrivialPlay::addPersistentCharacter(const std::shared_ptr<AbstractCharacter>&)
    {
        return false;
    }

    bool TrivialPlay::deletePersistentCharacter(const std::string&)
    {
        return false;
    }

    void TrivialPlay::loadExternalRessources()
    {
    }

    void TrivialPlay::setUpPersistentCharacters()
    {
        SingleActPlay::addPersistentCharacter(_singletonCharacter);
    }
}
