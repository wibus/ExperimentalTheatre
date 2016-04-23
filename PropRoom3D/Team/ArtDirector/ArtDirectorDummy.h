#ifndef PROPROOM3D_ARTDIRECTORDUMMY_H
#define PROPROOM3D_ARTDIRECTORDUMMY_H

#include "AbstractArtDirector.h"


namespace prop3
{
    class CpuRaytracerEngine;

    class PROP3D_EXPORT ArtDirectorDummy :
            public AbstractArtDirector
    {
    public:
        ArtDirectorDummy();
        virtual ~ArtDirectorDummy();

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) override;
        virtual void update(double dt) override;
        virtual void draw(double dt) override;
        virtual void terminate() override;

        virtual void notify(cellar::CameraMsg &msg) override;
    };
}

#endif // PROPROOM3D_ARTDIRECTORDUMMY_H
