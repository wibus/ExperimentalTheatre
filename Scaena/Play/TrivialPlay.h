#ifndef SCAENA_TRIVIALPLAY_H
#define SCAENA_TRIVIALPLAY_H

#include "SingleActPlay.h"


namespace scaena
{
    class SCAENA_EXPORT TrivialPlay : public SingleActPlay
    {
    public:
        TrivialPlay(const std::string& id,
                    const std::shared_ptr<AbstractCharacter>& character);
        virtual ~TrivialPlay();

        // Characters
        virtual bool addPersistentCharacter(const std::shared_ptr<AbstractCharacter>& character);
        virtual bool deletePersistentCharacter(const std::string& id);

    protected:
        // Now implemented
        virtual void loadExternalRessources();
        virtual void setUpPersistentCharacters();

        std::shared_ptr<AbstractCharacter> _singletonCharacter;
    };
}

#endif // SCAENA_TRIVIALPLAY_H
