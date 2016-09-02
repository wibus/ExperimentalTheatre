#ifndef PROPROOM2D_RADIANCEMACHINE_H
#define PROPROOM2D_RADIANCEMACHINE_H

#include <vector>

#include <PropRoom2D/libPropRoom2D_global.h>

namespace prop2
{
    class Mesh;
    class LightPath;


    class PROP2D_EXPORT RadianceMachine
    {
    public:
        RadianceMachine();
        ~RadianceMachine();

        void reset(const Mesh& mesh);

        void tracePaths(std::vector<LightPath>& paths);

    private:
    };
}

#endif // PROPROOM2D_RADIANCEMACHINE_H
