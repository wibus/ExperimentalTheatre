#ifndef PROPROOM3D_STAGESETNODE_H
#define PROPROOM3D_STAGESETNODE_H

#include <vector>
#include <memory>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class StageSetVisitor;


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


    class PROP3D_EXPORT StageSetNode
    {
    protected:
        StageSetNode();

    public:
        virtual ~StageSetNode();

        virtual void accept(StageSetVisitor& visitor) = 0;

        virtual std::vector<std::shared_ptr<StageSetNode>> children() const;

        TimeStamp timeStamp() const;


    protected:
        void stampCurrentUpdate();


    private:
        TimeStamp _timeStamp;
    };



    // IMPLEMENTATION //
    inline TimeStamp StageSetNode::timeStamp() const
    {
        return _timeStamp;
    }
}

#endif // PROPROOM3D_STAGESETNODE_H
