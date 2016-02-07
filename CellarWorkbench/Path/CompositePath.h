#ifndef CELLAR_COMPOSITEPATH_H
#define CELLAR_COMPOSITEPATH_H

#include <vector>
#include <memory>

#include "AbstractPath.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT CompositePath : public AbstractPath<Data>
    {
    public:
        CompositePath();
        virtual ~CompositePath();

        void clearPaths();
        void addPath(double length, const std::shared_ptr<AbstractPath<Data>>& path);


        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

    private:
        int findPath(double& t) const;

        std::vector<double> _lengths;
        std::vector<std::shared_ptr<AbstractPath<Data>>> _paths;
    };



    // IMPLEMENTATION
    template<typename Data>
    CompositePath<Data>::CompositePath()
    {

    }

    template<typename Data>
    CompositePath<Data>::~CompositePath()
    {

    }

    template<typename Data>
    void CompositePath<Data>::clearPaths()
    {
        _paths.clear();
        _lengths.clear();
    }

    template<typename Data>
    void CompositePath<Data>::addPath(double length, const std::shared_ptr<AbstractPath<Data>>& path)
    {
        _paths.push_back(path);
        _lengths.push_back(length);
    }

    template<typename Data>
    Data CompositePath<Data>::value(double t) const
    {
        int i = findPath(t);
        return _paths[i]->value(t);
    }

    template<typename Data>
    Data CompositePath<Data>::tangent(double t) const
    {
        int i = findPath(t);
        return _paths[i]->tangent(t) / _lengths[i];
    }

    template<typename Data>
    Data CompositePath<Data>::curvature(double t) const
    {
        int i = findPath(t);
        return _paths[i]->curvature(t) / (_lengths[i] * _lengths[i]);
    }

    template<typename Data>
    int CompositePath<Data>::findPath(double& t) const
    {
        assert(!_paths.empty());
        size_t pathCount = _paths.size();
        for(int i=0; i < pathCount; ++i)
        {
            double pathLength = _lengths[i];
            if(t < pathLength)
            {
                t /= pathLength;
                return i;
            }
            else
            {
                t -= pathLength;
            }
        }

        t = 1.0;
        return pathCount-1;
    }
}

#endif // CELLAR_COMPOSITEPATH_H
