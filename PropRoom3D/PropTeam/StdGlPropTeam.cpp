#include "StdGlPropTeam.h"
#include "StdPropDesigner.h"
#include "CpuRaytracer.h"
#include "StdChoreographer.h"

namespace prop3
{
    StdGlPropTeam::StdGlPropTeam() :
        AbstractPropTeam(new StdPropDesigner,
                         new CpuRaytracer(),
                         new StdChoreographer())
    {

    }

    StdGlPropTeam::~StdGlPropTeam()
    {

    }
}
