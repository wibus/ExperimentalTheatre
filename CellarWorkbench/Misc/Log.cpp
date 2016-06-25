#include "Log.h"

#include <fstream>
#include <iostream>

using namespace std;


namespace cellar
{
    Message::Message(char type,bool isFatal,
                     const std::string& text,
                     const std::string& addresser) :
        _type(type),
        _isFatal(isFatal),
        _text(text),
        _addresser(addresser)
    {}

    Log& getLog()
    {
        return *Log::getInstance();
    }

    Log::Log():
        _appPath(),
        _out(0x0),
        _logMessages()
    {
        _out = &std::cout;
    }

    Log::~Log()
    {
    }

    // Geters
    ostream& Log::getOutput() const
    {
        return *_out;
    }

    // Seters
    void Log::setOuput(ostream& out)
    {
        _out = &out;
        showAllMessages();
    }

    void Log::postMessage(Message* message)
    {
        outputMessage(*_out, message);
        _logMessages.push_back(message);
    }

    void Log::showAllMessages()
    {
        for(unsigned int m=0; m < _logMessages.size(); m++)
            outputMessage(*_out, _logMessages[m]);
    }

    void Log::outputMessage(
            std::ostream& os,
            const Message* msg)
    {
        os << msg->type() << " -> "
           << msg->addresser() << " \t"
           << msg->text() << endl;
    }

    const Message* Log::getLastMessage() const
    {
        return _logMessages.back();
    }
}
