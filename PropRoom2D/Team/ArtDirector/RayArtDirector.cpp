#include "RayArtDirector.h"

#include <GLM/gtc/constants.hpp>

#include "Prop/Shape/Circle.h"
#include "Prop/Shape/Polygon.h"

namespace prop2
{
    const int RayArtDirector::OUT_BOUND_ID = 1;
    const int RayArtDirector::FIRST_IN_BOUND_ID = 2;
    const glm::vec2 RayArtDirector::SCENE_BOUNDS_EPSILON = glm::vec2(1, 1);


    RayArtDirector::RayArtDirector()
    {

    }

    RayArtDirector::~RayArtDirector()
    {

    }

    void RayArtDirector::notify(cellar::CameraMsg& msg)
    {
    }

    void RayArtDirector::setup()
    {

    }

    void RayArtDirector::reset()
    {
    }

    void RayArtDirector::draw(double dt)
    {

    }

    void RayArtDirector::update(double dt)
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


        std::vector<glm::ivec3> edges;
        std::vector<glm::dvec2> vertices;
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
                edges.push_back(glm::ivec3(
                    vertices.size()+i,
                    vertices.size()+i+1,
                    boundId));
            }

            // Close the loop
            edges.push_back(glm::ivec3(
                vertices.size() + lastIdx,
                vertices.size() + 0,
                boundId));


            // Add vertices
            glm::dvec2 viewCenter = glm::dvec2(
                view * glm::vec4(circle->center(), 0, 1.0));
            for(int i=0; i < vertCount; ++i)
            {
                double angle = i*2.0*glm::pi<double>() / vertCount;

                glm::dvec2 viewPos =
                    viewCenter +
                    viewRadius * glm::dvec2(
                        glm::cos(angle),
                        glm::sin(angle));

                sceneMin = glm::min(sceneMin, viewPos);
                sceneMax = glm::min(sceneMax, viewPos);
                vertices.push_back(viewPos);
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
                edges.push_back(glm::ivec3(
                    vertices.size()+i,
                    vertices.size()+i+1,
                    boundId));
            }

            // Close the loop
            edges.push_back(glm::ivec3(
                vertices.size() + lastIdx,
                vertices.size() + 0,
                boundId));


            // Add vertices
            glm::dmat3 transform = polygon->transformMatrix();
            for(glm::dvec2 v : polygon->relVertices())
            {
                glm::dvec2 pos(transform * glm::dvec3(v, 1.0));
                glm::dvec2 viewPos(view * glm::dvec4(pos, 0, 1));
                sceneMin = glm::min(sceneMin, viewPos);
                sceneMax = glm::min(sceneMax, viewPos);
                vertices.push_back(viewPos);
            }

            props.push_back(polygon);
        }

        // Every scene vertices
        sceneMin -= SCENE_BOUNDS_EPSILON;
        sceneMax += SCENE_BOUNDS_EPSILON;
        int prevBoundsEdgeCount = vertices.size();
        vertices.push_back(glm::dvec2(sceneMin.x, sceneMin.y));
        vertices.push_back(glm::dvec2(sceneMax.x, sceneMin.y));
        vertices.push_back(glm::dvec2(sceneMax.x, sceneMax.y));
        vertices.push_back(glm::dvec2(sceneMin.x, sceneMax.y));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount, prevBoundsEdgeCount+1, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+1, prevBoundsEdgeCount+2, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+2, prevBoundsEdgeCount+3, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+3, prevBoundsEdgeCount, OUT_BOUND_ID));

        // Transform every vertex in clip space to get nice [-1,1] coord range
        for(glm::dvec2& v : vertices)
        {
            glm::dvec4 vw = proj * glm::dvec4(v, 0, 1);
            v = glm::dvec2(vw) / vw.w;
        }


        // Invoke Triangle to produce a quality triangulation of the scene
    }
}
