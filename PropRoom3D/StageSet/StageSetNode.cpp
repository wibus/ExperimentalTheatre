#include "StageSetNode.h"

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

    StageSetNode::StageSetNode()
    {
        stampCurrentUpdate();
    }

    StageSetNode::~StageSetNode()
    {

    }

    std::vector<std::shared_ptr<StageSetNode>> StageSetNode::children() const
    {
        return std::vector<std::shared_ptr<StageSetNode>>();
    }

    void StageSetNode::stampCurrentUpdate()
    {
        _timeStamp = TimeStamp::getCurrentTimeStamp();
    }
}
