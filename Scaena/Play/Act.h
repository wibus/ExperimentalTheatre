#ifndef SCAENA_ACT_H
#define SCAENA_ACT_H

#include <string>
#include <list>
#include <memory>

#include "../libScaena_global.h"


namespace scaena
{
    class Play;
    class Character;
    class StageManager;


    class SCAENA_EXPORT Act
    {
    public:
        Act(Play& play, const std::string& id);
        virtual ~Act();

        // Open close
        virtual void openCurtain();
        virtual void closeCurtain();
        virtual void restart();

        // Characters
        virtual std::shared_ptr<Character> getCharacter(const std::string& id) const;
        virtual bool addCharacter(const std::shared_ptr<Character>& character);
        virtual bool deleteCharacter(const std::string& id);

        // Attributs getters
        Play& play() const;
        const std::string id() const;
        bool isPlaying() const;
        int position() const;

        // Attributs setters
        void setPosition(int pos);

        // Visitors
        void welcome(StageManager& manager);

        // Operators
        bool operator< (const Act& act) const;
        bool operator== (const Act& act) const;


    protected :
        Play& _play;
        std::string _id;
        int _position;
        bool _isPlaying;
        std::list< std::shared_ptr<Character> > _characters;
        typedef std::list< std::shared_ptr<Character> >::iterator CharIterator;
    };



    // IMPLEMENTATION //
    inline const std::string Act::id() const
        {return _id;}

    inline Play& Act::play() const
        {return _play;}

    inline bool Act::isPlaying() const
        {return _isPlaying;}

    inline int Act::position() const
        {return _position;}

    inline void Act::setPosition(int pos)
        {_position = pos;}

    inline bool Act::operator< (const Act& act) const
        {return _position < act._position;}

    inline bool Act::operator== (const Act& act) const
        {return _id == act._id;}
}

#endif // SCAENA_ACT_H
