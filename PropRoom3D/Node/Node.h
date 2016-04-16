#ifndef PROPROOM3D_NODE_H
#define PROPROOM3D_NODE_H

#include <vector>
#include <memory>

#include <CellarWorkbench/DesignPattern/SpecificSubject.h>
#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

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


    class PROP3D_EXPORT Node : public cellar::SpecificSubject<TimeStamp>,
                               public cellar::SpecificObserver<TimeStamp>
    {
    protected:
        Node();

    public:
        virtual ~Node();

        virtual void accept(Visitor& visitor) = 0;

        virtual std::vector<std::shared_ptr<Node>> children() const;

        TimeStamp timeStamp() const;

        virtual void notify(TimeStamp& childStamp) override;

    protected:
        void blockUpdates();
        void unblockUpdates();
        void stampCurrentUpdate();
        void registerTo(const std::shared_ptr<Node>& child);
        void unregisterFrom(const std::shared_ptr<Node>& child);
        void swapChild(const std::shared_ptr<Node>& oldChild,
                       const std::shared_ptr<Node>& newChild);


    private:
        TimeStamp _timeStamp;
        bool _blockUpdates;
    };



    // IMPLEMENTATION //
    inline TimeStamp Node::timeStamp() const
    {
        return _timeStamp;
    }
}

#endif // PROPROOM3D_NODE_H
