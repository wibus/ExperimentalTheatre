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
            _relVertices[i] =  vertices[i] - proposedCentroid;
        }

        // Update cached attributes
        updateAbsVerticesAndOutline();
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

    bool Polygon::contains(const Vec2r& point) const
    {
        bool oddNodes = false;

        size_t nbSides = nbVertices();

        for(size_t i=0, j=nbSides; i < nbSides; i++)
        {
            if ((_absVertices[i].y() < point.y() && _absVertices[j].y() >= point.y())
                    ||
                (_absVertices[j].y() < point.y() && _absVertices[i].y() >= point.y()))
            {
                if (_absVertices[i].x() +
                      (point.y() - _absVertices[i].y()) /
                      (_absVertices[j].y() - _absVertices[i].y()) *
                      (_absVertices[j].x() - _absVertices[i].x())
                    < point.x())
                {
                    oddNodes = !oddNodes;
                }
            }
            j = i;
        }

        return oddNodes;
    }

    bool Polygon::intersects(const Segment2Dr& line) const
    {
        size_t nbSides = _outline.size();

        for(size_t i=0; i < nbSides; ++i)
        {
            if(line.intersects(_outline[i]))
                return true;
        }

        return false;
    }

    void Polygon::updateAbsVerticesAndOutline()
    {
        size_t nbVertices = _relVertices.size();
        for(size_t i=0; i < nbVertices; ++i)
        {
            _absVertices[i] = Vec2r(_tranformMatrix * Vec3r(_relVertices[i],real(1)));
        }

        _outline = evaluateOutline(_absVertices);
    }

    void Polygon::updateTranformMatrix()
    {
        _tranformMatrix.loadIdentity();
        _tranformMatrix *= cellar::translate(_centroid.x(), _centroid.y());
        _tranformMatrix *= cellar::rotate(_angle);
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
        _mass = _density * _area;

        real meanR = real(0);
        for(int i=0; i<nbVertices(); ++i) meanR += static_cast<real>(_relVertices[i].length());
        meanR /= nbVertices();

        _momentOfInertia = real(0.5) * _mass * meanR * meanR;
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
