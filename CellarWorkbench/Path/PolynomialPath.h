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
        virtual void findIntervall(double t, int& beg, int& end) const;

        struct CELLAR_EXPORT Node
        {
            double eval(double t) const;
            void build(double center, double intervall, double weight, int pos);

            static const int FIRST = 0;
            static const int SECOND = 1;
            static const int MIDDLE = 2;
            static const int BEFORE_LAST = 3;
            static const int LAST = 4;

            const double P[5][5] = {{ 1.000000, -0.500000, 0.062500, 0.000000, 0.000000  },
                                    { 0.000000, 2.370370, -1.777778, 0.444444, -0.037037  },
                                    { 0.000000, 0.000000, 1.000000, -0.500000, 0.062500  },
                                    { 0.000000, 0.000000, -0.000000, 0.148148, -0.037037  },
                                    { 0.000000, 0.000000, 0.062500, 0.000000, 0.000000  }};

        private:
            int _pos;
            double _scale;
            double _origin;
            double _weight;
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
        int beg, end;
        findIntervall(t, beg, end);

        Data value(0);
        double weight = 0.0;
        for(int i = beg; i <= end; ++i)
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
        assert(_ctrlPts.size() >= 5);
        assert(_ctrlPts.size() == _weights.size());

        _nodes.resize(_ctrlPts.size());
        size_t intervalCount = _ctrlPts.size() - 1;
        double len = 1.0 / intervalCount;

        // First node
        _nodes[0].build( 0.0, len, _weights[0], Node::FIRST);

        // Second node
        _nodes[1].build( len, len, _weights[1], Node::SECOND);

        // Middle nodes
        for(size_t i=2; i < _nodes.size() - 2; ++i)
        {
            _nodes[i].build( i * len, len, _weights[i], Node::MIDDLE);
        }

        // Second to last node
        size_t s = _nodes.size() - 2;
        _nodes[s].build( 1.0-len, len, _weights[s], Node::BEFORE_LAST);

        // Last node
        size_t l = _nodes.size() - 1;
        _nodes[l].build( 1.0, len, _weights[l], Node::LAST);
    }

    template<typename Data>
    void PolynomialPath<Data>::findIntervall(double t, int& beg, int& end) const
    {
        int intervalCount = _ctrlPts.size() - 1;
        int mid = int(t * intervalCount);
        beg = glm::max(mid - 3, 0);
        end = glm::min(mid + 4, int(_ctrlPts.size()-1));
    }

    template<typename Data>
    double PolynomialPath<Data>::Node::eval(double t) const
    {
        double u = (t - _origin) * _scale;

        if(u < 0 || u > 4)
            return 0.0;

        return (P[_pos][0] +
                 u*(P[_pos][1] +
                  u*(P[_pos][2] +
                   u*(P[_pos][3] +
                    u*(P[_pos][4])
                )))) * _weight;
    }

    template<typename Data>
    void PolynomialPath<Data>::Node::build(double center, double intervall, double weight, int pos)
    {
        _scale = 1.0 / intervall;
        _origin = center - pos * intervall;
        _weight = weight;
        _pos = pos;
    }

    template<typename Data>
    std::string PolynomialPath<Data>::csv(double dt)
    {
        std::string data;
        for(double t = 0.0; t <= 1.0; t += dt)
        {
            int beg, end;
            findIntervall(t, beg, end);
            for(size_t i=0; i < _nodes.size(); ++i)
                if(beg <= i && i <= end)
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
