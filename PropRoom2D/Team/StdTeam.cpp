#include "StdTeam.h"

#include "ArtDirector/GlArtDirector.h"
#include "Choreographer/StdChoreographer.h"
#include "Designer/StdDesigner.h"


namespace prop2
{
    StdTeam::StdTeam() :
        AbstractTeam(new StdDesigner(),
                     new StdChoreographer())
    {
    }

    StdTeam::~StdTeam()
    {
    }
}
