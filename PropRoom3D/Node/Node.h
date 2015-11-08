#ifndef PROPROOM3D_NODE_H
#define PROPROOM3D_NODE_H

#include <vector>
#include <memory>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Visitor;


    class PROP3D_EXPORT TimeStamp
    {
        typedef unsigned long long int Time;

        TimeStamp(Time time);

    public:
        TimeStamp();

        bool operator<(const TimeStamp& t2) const;

        static TimeStamp getCurrentTimeStamp();

    private:
        static Time getCurrentTime();

        Time _time;
    };


    class PROP3D_EXPORT Node
    {
    protected:
        Node();

    public:
        virtual ~Node();

        virtual void accept(Visitor& visitor) = 0;

        virtual std::vector<std::shared_ptr<Node>> children() const;

        TimeStamp timeStamp() const;


    protected:
        void stampCurrentUpdate();


    private:
        TimeStamp _timeStamp;
    };



    // IMPLEMENTATION //
    inline TimeStamp Node::timeStamp() const
    {
        return _timeStamp;
    }
}

#endif // PROPROOM3D_NODE_H
