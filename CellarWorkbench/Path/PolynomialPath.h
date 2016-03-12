#ifndef CELLAR_POLYNOMIALPATH_H
#define CELLAR_POLYNOMIALPATH_H

#include <vector>
#include <string>

#include <GLM/glm.hpp>

#include "AbstractPath.h"
#include "PathVisitor.h"


namespace cellar
{

    template<typename Data>
    class CELLAR_EXPORT PolynomialPath : public LeafPath<Data>
    {
    public:
        PolynomialPath(double duration,
                       const std::vector<Data>& ctrlPts,
                       const std::vector<double>& weights);
        virtual ~PolynomialPath();


        std::vector<Data>& ctrlPts();
        const std::vector<Data>& ctrlPts() const;

        std::vector<double>& weights();
        const std::vector<double>& weights() const;


        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;

        virtual void update() override;

        virtual std::string csv(double dt);

    private:
        enum class ENodePos {
            First,
            Second,
            Middle,
            BeforeLast,
            Last
        };

        struct CELLAR_EXPORT Node
        {
            double eval(double t) const;
            void buildFront(double center, double intervall, double weight, ENodePos pos);
            void buildDuring(double center, double intervall, double weight, ENodePos pos);
            void buildBack(double center, double intervall, double weight, ENodePos pos);
            // The index of the coeffs equals its degree
            glm::dvec3 _before;
            glm::dvec3 _during;
            glm::dvec3 _after;
            double _lowerBound;
            double _upperBound;
        };

        std::vector<double> _weights;
        std::vector<Data> _ctrlPts;
        std::vector<Node> _nodes;
    };



    // IMPLEMENTATION

    template<typename Data>
    PolynomialPath<Data>::PolynomialPath(
            double duration,
            const std::vector<Data>& ctrlPts,
            const std::vector<double>& weights) :
        LeafPath<Data>(duration),
        _weights(weights),
        _ctrlPts(ctrlPts)
    {
        update();
    }

    template<typename Data>
    PolynomialPath<Data>::~PolynomialPath()
    {

    }

    template<typename Data>
    std::vector<Data>& PolynomialPath<Data>::ctrlPts()
    {
        return _ctrlPts;
    }

    template<typename Data>
    const std::vector<Data>& PolynomialPath<Data>::ctrlPts() const
    {
        return _ctrlPts;
    }

    template<typename Data>
    std::vector<double>& PolynomialPath<Data>::weights()
    {
        return _weights;
    }

    template<typename Data>
    const std::vector<double>& PolynomialPath<Data>::weights() const
    {
        return _weights;
    }

    template<typename Data>
    Data PolynomialPath<Data>::value(double t) const
    {
        int intervalCount = _ctrlPts.size() - 1;
        int start = int(t * intervalCount) - 1;
        int end = glm::min(start + 4, int(_ctrlPts.size()));
        start = glm::max(0, start);

        Data value(0);
        double weight = 0.0;
        for(int i = start; i < end; ++i)
        {
            double w = _nodes[i].eval(t);
            value += _ctrlPts[i] * w;
            weight += w;
        }

        return value / weight;
    }

    template<typename Data>
    Data PolynomialPath<Data>::tangent(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    Data PolynomialPath<Data>::curvature(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    void PolynomialPath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }

    template<typename Data>
    void PolynomialPath<Data>::update()
    {
        assert(_ctrlPts.size() >= 4);
        assert(_ctrlPts.size() == _weights.size());

        _nodes.resize(_ctrlPts.size());
        size_t intervalCount = _ctrlPts.size() - 1;
        double len = 1.0 / intervalCount;

        // First node
        _nodes[0].buildFront( 0.0, len, _weights[0], ENodePos::First);
        _nodes[0].buildDuring(0.0, len, _weights[0], ENodePos::First);
        _nodes[0].buildBack(  0.0, len, _weights[0], ENodePos::First);

        // Second node
        _nodes[1].buildFront( len, len, _weights[1], ENodePos::Second);
        _nodes[1].buildDuring(len, len, _weights[1], ENodePos::Second);
        _nodes[1].buildBack(  len, len, _weights[1], ENodePos::Second);

        // Second to last node
        size_t s = _nodes.size() - 2;
        _nodes[s].buildFront( 1.0-len, len, _weights[s], ENodePos::BeforeLast);
        _nodes[s].buildDuring(1.0-len, len, _weights[s], ENodePos::BeforeLast);
        _nodes[s].buildBack(  1.0-len, len, _weights[s], ENodePos::BeforeLast);
        // Last node
        size_t l = _nodes.size() - 1;
        _nodes[l].buildFront( 1.0, len, _weights[l], ENodePos::Last);
        _nodes[l].buildDuring(1.0, len, _weights[l], ENodePos::Last);
        _nodes[l].buildBack(  1.0, len, _weights[l], ENodePos::Last);

        for(size_t i=2; i < _nodes.size() - 2; ++i)
        {
            _nodes[i].buildFront( i * len, len, _weights[i], ENodePos::Middle);
            _nodes[i].buildDuring(i * len, len, _weights[i], ENodePos::Middle);
            _nodes[i].buildBack(  i * len, len, _weights[i], ENodePos::Middle);
        }
    }

    template<typename Data>
    double PolynomialPath<Data>::Node::eval(double t) const
    {
        glm::dvec3 poly;
        if(t <= _lowerBound)
        {
            poly = _before;
        }
        else if(t >= _upperBound)
        {
            poly = _after;
        }
        else
        {
            poly = _during;
        }

        return poly[0] + t*(poly[1] + t*(poly[2]));
    }

    template<typename Data>
    void PolynomialPath<Data>::Node::buildFront(double center, double intervall, double weight, ENodePos pos)
    {
        if(pos == ENodePos::First)
        {
            _lowerBound = 0.0;
            _before = glm::dvec3(weight, 0.0, 0.0);
        }
        else if(pos == ENodePos::Second)
        {
            _lowerBound = 0.0;
            _before = glm::dvec3(0.0, 0.0, 0.0);
        }
        else
        {
            _lowerBound = center - intervall;

            double t0 = center - 2.0 * intervall;
            double t1 = center - intervall;
            double t2 = center;
            glm::mat3 A(
                1.0,   1.0,   1.0,
                t0,    t1,    t2,
                t0*t0, t1*t1, t2*t2);
            glm::dvec3 R(0.0, 0.25, weight);
            _before = glm::inverse(A) * R;
        }
    }

    template<typename Data>
    void PolynomialPath<Data>::Node::buildDuring(double center, double intervall, double weight, ENodePos pos)
    {
        double t0 = center - intervall;
        double t1 = center;
        double t2 = center + intervall;
        glm::mat3 A(
            1.0,   1.0,   1.0,
            t0,    t1,    t2,
            t0*t0, t1*t1, t2*t2);

        glm::dvec3 R(0.25, weight, 0.25);
        if(pos == ENodePos::Second)
            R[0] = 0.0;
        else if(pos == ENodePos::BeforeLast)
            R[2] = 0.0;

        _during = glm::inverse(A) * R;
    }

    template<typename Data>
    void PolynomialPath<Data>::Node::buildBack(double center, double intervall, double weight, ENodePos pos)
    {
        if(pos == ENodePos::BeforeLast)
        {
            _upperBound = 1.0;
            _after = glm::dvec3(0.0, 0.0, 0.0);
        }
        else
        if(pos == ENodePos::Last)
        {
            _upperBound = 1.0;
            _after = glm::dvec3(weight, 0.0, 0.0);
        }
        else
        {
            _upperBound = center + intervall;

            double t0 = center;
            double t1 = center + intervall;
            double t2 = center + 2.0 * intervall;
            glm::mat3 A(
                1.0,   1.0,   1.0,
                t0,    t1,    t2,
                t0*t0, t1*t1, t2*t2);
            glm::dvec3 R(weight, 0.25, 0.0);
            _after = glm::inverse(A) * R;
        }
    }

    template<typename Data>
    std::string PolynomialPath<Data>::csv(double dt)
    {
        std::string data;
        for(double t = 0.0; t <= 1.0; t += dt)
        {
            int intervalCount = _ctrlPts.size() - 1;
            int start = int(t * intervalCount) - 1;
            int end = glm::min(start + 4, int(_ctrlPts.size()));
            start = glm::max(0, start);

            for(size_t i=0; i < _nodes.size(); ++i)
                if(start <= i && i < end)
                    data += std::to_string(_nodes[i].eval(t)) + ",";
                else
                    data += "0,";
            data.back() = ' ';
            data += "\n";
        }

        return data;
    }
}

#endif // CELLAR_POLYNOMIALPATH_H
