#ifndef PROPROOM2D_RADIANCEARTDIRECTOR_H
#define PROPROOM2D_RADIANCEARTDIRECTOR_H

#include <vector>
#include <map>

#include <CellarWorkbench/GL/GlProgram.h>
#include <CellarWorkbench/GL/GlVao.h>

#include "../AbstractArtDirector.h"


// Triangle IO
struct triangulateio;


namespace prop2
{
    class Mesh;
    class RenderMachine;
    class RadianceMachine;


    class PROP2D_EXPORT RadianceArtDirector : public AbstractArtDirector
    {
    public:
        enum class ERenderMode {WIREFRAME, IRRADIANCE, OVERLAY};

        RadianceArtDirector();
        virtual ~RadianceArtDirector();

        virtual void notify(cellar::CameraMsg &msg);

        virtual void setup();
        virtual void reset();
        virtual void draw(double dt);
        virtual void update(double dt);

        ERenderMode renderMode() const;
        virtual void setRenderMode(ERenderMode& renderMode);

    protected:

        static const int OUT_BOUND_ID;
        static const int FIRST_IN_BOUND_ID;
        static const glm::vec2 SCENE_BOUNDS_EPSILON;

        virtual void genScenePslg(Mesh& mesh);

        virtual void genMeshTriangulation(Mesh& mesh);

        virtual void meshToTriangle(triangulateio& triangle, const Mesh& mesh);

        virtual void triangleToMesh(Mesh& mesh, const triangulateio& triangle);

        virtual void decorateMesh(Mesh& mesh);


    private:
        std::shared_ptr<Mesh> _mesh;
        std::shared_ptr<RenderMachine> _renderMachine;
        std::shared_ptr<RadianceMachine> _radianceMachine;

        bool _meshWasTouched;
        bool _clearIrradiance;
        ERenderMode _renderMode;
    };



    // IMPLEMENTATION //
    inline RadianceArtDirector::ERenderMode RadianceArtDirector::renderMode() const
    {
        return _renderMode;
    }
}

#endif // PROPROOM2D_RADIANCEARTDIRECTOR_H
