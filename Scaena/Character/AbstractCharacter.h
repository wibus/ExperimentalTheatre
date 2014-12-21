#ifndef SCAENA_ABTSRACT_CHARACTER_H
#define SCAENA_ABTSRACT_CHARACTER_H

#include <string>

#include "../libScaena_global.h"


namespace scaena
{
    class AbstractAct;
    class AbstractStage;
    class KeyboardEvent;
    class MouseEvent;
    class StageManager;
    class StageTime;


    class SCAENA_EXPORT AbstractCharacter
    {
    protected: AbstractCharacter(AbstractStage& stage,
                                 const std::string& id,
                                 float priority = 0.0);

    public:
        virtual ~AbstractCharacter();

        // Life cycle
        virtual void enterStage();
        virtual void beginStep(const StageTime& time);
        virtual void endStep(const StageTime& time);
        virtual void draw(const StageTime& time);
        virtual void exitStage();

        // Events
        virtual bool keyPressEvent(const KeyboardEvent& event);
        virtual bool keyReleaseEvent(const KeyboardEvent& event);
        virtual bool mousePressEvent(const MouseEvent& event);
        virtual bool mouseReleaseEvent(const MouseEvent& event);
        virtual bool mouseMoveEvent(const MouseEvent& event);

        // Visitor
        virtual void welcome(StageManager& manager);

        // Getters
        AbstractStage&      stage()     const;
        float               priority()  const;
        const std::string   id()        const;
        bool                updates()   const;
        bool                visible()   const;

        // Setters
        void setUpdates(bool updates);
        void setVisible(bool visible);

        // Operators
        bool operator==(const AbstractCharacter& character) const;
        bool operator< (const AbstractCharacter& character) const;


    private :
        AbstractStage& _stage;
        std::string _id;
        float _priority;

        bool _updates;
        bool _visible;
    };



    // IMPLEMENTATION //
    // Getters
    inline AbstractStage& AbstractCharacter::stage() const
        {return _stage;}

    inline float AbstractCharacter::priority() const
        {return _priority;}

    inline const std::string AbstractCharacter::id() const
        {return _id;}

    inline bool AbstractCharacter::updates() const
        {return _updates;}

    inline bool AbstractCharacter::visible() const
        {return _visible;}


    // Setters
    inline void AbstractCharacter::setUpdates(bool updates)
        {_updates = updates;}

    inline void AbstractCharacter::setVisible(bool visible)
        {_visible = visible;}


    // Operators
    inline bool AbstractCharacter::operator< (const AbstractCharacter& character) const
        {return (character._priority < _priority);}


}

#endif // SCAENA_ABTSRACT_CHARACTER_H
