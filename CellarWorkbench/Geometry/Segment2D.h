#ifndef CELLARWORKBENCH_LINE2D_H
#define CELLARWORKBENCH_LINE2D_H

#include "libCellarWorkbench_global.h"
#include "DataStructure/Vector.h"
#include "Misc/CellarUtils.h"


namespace cellar
{
    template<typename T>
    class Segment2D
    {
    public:
        Segment2D();
        template<typename T2> Segment2D(const Segment2D<T2>& line);
        Segment2D(T xbegin, T ybegin, T xend, T yend);
        Segment2D(const Vector<2, T>& begin, const Vector<2, T>& end);

        T xbegin() const;
        T ybegin() const;
        T xend() const;
        T yend() const;
        Vector<2, T> begin() const;
        Vector<2, T> end() const;
        Vector<2, T> unitDirector() const;
        Vector<2, T> unitNormal() const;
        T length() const;

        void setBegin(T x, T y);
        void setBegin(const Vector<2, T>& pos);
        void setEnd(T x, T y);
        void setEnd(const Vector<2, T>& pos);
        void moveBy(const Vector<2, T>& ds);


        enum Lateral     {LEFT,     RIGHT,          NB_LATERALS};
        enum Transversal {IN_FRONT, BESIDE, BEHIND, NB_TRANSVERSALS};

        Lateral     pointLateralPosition(const Vector<2, T>& point) const;
        Transversal pointTransversalPosition(const Vector<2, T>& point) const;
        T           pointMinimalDistance(const Vector<2, T>& point) const;
        Vector<2, T> pointMinimalDirection(const Vector<2, T>& point) const;
        bool        intersects(const Segment2D<T>& line) const;
        Vector<2, T> intersectionPoint(const Segment2D<T> &line) const; // Vec(0, 0) if not intersecting


    private:
        Vector<2, T> _begin;
        Vector<2, T> _end;
    };

    typedef Segment2D<int>    Line2Di;
    typedef Segment2D<float>  Line2Df;
    typedef Segment2D<double> Line2Dd;



    // IMPLEMENTATION //
    template<typename T>
    Segment2D<T>::Segment2D() :
        _begin(0, 0),
        _end(0, 0)
    {
    }


    template<typename T>
    template <typename T2>
    Segment2D<T>::Segment2D(const Segment2D<T2>& line) :
        _begin(line.begin()),
        _end(line.end())
    {
    }


    template<typename T>
    Segment2D<T>::Segment2D(T xbegin, T ybegin, T xend, T yend) :
        _begin(xbegin, ybegin),
        _end(xend, yend)
    {
    }


    template<typename T>
    Segment2D<T>::Segment2D(const Vector<2, T>& begin, const Vector<2, T>& end) :
        _begin(begin),
        _end(end)
    {
    }



    template<typename T>
    inline T Segment2D<T>::xbegin() const
    {
        return _begin.x();
    }

    template<typename T>
    inline T Segment2D<T>::ybegin() const
    {
        return _begin.y();
    }

    template<typename T>
    inline T Segment2D<T>::xend() const
    {
        return _end.x();
    }

    template<typename T>
    inline T Segment2D<T>::yend() const
    {
        return _end.y();
    }

    template<typename T>
    inline Vector<2, T> Segment2D<T>::begin() const
    {
        return _begin;
    }

    template<typename T>
    inline Vector<2, T> Segment2D<T>::end() const
    {
        return _end;
    }

    template<typename T>
    inline Vector<2, T> Segment2D<T>::unitDirector() const
    {
        return (_end - _begin).normalize();
    }

    template<typename T>
    inline Vector<2, T> Segment2D<T>::unitNormal() const
    {
        return perpCCW(unitDirector());
    }

    template<typename T>
    inline T Segment2D<T>::length() const
    {
        return (_end - _begin).length();
    }

    template<typename T>
    inline void Segment2D<T>::setBegin(T x, T y)
    {
        _begin(x, y);
    }

    template<typename T>
    inline void Segment2D<T>::setBegin(const Vector<2, T>& pos)
    {
        _begin = pos;
    }

    template<typename T>
    inline void Segment2D<T>::setEnd(T x, T y)
    {
        _end(x, y);
    }

    template<typename T>
    inline void Segment2D<T>::setEnd(const Vector<2, T>& pos)
    {
        _end = pos;
    }

    template<typename T>
    inline void Segment2D<T>::moveBy(const Vector<2, T>& ds)
    {
        _begin += ds;
        _end   += ds;
    }


    template<typename T>
    typename Segment2D<T>::Lateral Segment2D<T>::pointLateralPosition(const Vector<2, T>& point) const
    {
        T pointLineDist = (point-_begin) ^ (_end-_begin);

        if(pointLineDist >= 0)
            return LEFT;
        //else
            return RIGHT;
    }


    template<typename T>
    typename Segment2D<T>::Transversal Segment2D<T>::pointTransversalPosition(const Vector<2, T>& point) const
    {
        auto director = (_end - _begin);
        auto len = director.length();
        auto pointProjection = dot(point - _begin, director.normalize());

        if(pointProjection < 0)
            return BEHIND;
        else if(pointProjection > len)
            return IN_FRONT;
        //else
        return BESIDE;
    }


    template<typename T>
    T Segment2D<T>::pointMinimalDistance(const Vector<2, T>& point) const
    {
        switch (pointTransversalPosition(point))
        {
        case IN_FRONT:
            return (point - _end).length();

        case BEHIND:
            return (point - _begin).length();

        default:
            return absolute(cross(point - _begin, unitDirector()));
        }
    }


    template<typename T>
    Vector<2, T> Segment2D<T>::pointMinimalDirection(const Vector<2, T>& point) const
    {
        switch (pointTransversalPosition(point))
        {
        case IN_FRONT:
            return point - _end;

        case BEHIND:
            return point - _begin;

        default:
            return cross(point - _begin, unitDirector()) * unitNormal();
        }
    }


    template<typename T>
    bool Segment2D<T>::intersects(const Segment2D<T>& line) const
    {
        Vector<2, T> vec1 = _end - _begin;
        Vector<2, T> vec2 = line._end - line._begin;

        float denum = cross(vec1, vec2);
        if(denum == 0.0f)
            return false;

        Vector<2, T> origDist = this->_begin - line._begin;

        float A = cross(vec2, origDist) / denum;
        if(!(0.0f <= A && A <= 1.0f))
            return false;

        float B = cross(vec1, origDist) / denum; // origDist and denum minus simplification
        if(!(0.0f <= B && B <= 1.0f))
            return false;

        return true;
    }


    template<typename T>
    Vector<2, T> Segment2D<T>::intersectionPoint(const Segment2D<T>& line) const
    {
        Vector<2, T> vec1 = _end      - _begin;
        Vector<2, T> vec2 = line._end - line._begin;
        float denum = cross(vec1, vec2);

        Vector<2, T> origDist = this->_begin - line._end;

        float A = (vec2 ^ origDist) / denum;
        return _begin + A*vec1;
    }
}

#endif // CELLARWORKBENCH_LINE2D_H
