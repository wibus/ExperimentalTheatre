#include "Polygon.h"
#include "Costume/PolygonCostume.h"
#include "Material/Material.h"


#include <cassert>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;


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

    const std::vector<Vec2r>& Polygon::relVertices() const
    {
        return _relVertices;
    }

    const std::vector<Segment2Dr>& Polygon::outline() const
    {
        return _outline;
    }

    void Polygon::setVertices(const std::vector<Vec2r>& absolutePositions)
    {
        assert(absolutePositions.size() == (size_t)nbVertices());

        Vec2r proposedCentroid = evaluateCentroid(absolutePositions);

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
            Vec2r dir = _relVertices[(i+1)%nbv] - _relVertices[i];
            Vec2r nextDir = _relVertices[(i+2)%nbv] - _relVertices[(i+1)%nbv];
            Vec2r nextNormal = perpCCW(nextDir);
            if(dot(dir, nextNormal) < real(0))
            {
                _isConcave = true;
                break;
            }
        }

        // Update cached attributes
        _angle = real(0);
        _centroid = proposedCentroid;
        updateTransformMatrix();
        updateInertia();
    }

    void Polygon::moveVertexAt(int cIdx, const Vec2r& position)
    {
        assert(0 <= cIdx && cIdx < nbVertices());
        moveBy(position - _outline[cIdx].begin());
    }

    int getQuadrant(const Vec2r& origin, const Vec2r& pt)
    {
        return pt.x() < origin.x() ?
                    pt.y() < origin.y() ?
                        3 :
                        2 :
                    pt.y() < origin.y() ?
                        4 :
                        1;
    }

    bool Polygon::contains(const Vec2r& point) const
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

    Vec2r Polygon::nearestSurface(const Vec2r& point) const
    {
        Vec2r minDist = _outline[0].pointMinimalDirection(point);
        real minDistLen2 = minDist.length2();

        int nbv = nbVertices();
        for(int i=1; i < nbv; ++i)
        {
            Vec2r dist = _outline[i].pointMinimalDirection(point);
            real distLen2 = dist.length2();
            if(distLen2 < minDistLen2)
            {
                minDist = dist;
                minDistLen2 = distLen2;
            }
        }

        return -minDist;
    }

    real Polygon::computeArea() const
    {

        real diagonal1 = real(0.0);
        real diagonal2 = real(0.0);

        int nbSides = nbVertices();
        for(size_t i=0; i < nbSides; ++i)
        {
            const Segment2Dr& line = _outline[i];
            diagonal1 += line.begin().x() * line.end().y();
            diagonal2 += line.begin().y() * line.end().x();
        }

        return absolute(diagonal1 - diagonal2) / real(2.0);
    }

    bool Polygon::isConcave() const
    {
        return _isConcave;
    }

    std::vector<cellar::Vec2f> Polygon::rectangle(real width, real height)
    {
        std::vector<cellar::Vec2f> vertices;
        vertices.push_back(Vec2f(-width/real(2), -height/real(2)));
        vertices.push_back(Vec2f( width/real(2), -height/real(2)));
        vertices.push_back(Vec2f( width/real(2),  height/real(2)));
        vertices.push_back(Vec2f(-width/real(2),  height/real(2)));
        return vertices;
    }
    std::vector<cellar::Vec2f> Polygon::regularPolygon(real radius, int nbSides)
    {
        std::vector<cellar::Vec2f> vertices;
        for(int i=0; i<nbSides; ++i)
        {
            double angle = 2.0*i*cellar::PI/nbSides;
            vertices.push_back(radius * Vec2r(cos(angle), sin(angle)));
        }
        return vertices;
    }

    void Polygon::updateTransformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::translate(_centroid);
        _tranformMatrix *= cellar::rotate(_angle);

        // Update outline
        int nbv = nbVertices();
        for(int i=0; i<nbv; ++i)
        {
            _outline[i].setBegin(_tranformMatrix *
                                 Vec3r(_relVertices[i], real(1)));
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
            real density = _material->density();
            _inverseMass = real(1.0) / (density * computeArea());

            real Ix = real(0.0);
            real Iy = real(0.0);
            int nbv = nbVertices();
            for(int i=0; i < nbv; ++i)
            {
                Vec2r pb = _relVertices[i];
                Vec2r pe = _relVertices[(i+1)%nbv];
                real ai = pb.x()*pe.y() - pe.x()*pb.y();
                Ix += ai * (pb.y()*pb.y() + pb.y()*pe.y() + pe.y()*pe.y());
                Iy += ai * (pb.x()*pb.x() + pb.x()*pe.x() + pe.x()*pe.x());
            }
            _inverseMomentOfInertia = 12 / ((Ix + Iy) * density);
        }
        else
        {
            _inverseMass = real(0);
            _inverseMomentOfInertia = real(0);
        }
    }

    Vec2r Polygon::evaluateCentroid(const std::vector<Vec2r>& vertices)
    {
        real diagonal1 = real(0.0);
        real diagonal2 = real(0.0);
        Vec2r center(real(0), real(0));

        int nbv = static_cast<int>(vertices.size());
        for(int i=0; i < nbv; ++i)
        {
            const Vec2r& begin = vertices[i];
            const Vec2r& end = vertices[(i+1)%nbv];
            real diag1 = begin.x() * end.y();
            real diag2 = begin.y() * end.x();
            real diagDiff = diag1 - diag2;
            center += (begin + end) * diagDiff;
            diagonal1 += diag1;
            diagonal2 += diag2;
        }

        real area = absolute(diagonal1 - diagonal2) / real(2.0);

        return center / (real(6.0) * area);
    }
}
