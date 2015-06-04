#ifndef SCAENA_CHARACTER_H
#define SCAENA_CHARACTER_H

#include <string>
#include <memory>

#include "../libScaena_global.h"


namespace scaena
{
    class Act;
    class Play;
    class View;
    class StageTime;
    class MouseEvent;
    class KeyboardEvent;
    class StageManager;


    class SCAENA_EXPORT Character
    {
    protected: Character(const std::string& id,
                         float priority = 0.0);

    public:
        virtual ~Character();

        // Life cycle
        virtual void enterStage();
        virtual void beginStep(const StageTime& time);
        virtual void endStep(const StageTime& time);
        virtual void draw(const std::shared_ptr<View>& view, const StageTime& time);
        virtual void exitStage();

        // Events
        virtual bool keyPressEvent(const KeyboardEvent& event);
        virtual bool keyReleaseEvent(const KeyboardEvent& event);
        virtual bool mousePressEvent(const MouseEvent& event);
        virtual bool mouseReleaseEvent(const MouseEvent& event);
        virtual bool mouseMoveEvent(const MouseEvent& event);
        virtual bool mouseWheelEvent(const MouseEvent& event);

        // Visitor
        virtual void welcome(StageManager& manager);

        // Getters
        const std::string&  id()        const;
        float               priority()  const;
        bool                updates()   const;
        bool                visible()   const;

        // Setters
        void setUpdates(bool updates);
        void setVisible(bool visible);

        // Operators
        bool operator==(const Character& character) const;
        bool operator< (const Character& character) const;


    protected:
        Play& play() const;


    private :
        std::string _id;
        float _priority;

        bool _updates;
        bool _visible;
    };



    // IMPLEMENTATION //
    // Getters
    inline const std::string& Character::id() const
        {return _id;}

    inline float Character::priority() const
        {return _priority;}

    inline bool Character::updates() const
        {return _updates;}

    inline bool Character::visible() const
        {return _visible;}


    // Setters
    inline void Character::setUpdates(bool updates)
        {_updates = updates;}

    inline void Character::setVisible(bool visible)
        {_visible = visible;}


    // Operators
    inline bool Character::operator< (const Character& character) const
        {return (character._priority < _priority);}


}

#endif // SCAENA_CHARACTER_H
