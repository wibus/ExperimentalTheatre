#include "AbstractCharacter.h"

using namespace std;

#include "../Stage/Manager/StageManager.h"


namespace scaena
{
    AbstractCharacter::AbstractCharacter(
        AbstractStage& stage,
        const string& id,
        float priority) :
            _stage(stage),
            _id(id),
            _priority(priority),
            _updates(true),
            _visible(true)
    {}

    AbstractCharacter::~AbstractCharacter()
    {}

    void AbstractCharacter::enterStage()
    {}

    void AbstractCharacter::beginStep(const StageTime&)
    {}

    void AbstractCharacter::endStep(const StageTime&)
    {}

    void AbstractCharacter::draw(const StageTime&)
    {}

    void AbstractCharacter::exitStage()
    {}

    bool AbstractCharacter::keyPressEvent(const KeyboardEvent&)
    {
        return false;
    }

    bool AbstractCharacter::keyReleaseEvent(const KeyboardEvent&)
    {
        return false;
    }

    bool AbstractCharacter::mousePressEvent(const MouseEvent&)
    {
        return false;
    }

    bool AbstractCharacter::mouseReleaseEvent(const MouseEvent&)
    {
        return false;
    }

    bool AbstractCharacter::mouseMoveEvent(const MouseEvent&)
    {
        return false;
    }

    void AbstractCharacter::welcome(StageManager& manager)
    {
        manager.visit(*this);
    }

    inline bool AbstractCharacter::operator==(const AbstractCharacter& character) const
    {
        if(_id == "" && character._id == "")
            return this == &character;      //if they are anonym characters
        else
            return _id == character._id;    //if one of them is named
    }
}
