#ifndef CELLARWORKBENCH_LOG_H
#define CELLARWORKBENCH_LOG_H

#include <string>
#include <vector>

#include "../libCellarWorkbench_global.h"
#include "../DesignPattern/Singleton.h"


namespace cellar
{
    class CELLAR_EXPORT Message
    {
    public :
        Message(char type,bool isFatal,
                const std::string& text,
                const std::string& addresser);

        char                type()        const;
        bool                isFatal()     const;
        const std::string   text()        const;
        const std::string   addresser()   const;

    private :
        char _type;
        bool _isFatal;
        std::string _text;
        std::string _addresser;
    };


    class CELLAR_EXPORT Log : public Singleton< Log >
    {
            friend class Singleton< Log >;

    public :
        std::ostream& getOutput() const;
        void setOuput(std::ostream& out);

        void postMessage(Message* message);
        const Message* getLastMessage() const;

    private :
        Log();
        ~Log();
        void showAllMessages();
        void outputMessage(std::ostream& os,
                           const Message* msg);

        std::string _appPath;
        std::ostream* _out;
        std::vector<Message*> _logMessages;
    };

    CELLAR_EXPORT Log& getLog();



    // IMPLEMENTATION //
    inline char Message::type() const
        {return _type;}

    inline bool Message::isFatal() const
        {return _isFatal;}

    inline const std::string Message::text() const
        {return _text;}

    inline const std::string Message::addresser() const
        {return _addresser;}
}
#endif // CELLARWORKBENCH_LOG_H
