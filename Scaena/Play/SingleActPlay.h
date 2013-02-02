#ifndef SCAENA_SINGLE_ACT_PLAY_H
#define SCAENA_SINGLE_ACT_PLAY_H

#include "AbstractPlay.h"


namespace scaena
{
    class SCAENA_EXPORT SingleActPlay : public AbstractPlay
    {
    protected:
        SingleActPlay(const std::string& id);

        // Interface to implement
        virtual void loadExternalRessources() =0;
        virtual void setUpPersistentCharacters() =0;

        // Now implemented
        virtual void setUpActs();

    public:
        virtual ~SingleActPlay();

        // Acts
        virtual bool addActAtPosition(const std::shared_ptr<AbstractAct>& act, int position);
        virtual bool appendAct(const std::shared_ptr<AbstractAct>& act);
        virtual bool deleteAct(const std::string& id);
    };
}

#endif // SCAENA_SINGLE_ACT_PLAY_H
