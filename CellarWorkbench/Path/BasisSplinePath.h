#ifndef CELLAR_BASISSPLINEPATH_H
#define CELLAR_BASISSPLINEPATH_H

#include <vector>

#include "AbstractPath.h"
#include "PathVisitor.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT BasisSplinePath : public LeafPath<Data>
    {
    public:
        BasisSplinePath(double duration, const std::vector<Data>& ctrlPts);
        virtual ~BasisSplinePath();

        const std::vector<Data>& ctrlPts() const;

        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;


    protected:
        virtual int findIntervall(double& t) const;
        virtual Data interpolate(int i, double h[]) const;

    private:
        struct Node {Data v; Data t;};
        std::vector<Data> _ctrlPts;
        std::vector<Node> _nodes;
    };



    // IMPLEMENTATION
    template<typename Data>
    BasisSplinePath<Data>::BasisSplinePath(
            double duration,
            const std::vector<Data>& ctrlPts) :
        LeafPath<Data>(duration)
    {
        assert(ctrlPts.size() >= 4);
        int lastCtrlPt = ctrlPts.size() - 1;
        int nodeCount = ctrlPts.size() - 2;
        int lastNode = nodeCount - 1;
        _nodes.resize(nodeCount);

        _nodes[0].t = ctrlPts[1] - ctrlPts[0];
        _nodes[lastNode].t = ctrlPts[lastCtrlPt] - ctrlPts[lastCtrlPt-1];

        for(int i=0; i < nodeCount; ++i)
            _nodes[i].v = ctrlPts[i+1];


        // System variables
        int nbEq = nodeCount - 2;
        if(nbEq > 0)
        {
            // Construction du vecteur resultant a partir des
            // tangentes aux extremites et des positions des points de controle
            std::vector<Data> res(nbEq);
            res[0] = (_nodes[2].v - _nodes[0].v - _nodes[0].t / 3.0) * 3.0;
            for (int i = 1; i < nbEq - 1; ++i) res[i] = 3.0 * (_nodes[i + 2].v - _nodes[i].v);
            res[nbEq - 1] = (_nodes[lastNode].v - _nodes[nodeCount - 3].v - _nodes[lastNode].t / 3.0) * 3.0;

            // Quelques variables temporaires
            std::vector<double> gam(nbEq);
            double a = 1, b = 4, c = 1;
            double bet = b;

            // Resolution du systeme tridiagonal
            // Reference : http://www.haoli.org/nr/bookcpdf/c2-4.pdf
            // Bon demelage d'indices :) !
            _nodes[1].t = res[0] / bet;
            for (int i = 1; i < nbEq; ++i)
            {
                gam[i] = c / bet;
                bet = b - a * gam[i];
                _nodes[i + 1].t = (res[i] - a * _nodes[i].t) / bet;
            }
            for (int i = nbEq - 2; i >= 0; --i)
            {
                _nodes[i + 1].t -= gam[i + 1] * _nodes[i + 2].t;
            }
        }
    }

    template<typename Data>
    BasisSplinePath<Data>::~BasisSplinePath()
    {

    }

    template<typename Data>
    const std::vector<Data>& BasisSplinePath<Data>::ctrlPts() const
    {
        return _ctrlPts;
    }

    template<typename Data>
    Data BasisSplinePath<Data>::value(double t) const
    {
        double u = glm::clamp(t, 0.0, 1.0);
        int i = findIntervall(u);

        float u2 = u * u,
              u3 = u2 * u;

        double h[] = {
            2*u3 - 3*u2 + 1,
            -2*u3 + 3*u2,
            u3 - 2*u2 + u,
            u3 - u2
        };

        return interpolate(i, h);
    }

    template<typename Data>
    Data BasisSplinePath<Data>::tangent(double t) const
    {
        double u = glm::clamp(t, 0.0, 1.0);
        int i = findIntervall(u);

        float u2 = u * u;

        double h[] = {
            6*u2  - 6*u,
            -6*u2 + 6*u,
            3*u2  - 4*u + 1,
            3*u2  - 2*u
        };

        return interpolate(i, h);
    }

    template<typename Data>
    Data BasisSplinePath<Data>::curvature(double t) const
    {
        double u = glm::clamp(t, 0.0, 1.0);
        int i = findIntervall(u);

        double h[] = {
            12*u  - 6,
            -12*u + 6,
            6*u  - 4,
            6*u  - 2
        };

        return interpolate(i, h);
    }

    template<typename Data>
    int BasisSplinePath<Data>::findIntervall(double& t) const
    {
        int nbSections = _nodes.size() - 1;
        float u = t * (nbSections);
        int i = (int)glm::floor(u);
        t = u - i;
        return i;
    }

    template<typename Data>
    Data BasisSplinePath<Data>::interpolate(int i, double h[]) const
    {
        return
            h[0] * _nodes[i].v +
            h[1] * _nodes[i + 1].v +
            h[2] * _nodes[i].t +
            h[3] * _nodes[i + 1].t;
    }

    template<typename Data>
    void BasisSplinePath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }
}

#endif // CELLAR_BASISSPLINEPATH_H
