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

    void View::setPostProdManager(const std::shared_ptr<prop3::PostProdManager>& manager)
    {
        _artDirector3D->setPostProdManager(manager);
    }

    void View::lookAt(const Play& play)
    {
        if(play.propTeam2D())
            play.propTeam2D()->addArtDirector(artDirector2D());

        if(play.propTeam3D())
            play.propTeam3D()->addArtDirector(artDirector3D());
    }

    glm::ivec2 View::viewport() const
    {
        return _artDirector2D->camera()->viewport();
    }

    std::shared_ptr<cellar::Camera> View::camera2D() const
    {
        return _artDirector2D->camera();
    }

    std::shared_ptr<cellar::Camera> View::camera3D() const
    {
        return _artDirector3D->camera();
    }
}
