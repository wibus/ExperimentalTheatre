#include "StdGlPropTeam.h"

#include "StdPropDesigner.h"
#include "GlArtDirector.h"
#include "StdChoreographer.h"


namespace prop2
{
    StdGlPropTeam::StdGlPropTeam() :
        AbstractPropTeam(new StdPropDesigner(),
                         new GlArtDirector(),
                         new StdChoreographer())
    {
    }

    StdGlPropTeam::~StdGlPropTeam()
    {
    }
}
