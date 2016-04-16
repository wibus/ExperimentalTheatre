#include "Node.h"

#include <chrono>

#include <cmath>


namespace prop3
{
    TimeStamp::TimeStamp(Time time) :
        _time(time)
    {

    }

    TimeStamp::TimeStamp() :
        _time(getCurrentTime())
    {

    }

    bool TimeStamp::operator<(const TimeStamp& t2) const
    {
        return _time < t2._time;
    }

    TimeStamp TimeStamp::getCurrentTimeStamp()
    {
        return TimeStamp(getCurrentTime());
    }

    TimeStamp::Time TimeStamp::getCurrentTime()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }

    Node::Node() :
        _blockUpdates(false)
    {
        stampCurrentUpdate();
    }

    Node::~Node()
    {

    }

    std::vector<std::shared_ptr<Node>> Node::children() const
    {
        return std::vector<std::shared_ptr<Node>>();
    }

    void Node::notify(TimeStamp& childStamp)
    {
        stampCurrentUpdate();
    }

    void Node::blockUpdates()
    {
        _blockUpdates = true;
    }

    void Node::unblockUpdates()
    {
        _blockUpdates = false;
        stampCurrentUpdate();
    }

    void Node::stampCurrentUpdate()
    {
        if(!_blockUpdates)
        {
            _timeStamp = TimeStamp::getCurrentTimeStamp();
            notifyObservers(_timeStamp);
        }
    }

    void Node::registerTo(const std::shared_ptr<Node>& child)
    {
        if(child.get() != nullptr)
        {
            child->registerObserver(*this);
        }
    }

    void Node::unregisterFrom(const std::shared_ptr<Node>& child)
    {
        if(child.get() != nullptr)
        {
            child->unregisterObserver(*this);
        }
    }

    void Node::swapChild(const std::shared_ptr<Node>& oldChild,
                         const std::shared_ptr<Node>& newChild)
    {
        unregisterFrom(oldChild);
        registerTo(newChild);
    }
}
