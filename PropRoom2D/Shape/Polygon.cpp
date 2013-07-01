#include "Polygon.h"
#include "Costume/PolygonCostume.h"


#include <cassert>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;


namespace prop2
{
    Polygon::Polygon(int nbVertices) :
        AbstractShape(PropType::POLYGON),
        _costume(),
        _relVertices(nbVertices),
        _absVertices(nbVertices),
        _outline(nbVertices > 1 ? nbVertices : 0)
    {
    }

    Polygon::~Polygon()
    {
    }

    const AbstractCostume& Polygon::abstractCostume() const
    {
        return *_costume;
    }

    void Polygon::setVertices(const std::vector<Vec2r>& vertices)
    {
        assert(vertices.size() == _relVertices.size());

        Vec2r proposedCentroid = evaluateCentroid(vertices);

        // Move vertices' centroid to (0,0)
        for(size_t i=0; i<vertices.size(); ++i)
        {
            _relVertices[i] = vertices[i] - proposedCentroid;
        }

        // Update cached attributes
        _angle = 0.0;
        _centroid = proposedCentroid;
        updateTranformMatrix();

        updatePerimeter();
        updateArea();
        updateInertia();
    }

    void Polygon::moveVertexAt(int cIdx, const Vec2r& position)
    {
        assert(0 <= cIdx && cIdx < (int)_absVertices.size());
        moveBy(position - _absVertices[cIdx]);
    }

    void Polygon::setCostume(const std::shared_ptr<PolygonCostume>& costume)
    {
        assert(costume->verticesColors().size() == (size_t)nbVertices());
        _costume = costume;
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

    void Polygon::updateAbsVerticesAndOutline()
    {
        size_t nbVertices = _relVertices.size();
        for(size_t i=0; i < nbVertices; ++i)
        {
            _absVertices[i] =
                Vec2r(_tranformMatrix * Vec3r(_relVertices[i], real(1)));
        }

        _outline = evaluateOutline(_absVertices);
    }

    void Polygon::updateTranformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::translate(_centroid.x(), _centroid.y());
        _tranformMatrix *= cellar::rotate(_angle);

        updateAbsVerticesAndOutline();
    }

    void Polygon::updatePerimeter()
    {
        _perimeter = real(0.0);

        size_t nbvert = _outline.size();
        for(size_t i=0; i < nbvert; ++i)
        {
            _perimeter += static_cast<real>((_outline[i].end() - _outline[i].begin()).length());
        }
    }

    void Polygon::updateArea()
    {
        size_t nbSides = _outline.size();
        if(nbSides == 0) _area = real(0);

        real diagonal1 = real(0.0);
        real diagonal2 = real(0.0);

        for(size_t i=0; i < nbSides; ++i)
        {
            const Segment2Dr& line = _outline[i];
            diagonal1 += line.begin().x() * line.end().y();
            diagonal2 += line.begin().y() * line.end().x();
        }

        _area = absolute(diagonal1 - diagonal2) / real(2.0);
    }

    void Polygon::updateInertia()
    {
        if(_density == INFINITE_DENSITY ||
           _bodyType != BodyType::DYNAMIC)
        {
            _inverseMass = INFINITE_INERTIA;
            _inverseMomentOfInertia = INFINITE_INERTIA;
        }
        else
        {
            _inverseMass = real(1.0) / (_density * _area);

            real Ix = real(0.0);
            real Iy = real(0.0);
            int nbv = nbVertices();
            for(int i=0; i < nbv; ++i)
            {
                Vec2r pb = _outline[i].begin();
                Vec2r pe = _outline[i].end();
                real ai = pb.x()*pe.y() - pe.x()*pb.y();
                Ix += ai * (pb.y()*pb.y() + pb.y()*pe.y() + pe.y()*pe.y());
                Iy += ai * (pb.x()*pb.x() + pb.x()*pe.x() + pe.x()*pe.x());
            }

            _inverseMomentOfInertia = 12 / ((Ix + Iy) * _density);
        }
    }

    Vec2r Polygon::evaluateCentroid(const std::vector<Vec2r>& vertices)
    {
        size_t nbvert = vertices.size();
        if(nbvert == 0) return Vec2r(real(0.0), real(0.0));

        vector<Segment2Dr> outline = evaluateOutline(vertices);

        real diagonal1 = real(0.0);
        real diagonal2 = real(0.0);
        Vec2r center(real(0), real(0));

        for(size_t i=0; i < nbvert; ++i)
        {
            const Segment2Dr& line = outline[i];
            const Vec2r& begin = line.begin();
            const Vec2r& end = line.end();
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

    std::vector<Segment2Dr> Polygon::evaluateOutline(const std::vector<Vec2r>& vertices)
    {
        size_t nbVert = vertices.size();

        vector<Segment2Dr> lines;
        lines.reserve(nbVert);

        for(size_t i=0; i < nbVert-1; ++i)
        {
            lines.push_back(Segment2Dr(vertices[i], vertices[i+1]));
        }

        if(nbVert > 1)
        {
            lines.push_back(Segment2Dr(vertices.back(), vertices.front()));
        }

        return lines;
    }
}
