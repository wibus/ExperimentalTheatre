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
        BasisSplinePath(double duration, int degree,
                        const std::vector<Data>& ctrlPts);
        virtual ~BasisSplinePath();

        int degree() const;

        std::vector<Data>& ctrlPts();
        const std::vector<Data>& ctrlPts() const;

        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;

        virtual void update() override;


    protected:
        virtual int findIntervall(double t) const;
        virtual void basisFunctions(int i, double t, double* N) const;

    private:
        struct Node {Data v; Data t;};
        std::vector<Data> _ctrlPts;
        std::vector<double> _knots;
        int _degree;
    };



    // IMPLEMENTATION
    template<typename Data>
    BasisSplinePath<Data>::BasisSplinePath(
            double duration, int degree,
            const std::vector<Data>& ctrlPts) :
        LeafPath<Data>(duration),
        _degree(degree),
        _ctrlPts(ctrlPts),
        _knots()
    {
        int m = _ctrlPts.size() + degree;
        _knots.resize(m + 1);

        int knotCount = _knots.size();
        for(int i=0; i < _degree; ++i)
        {
            _knots[i]  = double(0.0);
            _knots[knotCount - i - 1]  = double(1.0);
        }

        for(int i=degree; i < knotCount-_degree; ++i)
        {
            _knots[i] = double(i - degree) / (1.0 + knotCount - (_degree+1)*2.0);
        }
    }

    template<typename Data>
    BasisSplinePath<Data>::~BasisSplinePath()
    {

    }

    template<typename Data>
    int BasisSplinePath<Data>::degree() const
    {
        return _degree;
    }

    template<typename Data>
    std::vector<Data>& BasisSplinePath<Data>::ctrlPts()
    {
        return _ctrlPts;
    }

    template<typename Data>
    const std::vector<Data>& BasisSplinePath<Data>::ctrlPts() const
    {
        return _ctrlPts;
    }

    template<typename Data>
    Data BasisSplinePath<Data>::value(double t) const
    {
        // On trouve l'intervalle des fonctions
        // de base à évaluer pour le paramètre donné
        int span = findIntervall(t);

        // Le nombre de poids pour point de contrôle
        // est égal à l'ordre de la B-Spline
        double *N = new double[_degree+1];

        // On va chercher le poids des pt de contrôle
        basisFunctions(span, t, N);
        int firstCtrlPt = span - _degree;
        int lastCtrlPt = firstCtrlPt + _degree;

        // On calcul la somme pondérée des pt
        Data val;
        for(int i=firstCtrlPt; i<=lastCtrlPt; ++i)
        {
            val += _ctrlPts[i] *
                   N[i-firstCtrlPt];
        }

        delete N;

        return val;
    }

    template<typename Data>
    Data BasisSplinePath<Data>::tangent(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    Data BasisSplinePath<Data>::curvature(double t) const
    {
        return Data(0);
    }

    template<typename Data>
    int BasisSplinePath<Data>::findIntervall(double t) const
    {
        // On suppose le vecteur nodal ouvert
        // donc, on borne la recherche d'intervalle
        int currKnot = _degree + 1;
        int lastKnot = _knots.size() - (_degree + 1);

        // L'index d'intervalle correspond à l'index du noeud
        // qui borne inférieurement l'intervalle
        while(currKnot < lastKnot)
        {
            if(t <= _knots[currKnot])
            {
                return currKnot-1;
            }
            ++currKnot;
        }

        // Dernier interval non null si vecteur nodal ouvert
        return currKnot-1;
    }

    template<typename Data>
    void BasisSplinePath<Data>::basisFunctions(int i, double t, double* N) const
    {
        N[0] = 1.0;

         // Initialisation des poids à gauche
         // et à droite des fonctions de base
         double* left  = new double[_degree + 1];
         double* right = new double[_degree + 1];

         // Voir note de cours sur courbe B-Splinaire
         for(int j=1; j<=_degree; ++j)
         {
             left[j] = t - _knots[i+1-j];
             right[j] = _knots[i+j]-t;
             double saved = 0.0;

             for(int r=0; r<j; ++r)
             {
                 double temp = N[r]/(right[r+1]+left[j-r]);
                 N[r] = saved + right[r+1]*temp;
                 saved = left[j-r]*temp;
             }

             N[j] = saved;
         }

         delete left;
         delete right;
    }

    template<typename Data>
    void BasisSplinePath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }

    template<typename Data>
    void BasisSplinePath<Data>::update()
    {
    }
}

#endif // CELLAR_BASISSPLINEPATH_H
