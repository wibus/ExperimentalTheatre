#include "ArtDirectorDummy.h"

#include <GL3/gl3w.h>


namespace prop3
{
    ArtDirectorDummy::ArtDirectorDummy()
    {
    }

    ArtDirectorDummy::~ArtDirectorDummy()
    {
    }

    void ArtDirectorDummy::setup(const std::shared_ptr<StageSet>& stageSet)
    {
        camera()->refresh();
    }

    void ArtDirectorDummy::update(double dt)
    {
    }

    void ArtDirectorDummy::draw(double dt)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ArtDirectorDummy::terminate()
    {
    }

    void ArtDirectorDummy::notify(cellar::CameraMsg &msg)
    {

    }
}
