#ifndef PROPROOM3D_SCENENODE_H
#define PROPROOM3D_SCENENODE_H

#include <vector>
#include <memory>

#include "PropRoom3D/libPropRoom3D_global.h"


namespace prop3
{
    class SceneVisitor;


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


    class PROP3D_EXPORT SceneNode
    {
    protected:
        SceneNode();

    public:
        virtual ~SceneNode();

        virtual void accept(SceneVisitor& visitor) = 0;

        virtual std::vector<std::shared_ptr<SceneNode>> children() const;

        TimeStamp timeStamp() const;


    protected:
        void stampCurrentUpdate();


    private:
        TimeStamp _timeStamp;
    };



    // IMPLEMENTATION //
    inline TimeStamp SceneNode::timeStamp() const
    {
        return _timeStamp;
    }
}

#endif // PROPROOM3D_SCENENODE_H
