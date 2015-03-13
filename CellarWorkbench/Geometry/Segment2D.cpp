#include "Segment2D.h"


namespace cellar
{
    Segment2D::Segment2D() :
        _begin(0, 0),
        _end(0, 0)
    {
    }

    Segment2D::Segment2D(const Segment2D& line) :
        _begin(line.begin()),
        _end(line.end())
    {
    }

    Segment2D::Segment2D(double xbegin, double ybegin, double xend, double yend) :
        _begin(xbegin, ybegin),
        _end(xend, yend)
    {
    }

    Segment2D::Segment2D(const glm::dvec2& begin, const glm::dvec2& end) :
        _begin(begin),
        _end(end)
    {
    }
    signed char Segment2D::pointLateralPosition(const glm::dvec2& point) const
    {
        return glm::sign(cross(_end - _begin, point - _begin));
    }

    signed char Segment2D::pointTransversalPosition(const glm::dvec2& point) const
    {
        auto director = (_end - _begin);
        auto len = glm::length(director);
        auto pointProjection = dot(point - _begin, glm::normalize(director));

        if(pointProjection < 0)
            return -1;
        else if(pointProjection > len)
            return 1;
        //else
        return 0;
    }

    double Segment2D::pointMinimalDistance(const glm::dvec2& point) const
    {
        switch (pointTransversalPosition(point))
        {
        case 1:
            return glm::length(point - _end);

        case -1:
            return glm::length(point - _begin);

        default:
            return absolute(dot(point - _begin, unitNormal()));
        }
    }

    glm::dvec2 Segment2D::pointMinimalDirection(const glm::dvec2& point) const
    {
        switch (pointTransversalPosition(point))
        {
        case 1:
            return point - _end;

        case -1:
            return point - _begin;

        default:
            glm::dvec2 un = unitNormal();
            return un * glm::dot(point - _begin, un);
        }
    }

    bool Segment2D::intersects(const Segment2D& line) const
    {
        glm::dvec2 vec1 = _end - _begin;
        glm::dvec2 vec2 = line._end - line._begin;

        double denum = cross(vec1, vec2);
        if(denum == 0.0f)
            return false;

        glm::dvec2 origDist = this->_begin - line._begin;

        double A = cross(vec2, origDist) / denum;
        if(!(0.0f <= A && A <= 1.0f))
            return false;

        double B = cross(vec1, origDist) / denum; // origDist and denum minus simplification
        if(!(0.0f <= B && B <= 1.0f))
            return false;

        return true;
    }

    glm::dvec2 Segment2D::intersectionPoint(const Segment2D& line) const
    {
        glm::dvec2 vec1 = _end      - _begin;
        glm::dvec2 vec2 = line._end - line._begin;
        double denum = cross(vec1, vec2);

        glm::dvec2 origDist = this->_begin - line._end;

        double A = cross(vec2, origDist) / denum;
        return _begin + vec1 * A;
    }
}
