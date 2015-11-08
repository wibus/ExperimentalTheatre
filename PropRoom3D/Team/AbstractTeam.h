#ifndef PROPROOM3D_ABSTRACTTEAM_H
#define PROPROOM3D_ABSTRACTTEAM_H

#include <vector>
#include <memory>

#include <GLM/glm.hpp>

#include "../libPropRoom3D_global.h"


namespace cellar
{
    class Camera;
}

namespace prop3
{
    class Prop;
    class StageSet;
    class Environment;
    class AbstractArtDirector;
    class AbstractChoreographer;


    class PROP3D_EXPORT AbstractTeam
    {
    private:
        // Removed
        AbstractTeam(const AbstractTeam&) = delete;
        AbstractTeam& operator=(const AbstractTeam&) = delete;


    protected:
        AbstractTeam(AbstractChoreographer* choreographer);


    public:
        virtual ~AbstractTeam();

        virtual void setup();
        virtual void reset();
        virtual void update(double dt);

        // StageSet
        virtual std::shared_ptr<StageSet> stageSet() const;
        virtual bool saveScene(const std::string& fileName, bool prettyPrint = true);
        virtual bool loadScene(const std::string& fileName);

        // Team accessors/modifiers
        virtual void addArtDirector(
            const std::shared_ptr<AbstractArtDirector>& artDirector);


    private:
        // Team members
        std::shared_ptr<StageSet> _stageSet;
        std::unique_ptr<AbstractChoreographer> _choreographer;
        std::vector<std::shared_ptr<AbstractArtDirector>> _artDirectors;
    };



    // IMPLEMENTATION //
    inline std::shared_ptr<StageSet> AbstractTeam::stageSet() const
    {
        return _stageSet;
    }
}

#endif // PROPROOM3D_ABSTRACTTEAM_H
