#ifndef PROPROOM3D_STAGESET_H
#define PROPROOM3D_STAGESET_H

#include <string>
#include <vector>
#include <memory>

#include "StageZone.h"


namespace prop3
{
    class Environment;
    class Visitor;


    class PROP3D_EXPORT StageSet : public StageZone
    {
    public :
        StageSet();
        virtual ~StageSet();


        // Node interface
        virtual void accept(Visitor& visitor) override;

        virtual std::vector<std::shared_ptr<Node>> children() const;


        // Environment
        std::shared_ptr<Environment> environment() const;

        virtual void setEnvironment(const std::shared_ptr<Environment>& env);


        // Tree travelling
        virtual void makeTraveling(Visitor& visitor);


        // Timestamps
        bool updateTimeStamp();

        bool stageSetChanged() const;


    private:
        std::shared_ptr<Environment> _environment;
        TimeStamp _lastTimeStamp;
        bool _stageSetChanged;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<Environment> StageSet::environment() const
    {
        return _environment;
    }

    inline bool StageSet::stageSetChanged() const
    {
        return _stageSetChanged;
    }
}

#endif // PROPROOM3D_STAGESET_H
