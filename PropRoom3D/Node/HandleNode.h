#ifndef PROPROOM3D_HANDLENODE_H
#define PROPROOM3D_HANDLENODE_H

#include <GLM/glm.hpp>

#include "Node.h"


namespace prop3
{
    class PROP3D_EXPORT HandleNode : public Node
    {
    protected:
        HandleNode(const std::string& name);

    public:
        virtual ~HandleNode();


        // Visibility
        bool isVisible() const;
        virtual void setIsVisible(bool isVisible);

        // Name
        std::string name() const;
        void setName(const std::string& name);

        // Clear
        virtual void clear() = 0;

        // Transformations
        virtual void transform(const glm::dmat4& mat) = 0;
        virtual void translate(const glm::dvec3& dis) = 0;
        virtual void rotate(double angle, const glm::dvec3& axis) = 0;
        virtual void scale(double coeff) = 0;


    private:
        bool _isVisible;
        std::string _name;
    };



    // IMPLEMENTATION //
    inline bool HandleNode::isVisible() const
    {
        return _isVisible;
    }

    inline std::string HandleNode::name() const
    {
        return _name;
    }
}

#endif // PROPROOM3D_HANDLENODE_H
