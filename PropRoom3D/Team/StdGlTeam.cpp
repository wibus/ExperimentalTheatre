#include "StdGlTeam.h"

#include "Designer/StdDesigner.h"
#include "ArtDirector/CpuRaytracer.h"
#include "Choreographer/StdChoreographer.h"

namespace prop3
{
    StdGlTeam::StdGlTeam() :
        AbstractTeam(new StdDesigner,
                         new CpuRaytracer(),
                         new StdChoreographer())
    {

    }

    StdGlTeam::~StdGlTeam()
    {

    }
}
