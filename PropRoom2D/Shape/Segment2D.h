#ifndef MEDIAWORKBENCH_LINE2D_H
#define MEDIAWORKBENCH_LINE2D_H

#include <GLM/glm.hpp>

#include "../libPropRoom2D_global.h"


namespace prop2
{
    class PROP2D_EXPORT Segment2D
    {
    public:
        Segment2D();
        Segment2D(const Segment2D& line);
        Segment2D(double xbegin, double ybegin, double xend, double yend);
        Segment2D(const glm::dvec2& begin, const glm::dvec2& end);

        double xbegin() const;
        double ybegin() const;
        double xend() const;
        double yend() const;
        glm::dvec2 begin() const;
        glm::dvec2 end() const;
        glm::dvec2 unitDirector() const;
        glm::dvec2 unitNormal() const;
        double length() const;

        void setBegin(double x, double y);
        void setBegin(const glm::dvec2& pos);
        void setEnd(double x, double y);
        void setEnd(const glm::dvec2& pos);
        void moveBy(const glm::dvec2& ds);

        signed char pointLateralPosition(const glm::dvec2& point) const;
        signed char pointTransversalPosition(const glm::dvec2& point) const;
        double           pointMinimalDistance(const glm::dvec2& point) const;
        glm::dvec2 pointMinimalDirection(const glm::dvec2& point) const;
        bool        intersects(const Segment2D& line) const;
        glm::dvec2 intersectionPoint(const Segment2D &line) const; // Vec(0, 0) if not intersecting

        static double cross(const glm::dvec2& u, const glm::dvec2& v);
        static glm::dvec2 perpCCW(const glm::dvec2& vec);
        static glm::dvec2 perpCW(const glm::dvec2& vec);


    private:

        glm::dvec2 _begin;
        glm::dvec2 _end;
    };



    // IMPLEMENTATION //
    inline double Segment2D::xbegin() const
    {
        return _begin.x;
    }

    inline double Segment2D::ybegin() const
    {
        return _begin.y;
    }

    inline double Segment2D::xend() const
    {
        return _end.x;
    }

    inline double Segment2D::yend() const
    {
        return _end.y;
    }

    inline glm::dvec2 Segment2D::begin() const
    {
        return _begin;
    }

    inline glm::dvec2 Segment2D::end() const
    {
        return _end;
    }

    inline glm::dvec2 Segment2D::unitDirector() const
    {
        return glm::normalize(_end - _begin);
    }

    inline glm::dvec2 Segment2D::unitNormal() const
    {
        glm::dvec2 dir = unitDirector();
        return glm::dvec2(-dir.y, dir.x);
    }

    inline double Segment2D::length() const
    {
        return glm::length(_end - _begin);
    }

    inline void Segment2D::setBegin(double x, double y)
    {
        _begin = glm::dvec2(x, y);
    }

    inline void Segment2D::setBegin(const glm::dvec2& pos)
    {
        _begin = pos;
    }

    inline void Segment2D::setEnd(double x, double y)
    {
        _end = glm::dvec2(x, y);
    }

    inline void Segment2D::setEnd(const glm::dvec2& pos)
    {
        _end = pos;
    }

    inline void Segment2D::moveBy(const glm::dvec2& ds)
    {
        _begin += ds;
        _end   += ds;
    }

    inline double Segment2D::cross(const glm::dvec2& u, const glm::dvec2& v)
    {
        return u.x * v.y - u.y * v.x;
    }

    inline glm::dvec2 Segment2D::perpCW(const glm::dvec2& vec)
    {
        return glm::dvec2(vec[1], -vec[0]);
    }

    inline glm::dvec2 Segment2D::perpCCW(const glm::dvec2& vec)
    {
        return glm::dvec2(-vec[1], vec[0]);
    }
}

#endif // MEDIAWORKBENCH_LINE2D_H
