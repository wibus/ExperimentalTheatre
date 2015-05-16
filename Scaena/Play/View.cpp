#include "View.h"

#include <PropRoom2D/Team/AbstractTeam.h>
#include <PropRoom2D/Team/ArtDirector/AbstractArtDirector.h>

#include <PropRoom3D/Team/AbstractTeam.h>
#include <PropRoom3D/Team/ArtDirector/AbstractArtDirector.h>

#include "Play.h"


namespace scaena
{
    View::View(const std::string& id) :
        _id(id)
    {
    }

    View::~View()
    {

    }
}
