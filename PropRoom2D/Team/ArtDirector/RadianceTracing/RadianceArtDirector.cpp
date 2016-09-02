#include "RadianceArtDirector.h"

#include <GLM/gtc/constants.hpp>

#include <Triangle/triangle.h>

#include "Mesh/Mesh.h"

#include "LightPath.h"
#include "RenderMachine.h"
#include "RadianceMachine.h"

#include "Prop/Shape/Circle.h"
#include "Prop/Shape/Polygon.h"

namespace prop2
{
    const int RadianceArtDirector::OUT_BOUND_ID = 1;
    const int RadianceArtDirector::FIRST_IN_BOUND_ID = 2;
    const glm::vec2 RadianceArtDirector::SCENE_BOUNDS_EPSILON = glm::vec2(1, 1);

    RadianceArtDirector::RadianceArtDirector() :
        _mesh(),
        _renderMachine(),
        _radianceMachine(),
        _meshWasTouched(false),
        _clearIrradiance(false),
        _renderMode(ERenderMode::WIREFRAME)
    {

    }

    RadianceArtDirector::~RadianceArtDirector()
    {

    }

    void RadianceArtDirector::notify(cellar::CameraMsg& msg)
    {
    }

    void RadianceArtDirector::setup()
    {
        _renderMachine.reset(new RenderMachine());

        _radianceMachine.reset(new RadianceMachine());
    }

    void RadianceArtDirector::reset()
    {
    }

    void RadianceArtDirector::draw(double dt)
    {
        if(_mesh.get() != nullptr)
        {
            if(_meshWasTouched)
            {
                _radianceMachine->reset(*_mesh);
                _renderMachine->reset(*_mesh);
                _meshWasTouched = false;
            }

            if(_clearIrradiance)
            {
                _renderMachine->clearIrradiance();
                _clearIrradiance = false;
            }

            if(_radianceMachine.get() != nullptr)
            {
                std::vector<LightPath> paths;

                _radianceMachine->tracePaths(paths);

                if(_renderMachine.get() != nullptr)
                {
                    _renderMachine->traceRadiance(paths);
                }
            }


            if(_renderMachine.get() != nullptr)
            {
                switch (_renderMode)
                {
                case ERenderMode::WIREFRAME :
                    _renderMachine->renderMeshWireframe();
                    break;

                case ERenderMode::IRRADIANCE :
                    _renderMachine->renderIrradiance();
                    break;

                case ERenderMode::OVERLAY :
                    // Draw wireframe over irradiance
                    _renderMachine->renderIrradiance();
                    _renderMachine->renderMeshWireframe();
                    break;

                default :
                    assert(false /* Unsupported render mode */);
                    break;
                }
            }
        }
    }

    void RadianceArtDirector::update(double dt)
    {
        // Clear mesh
        _mesh.reset(new Mesh());

        // Setup Scene
        genScenePslg(*_mesh);
        genMeshTriangulation(*_mesh);
        decorateMesh(*_mesh);

        // Notify GL Machine
        _meshWasTouched = true;
    }

    void RadianceArtDirector::setRenderMode(ERenderMode& renderMode)
    {
        _renderMode = renderMode;
    }

    void RadianceArtDirector::genScenePslg(Mesh &mesh)
    {
        // Planar Straight Line Graph generation strategy :
        // 1. Get world -> raster space transform from camera
        // 2. Segment polygons and circles in raster space
        // 3. Compute scene bounds including viewport
        // 4. Transform graph from raster to clip space
        //      This makes a nice [-1,1] coord range


        // Get camera transforms
        glm::dmat4 proj = camera()->projectionMatrix();
        glm::dmat4 invProjMat = glm::inverse(proj);
        glm::dmat4 view = camera()->viewMatrix();

        // Get minimum triangulation bounds
        glm::dvec4 viewMin = invProjMat * glm::dvec4(-1, -1, 0, 1);
        glm::dvec4 viewMax = invProjMat * glm::dvec4(+1, +1, 0, 1);

        // Compute scene bounding box (including viewport)
        glm::dvec2 sceneMin = glm::dvec2(viewMin) / viewMin.w;
        glm::dvec2 sceneMax = glm::dvec2(viewMax) / viewMax.w;

        std::vector<std::shared_ptr<AbstractProp>> props;

        for(std::shared_ptr<Circle> circle : _circles)
        {
            double radius = circle->radius();
            double viewRadius = (view * glm::dvec4(radius)).x;
            int vertCount = glm::min(6, int(viewRadius * 3.0));
            int boundId = props.size() + FIRST_IN_BOUND_ID;

            // Add circle loop to edges
            int lastIdx = vertCount-1;
            for(int i=0; i < lastIdx; ++i)
            {
                mesh.bounds.push_back(Bound(
                    mesh.interfaces.size(),
                    mesh.edges.size()));

                mesh.edges.push_back(Edge(
                    mesh.verts.size()+i,
                    mesh.verts.size()+i+1,
                    mesh.bounds.size()-1));
            }

            // Close the loop
            mesh.bounds.push_back(Bound(
                mesh.interfaces.size(),
                mesh.edges.size()));

            mesh.edges.push_back(Edge(
                mesh.verts.size() + lastIdx,
                mesh.verts.size() + 0,
                mesh.bounds.size()-1));

            mesh.interfaces.push_back(Interface(circle));


            // Add vertices
            glm::dvec2 viewCenter = glm::dvec2(
                view * glm::vec4(circle->center(), 0, 1.0));
            for(int i=0; i < vertCount; ++i)
            {
                double angle = i*2.0*glm::pi<double>() / vertCount;

                glm::dvec2 viewDir(glm::cos(angle), glm::sin(angle));
                glm::dvec2 viewPos = viewCenter + viewRadius * viewDir;

                sceneMin = glm::min(sceneMin, viewPos);
                sceneMax = glm::min(sceneMax, viewPos);
                mesh.verts.push_back(Vert(viewPos, viewDir));
            }

            props.push_back(circle);
        }

        for(std::shared_ptr<Polygon> polygon : _polygons)
        {
            int boundId = props.size() + FIRST_IN_BOUND_ID;

            // Add polygon loop to edges
            int lastIdx = polygon->nbVertices()-1;
            for(int i=0; i < lastIdx; ++i)
            {
                mesh.bounds.push_back(Bound(
                    mesh.interfaces.size(),
                    mesh.edges.size()));

                mesh.edges.push_back(Edge(
                    mesh.verts.size()+i,
                    mesh.verts.size()+i+1,
                    mesh.bounds.size()-1));
            }

            // Close the loop
            mesh.bounds.push_back(Bound(
                mesh.interfaces.size(),
                mesh.edges.size()));

            mesh.edges.push_back(Edge(
                mesh.verts.size() + lastIdx,
                mesh.verts.size() + 0,
                mesh.bounds.size()-1));

            mesh.interfaces.push_back(Interface(polygon));


            // Add vertices
            glm::dmat3 transform = polygon->transformMatrix();
            for(glm::dvec2 v : polygon->relVertices())
            {
                glm::dvec2 pos(transform * glm::dvec3(v, 1.0));
                glm::dvec2 viewPos(view * glm::dvec4(pos, 0, 1));
                sceneMin = glm::min(sceneMin, viewPos);
                sceneMax = glm::min(sceneMax, viewPos);
                mesh.verts.push_back(Vert(viewPos));
            }

            props.push_back(polygon);
        }

        // Every scene vertices
        sceneMin -= SCENE_BOUNDS_EPSILON;
        sceneMax += SCENE_BOUNDS_EPSILON;
        int prevBoundsEdgeCount = mesh.verts.size();
        mesh.verts.push_back(glm::dvec2(sceneMin.x, sceneMin.y));
        mesh.verts.push_back(glm::dvec2(sceneMax.x, sceneMin.y));
        mesh.verts.push_back(glm::dvec2(sceneMax.x, sceneMax.y));
        mesh.verts.push_back(glm::dvec2(sceneMin.x, sceneMax.y));
        mesh.edges.push_back(Edge(prevBoundsEdgeCount, prevBoundsEdgeCount+1, OUT_BOUND_ID));
        mesh.edges.push_back(Edge(prevBoundsEdgeCount+1, prevBoundsEdgeCount+2, OUT_BOUND_ID));
        mesh.edges.push_back(Edge(prevBoundsEdgeCount+2, prevBoundsEdgeCount+3, OUT_BOUND_ID));
        mesh.edges.push_back(Edge(prevBoundsEdgeCount+3, prevBoundsEdgeCount, OUT_BOUND_ID));

        // Transform every vertex in clip space to get nice [-1,1] coord range
        for(Vert& v : mesh.verts)
        {
            glm::dvec4 vw = proj * glm::dvec4(v.p, 0, 1);
            v.p = glm::dvec2(vw) / vw.w;
        }
    }

    void RadianceArtDirector::genMeshTriangulation(Mesh& mesh)
    {
        triangulateio* triIn = new triangulateio;
        triangulateio* triOut = new triangulateio;

        meshToTriangle(*triIn, mesh);
        triangulate(nullptr, triIn, triOut, nullptr);
        triangleToMesh(mesh, *triOut);

        delete triIn;
        triIn = nullptr;

        delete triOut;
        triOut = nullptr;
    }

    void RadianceArtDirector::meshToTriangle(triangulateio& triangle, const Mesh& mesh)
    {

    }

    void RadianceArtDirector::triangleToMesh(Mesh& mesh, const triangulateio& triangle)
    {

    }

    void RadianceArtDirector::decorateMesh(Mesh& mesh)
    {

    }
}
