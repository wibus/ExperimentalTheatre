#include "SingleActPlay.h"

#include "Act/AbstractAct.h"
#include "Character/AbstractCharacter.h"

using namespace std;


namespace scaena
{
    class SingletonAct : public AbstractAct
    {
    public:
        SingletonAct(AbstractPlay& play) : AbstractAct(play, "SingletonAct") {}
        virtual void installStageSet() {}
    };


    SingleActPlay::SingleActPlay(const string& id) :
        AbstractPlay(id)
    {
    }

    SingleActPlay::~SingleActPlay()
    {}

    void SingleActPlay::setUpActs()
    {
        shared_ptr<AbstractAct> singletonAct( new SingletonAct(*this) );

        for(CharIt cIt = _persitentCharacters.begin();
            cIt != _persitentCharacters.end();
            ++cIt)
        {
            singletonAct->addCharacter(*cIt);
        }

        _acts.push_back(singletonAct);
    }

    bool SingleActPlay::addActAtPosition(const std::shared_ptr<AbstractAct>&, int)
    {
        return false;
    }

    bool SingleActPlay::appendAct(const std::shared_ptr<AbstractAct>&)
    {
        return false;
    }

    bool SingleActPlay::deleteAct(const std::string&)
    {
        return false;
    }
}
