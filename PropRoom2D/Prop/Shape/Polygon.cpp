#include "Polygon.h"

#include <cassert>
using namespace std;

#include <GLM/gtc/constants.hpp>


using namespace cellar;

#include "../Costume/PolygonCostume.h"
#include "../Hardware/Hardware.h"


namespace prop2
{
    Polygon::Polygon(int nbVertices) :
        AbstractShape(EPropType::POLYGON),
        _costume(),
        _relVertices(nbVertices),
        _outline(nbVertices),
        _isConcave(false)
    {
    }

    Polygon::~Polygon()
    {
    }

    const AbstractCostume& Polygon::abstractCostume() const
    {
        return *_costume;
    }

    const std::shared_ptr<PolygonCostume>& Polygon::costume() const
    {
        return _costume;
    }

    void Polygon::setCostume(const std::shared_ptr<PolygonCostume>& costume)
    {
        assert(costume->verticesColors().size() == (size_t)nbVertices());
        _costume = costume;
    }

    int Polygon::nbVertices() const
    {
        return static_cast<int>(_relVertices.size());
    }

    const std::vector<glm::dvec2>& Polygon::relVertices() const
    {
        return _relVertices;
    }

    const std::vector<Segment2D>& Polygon::outline() const
    {
        return _outline;
    }

    void Polygon::setVertices(const std::vector<glm::dvec2>& absolutePositions)
    {
        assert(absolutePositions.size() == (size_t)nbVertices());

        glm::dvec2 proposedCentroid = evaluateCentroid(absolutePositions);

        // Move vertices' centroid to (0,0)
        int nbv = nbVertices();
        for(int i=0; i<nbv; ++i)
        {
            _relVertices[i] = absolutePositions[i] - proposedCentroid;
        }

        // Test for concaveness
        _isConcave = false;
        for(int i=0; i<nbv; ++i)
        {
            glm::dvec2 dir = _relVertices[(i+1)%nbv] - _relVertices[i];
            glm::dvec2 nextDir = _relVertices[(i+2)%nbv] - _relVertices[(i+1)%nbv];
            glm::dvec2 nextNormal = glm::dvec2(-nextDir.y, nextDir.x);
            if(dot(dir, nextNormal) < double(0))
            {
                _isConcave = true;
                break;
            }
        }

        // Update cached attributes
        _angle = double(0);
        _centroid = proposedCentroid;
        updateTransformMatrix();
        updateInertia();
    }

    void Polygon::moveVertexAt(int cIdx, const glm::dvec2& position)
    {
        assert(0 <= cIdx && cIdx < nbVertices());
        moveBy(position - _outline[cIdx].begin());
    }

    int getQuadrant(const glm::dvec2& origin, const glm::dvec2& pt)
    {
        return pt.x < origin.x ?
                    pt.y < origin.y ?
                        3 :
                        2 :
                    pt.y < origin.y ?
                        4 :
                        1;
    }

    bool Polygon::contains(const glm::dvec2& point) const
    {
        int movesSum = 0;
        int lastQuadrant = getQuadrant(_outline[0].end(), point);

        int nbv = nbVertices();
        int first = 1, last = nbv+1;
        for(int i=first; i < last; ++i)
        {
            int idx = i % nbv;
            int currentQuadrant = getQuadrant(_outline[idx].end(), point);
            int delta = currentQuadrant - lastQuadrant;

            switch(delta)
            {
            case  3 : delta = -1; break;
            case -3 : delta =  1; break;

            case  2 :
            case -2 :
                delta = 2 * _outline[idx].pointLateralPosition(point);
                break;
            }

            movesSum += delta;
            lastQuadrant = currentQuadrant;
        }

        return movesSum == 4;
    }

    glm::dvec2 Polygon::nearestSurface(const glm::dvec2& point) const
    {
        glm::dvec2 minDist = _outline[0].pointMinimalDirection(point);
        double minDistLen2 = glm::dot(minDist, minDist);

        int nbv = nbVertices();
        for(int i=1; i < nbv; ++i)
        {
            glm::dvec2 dist = _outline[i].pointMinimalDirection(point);
            double distLen2 = glm::dot(dist, dist);
            if(distLen2 < minDistLen2)
            {
                minDist = dist;
                minDistLen2 = distLen2;
            }
        }

        return -minDist;
    }

    double Polygon::computeArea() const
    {

        double diagonal1 = double(0.0);
        double diagonal2 = double(0.0);

        int nbSides = nbVertices();
        for(size_t i=0; i < nbSides; ++i)
        {
            const Segment2D& line = _outline[i];
            diagonal1 += line.begin().x * line.end().y;
            diagonal2 += line.begin().y * line.end().x;
        }

        return glm::abs(diagonal1 - diagonal2) / double(2.0);
    }

    bool Polygon::isConcave() const
    {
        return _isConcave;
    }

    std::vector<glm::vec2> Polygon::rectangle(double width, double height)
    {
        std::vector<glm::vec2> vertices;
        vertices.push_back(glm::dvec2(-width/2.0, -height/2.0));
        vertices.push_back(glm::dvec2( width/2.0, -height/2.0));
        vertices.push_back(glm::dvec2( width/2.0,  height/2.0));
        vertices.push_back(glm::dvec2(-width/2.0,  height/2.0));
        return vertices;
    }
    std::vector<glm::vec2> Polygon::regularPolygon(double radius, int nbSides)
    {
        std::vector<glm::vec2> vertices;
        for(int i=0; i<nbSides; ++i)
        {
            double angle = 2.0*i*glm::pi<double>()/nbSides;
            vertices.push_back(radius * glm::dvec2(cos(angle), sin(angle)));
        }
        return vertices;
    }

    void Polygon::updateTransformMatrix()
    {
        double c = glm::cos(_angle);
        double s = glm::sin(_angle);

        _tranformMatrix = glm::dmat3();
        _tranformMatrix[0][0] = c; _tranformMatrix[1][0] = -s; _tranformMatrix[2][0] = _centroid.x;
        _tranformMatrix[0][1] = s; _tranformMatrix[1][1] = c;  _tranformMatrix[2][1] = _centroid.y;
        _tranformMatrix[0][2] = 0; _tranformMatrix[1][2] = 0;  _tranformMatrix[2][2] = 1.0f;


        // Update outline
        int nbv = nbVertices();
        for(int i=0; i<nbv; ++i)
        {
            _outline[i].setBegin(glm::dvec2(_tranformMatrix *
                                 glm::dvec3(_relVertices[i], 1)));
        }
        for(int i=0; i<nbv; ++i)
        {
            _outline[i].setEnd(_outline[(i+1)%nbv].begin());
        }
    }

    void Polygon::updateInertia()
    {
        if(_material)
        {
            double density = _material->density();
            _inverseMass = double(1.0) / (density * computeArea());

            double Ix = double(0.0);
            double Iy = double(0.0);
            int nbv = nbVertices();
            for(int i=0; i < nbv; ++i)
            {
                glm::dvec2 pb = _relVertices[i];
                glm::dvec2 pe = _relVertices[(i+1)%nbv];
                double ai = pb.x*pe.y - pe.x*pb.y;
                Ix += ai * (pb.y*pb.y + pb.y*pe.y + pe.y*pe.y);
                Iy += ai * (pb.x*pb.x + pb.x*pe.x + pe.x*pe.x);
            }
            _inverseMomentOfInertia = 12 / ((Ix + Iy) * density);
        }
        else
        {
            _inverseMass = double(0);
            _inverseMomentOfInertia = double(0);
        }
    }

    glm::dvec2 Polygon::evaluateCentroid(const std::vector<glm::dvec2>& vertices)
    {
        double diagonal1 = double(0.0);
        double diagonal2 = double(0.0);
        glm::dvec2 center(double(0), double(0));

        int nbv = static_cast<int>(vertices.size());
        for(int i=0; i < nbv; ++i)
        {
            const glm::dvec2& begin = vertices[i];
            const glm::dvec2& end = vertices[(i+1)%nbv];
            double diag1 = begin.x * end.y;
            double diag2 = begin.y * end.x;
            double diagDiff = diag1 - diag2;
            center += (begin + end) * diagDiff;
            diagonal1 += diag1;
            diagonal2 += diag2;
        }

        double area = glm::abs(diagonal1 - diagonal2) / double(2.0);

        return center / (double(6.0) * area);
    }
}
