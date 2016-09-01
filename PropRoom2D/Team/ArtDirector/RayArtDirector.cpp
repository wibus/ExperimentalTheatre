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
        // Get camera transforms
        glm::mat4 projView = camera()->projectionMatrix() * camera()->viewMatrix();
        glm::mat4 invProjView = glm::inverse(projView);

        // Get minimum triangulation bounds
        glm::vec4 viewMin = invProjView * glm::vec4(-1, -1, 0, 1);
        glm::vec4 viewMax = invProjView * glm::vec4(+1, +1, 0, 1);

        // Compute scene bounding box (including viewport)
        glm::vec2 sceneMin = glm::vec2(viewMin) / viewMin.w;
        glm::vec2 sceneMax = glm::vec2(viewMax) / viewMax.w;


        std::vector<glm::ivec3> edges;
        std::vector<glm::vec2> vertices;
        std::vector<std::shared_ptr<AbstractProp>> props;

        for(std::shared_ptr<Circle> circle : _circles)
        {
            double radius = circle->radius();
            int vertCount = glm::min(6, int(radius * 3.0));
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
            for(int i=0; i < vertCount; ++i)
            {
                double angle = i*2.0*glm::pi<double>() / vertCount;

                glm::vec2 pos =
                    circle->center() +
                    radius * glm::dvec2(
                        glm::cos(angle),
                        glm::sin(angle));

                sceneMin = glm::min(sceneMin, pos);
                sceneMax = glm::min(sceneMax, pos);
                vertices.push_back(pos);
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
                glm::vec2 pos(transform * glm::dvec3(v, 1.0));
                sceneMin = glm::min(sceneMin, pos);
                sceneMax = glm::min(sceneMax, pos);
                vertices.push_back(pos);
            }

            props.push_back(polygon);
        }

        // Every scene vertices
        sceneMin -= SCENE_BOUNDS_EPSILON;
        sceneMax += SCENE_BOUNDS_EPSILON;
        int prevBoundsEdgeCount = vertices.size();
        vertices.push_back(glm::vec2(sceneMin.x, sceneMin.y));
        vertices.push_back(glm::vec2(sceneMax.x, sceneMin.y));
        vertices.push_back(glm::vec2(sceneMax.x, sceneMax.y));
        vertices.push_back(glm::vec2(sceneMin.x, sceneMax.y));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount, prevBoundsEdgeCount+1, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+1, prevBoundsEdgeCount+2, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+2, prevBoundsEdgeCount+3, OUT_BOUND_ID));
        edges.push_back(glm::ivec3(prevBoundsEdgeCount+3, prevBoundsEdgeCount, OUT_BOUND_ID));


        // Invoke Triangle to produce a quality triangulation of the scene
    }
}
