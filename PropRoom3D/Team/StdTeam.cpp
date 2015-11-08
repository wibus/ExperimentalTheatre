#include "StdTeam.h"

#include "ArtDirector/ArtDirectorServer.h"
#include "Choreographer/StdChoreographer.h"

namespace prop3
{
    StdTeam::StdTeam() :
        AbstractTeam(new StdChoreographer())
    {

    }

    StdTeam::~StdTeam()
    {

    }
}
