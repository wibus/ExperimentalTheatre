#ifndef PROPROOM3D_BACKDROP_H
#define PROPROOM3D_BACKDROP_H

#include <GLM/glm.hpp>

#include <PropRoom3D/Light/Light.h>


namespace prop3
{

    class PROP3D_EXPORT Backdrop : public Light
    {
    protected:
        Backdrop(bool isDirectlyVisible = true);

    public:
        virtual ~Backdrop();


        bool isDirectlyVisible() const;

        void setIsDirectlyVisible(bool isDirectlyVisible);


    private:
        bool _isDirectlyVisible;
    };



    // IMPLEMENTATION //
    inline bool Backdrop::isDirectlyVisible() const
    {
        return _isDirectlyVisible;
    }
}

#endif // PROPROOM3D_BACKDROP_H
