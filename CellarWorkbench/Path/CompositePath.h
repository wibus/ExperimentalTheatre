#ifndef CELLAR_COMPOSITEPATH_H
#define CELLAR_COMPOSITEPATH_H

#include <vector>
#include <memory>

#include "AbstractPath.h"
#include "PathVisitor.h"


namespace cellar
{
    template<typename Data>
    class CELLAR_EXPORT CompositePath : public AbstractPath<Data>
    {
    public:
        CompositePath();
        virtual ~CompositePath();

        void clearPaths();
        void addPath(const std::shared_ptr<AbstractPath<Data>>& path);

        std::vector<std::shared_ptr<AbstractPath<Data>>> paths() const;

        virtual double duration() const override;

        virtual Data value(double t) const override;
        virtual Data tangent(double t) const override;
        virtual Data curvature(double t) const override;

        virtual void accept(PathVisitor<Data>& visitor) override;

        virtual void update() override;


    private:
        bool findPath(double& t, int& i) const;

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
    }

    template<typename Data>
    void CompositePath<Data>::addPath(const std::shared_ptr<AbstractPath<Data>>& path)
    {
        _paths.push_back(path);
    }

    template<typename Data>
    std::vector<std::shared_ptr<AbstractPath<Data>>> CompositePath<Data>::paths() const
    {
        return _paths;
    }

    template<typename Data>
    double CompositePath<Data>::duration() const
    {
        double duration = 0.0;
        size_t pathCount = _paths.size();
        for(int i=0; i < pathCount; ++i)
            duration += _paths[i]->duration();

        return duration;
    }

    template<typename Data>
    Data CompositePath<Data>::value(double t) const
    {
        int i;
        if(findPath(t, i))
            return _paths[i]->value(t);
        else
            return Data(0);
    }

    template<typename Data>
    Data CompositePath<Data>::tangent(double t) const
    {
        int i;
        if(findPath(t, i))
            return _paths[i]->tangent(t);
        else
            return Data(0);
    }

    template<typename Data>
    Data CompositePath<Data>::curvature(double t) const
    {
        int i;
        if(findPath(t, i))
            return _paths[i]->curvature(t);
        else
            return Data(0);
    }

    template<typename Data>
    bool CompositePath<Data>::findPath(double& t, int& i) const
    {
        if(_paths.empty())
        {
            t = 0.0;
            i = -1;
            return false;
        }

        t = glm::max(t, 0.0);
        size_t pathCount = _paths.size();
        for(i=0; i < pathCount; ++i)
        {
            double pathDuration = _paths[i]->duration();
            if(t < pathDuration)
            {
                t /= pathDuration;
                return true;
            }
            else
            {
                t -= pathDuration;
            }
        }

        --i;
        t = 1.0;
        return true;
    }

    template<typename Data>
    void CompositePath<Data>::accept(PathVisitor<Data>& visitor)
    {
        visitor.visit(*this);
    }

    template<typename Data>
    void CompositePath<Data>::update()
    {
        for(const auto& child : _paths)
            child->update();
    }
}

#endif // CELLAR_COMPOSITEPATH_H
