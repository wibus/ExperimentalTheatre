#include "Character.h"

#include "../ScaenaApplication/Application.h"
#include "../StageManagement/Manager/StageManager.h"

using namespace std;


namespace scaena
{
    Character::Character(const string& id, float priority) :
            _id(id),
            _priority(priority),
            _updates(true),
            _visible(true)
    {}

    Character::~Character()
    {}

    void Character::enterStage()
    {}

    void Character::beginStep(const StageTime&)
    {}

    void Character::endStep(const StageTime&)
    {}

    void Character::draw(const std::shared_ptr<View>&, const StageTime&)
    {}

    void Character::exitStage()
    {}

    bool Character::keyPressEvent(const KeyboardEvent&)
    {
        return false;
    }

    bool Character::keyReleaseEvent(const KeyboardEvent&)
    {
        return false;
    }

    bool Character::mousePressEvent(const MouseEvent&)
    {
        return false;
    }

    bool Character::mouseReleaseEvent(const MouseEvent&)
    {
        return false;
    }

    bool Character::mouseMoveEvent(const MouseEvent&)
    {
        return false;
    }

    bool Character::mouseWheelEvent(const MouseEvent&)
    {
        return false;
    }

    void Character::welcome(StageManager& manager)
    {
        manager.visit(*this);
    }

    bool Character::operator==(const Character& character) const
    {
        if(_id == "" && character._id == "")
            return this == &character;      //if they are anonym characters
        else
            return _id == character._id;    //if one of them is named
    }

    Play& Character::play() const
    {
        return getApplication().play();
    }
}
