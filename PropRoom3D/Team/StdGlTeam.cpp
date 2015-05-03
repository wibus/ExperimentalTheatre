#include "StdGlTeam.h"

#include "Designer/StdDesigner.h"
#include "ArtDirector/CpuRaytracerServer.h"
#include "Choreographer/StdChoreographer.h"

namespace prop3
{
    StdGlTeam::StdGlTeam() :
        AbstractTeam(new StdDesigner,
                     new CpuRaytracerServer(),
                     new StdChoreographer())
    {

    }

    StdGlTeam::~StdGlTeam()
    {

    }
}
