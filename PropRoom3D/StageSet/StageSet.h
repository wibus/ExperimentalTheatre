#ifndef PROPROOM3D_STAGESET_H
#define PROPROOM3D_STAGESET_H

#include <string>
#include <vector>
#include <memory>

#include <PropRoom3D/Prop/Prop.h>
#include <PropRoom3D/Lighting/Environment.h>


namespace prop3
{
    class PROP3D_EXPORT StageSet
    {
    public :
        StageSet();
        virtual ~StageSet();


        virtual void makeTraveling(StageSetVisitor& visitor);


        virtual void addProp(const std::shared_ptr<Prop>& prop);

        virtual void removeProp(const std::shared_ptr<Prop>& prop);

        const std::vector<std::shared_ptr<Prop>>&  props();


        virtual void setEnvironment(const std::shared_ptr<Environment>& env);

        std::shared_ptr<Environment> environment() const;


        bool updateTimeStamp();

        TimeStamp timeStamp() const;

        bool stageSetChanged() const;


    private:
        std::vector<std::shared_ptr<Prop>> _props;
        std::shared_ptr<Environment> _environment;
        TimeStamp _timeStamp;
        bool _stageSetChanged;
    };



    // IMPLEMENTATION //
    inline const std::vector<std::shared_ptr<Prop>>& StageSet::props()
    {
        return _props;
    }

    inline std::shared_ptr<Environment> StageSet::environment() const
    {
        return _environment;
    }

    inline TimeStamp StageSet::timeStamp() const
    {
        return _timeStamp;
    }

    inline bool StageSet::stageSetChanged() const
    {
        return _stageSetChanged;
    }
}

#endif // PROPROOM3D_STAGESET_H
