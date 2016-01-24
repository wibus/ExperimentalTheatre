#include "StdTeam.h"

#include "ArtDirector/ArtDirectorServer.h"
#include "Choreographer/StdChoreographer.h"

namespace prop3
{
    StdTeam::StdTeam() :
        AbstractTeam(std::make_shared<StdChoreographer>())
    {

    }

    StdTeam::~StdTeam()
    {

    }
}
