#ifndef PROPROOM3D_ABSTRACTARTDIRECTOR_H
#define PROPROOM3D_ABSTRACTARTDIRECTOR_H

#include <memory>

#include <GLM/glm.hpp>

#include <QObject>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include "../../libPropRoom3D_global.h"


namespace prop3
{
    class Prop;
    class StageSet;
    class Environment;
    class GlPostProdUnit;


    class PROP3D_EXPORT AbstractArtDirector :
            public QObject,
            public cellar::SpecificObserver<cellar::CameraMsg>
    {
        Q_OBJECT

    protected:
        AbstractArtDirector()
        {
            setCamera(std::shared_ptr<cellar::Camera>(new cellar::Camera()));
        }

    public:
        virtual ~AbstractArtDirector() {}

        virtual void setup(const std::shared_ptr<StageSet>& stageSet) =0;
        virtual void update(double dt) =0;
        virtual void draw(double dt) =0;
        virtual void terminate() =0;

        virtual void notify(cellar::CameraMsg &msg) = 0;

        virtual void resize(int width, int height);
        virtual const std::shared_ptr<cellar::Camera>& camera() const;
        virtual void setCamera(const std::shared_ptr<cellar::Camera>& camera);

        constexpr static const double FORCE_REFRESH_DT = 0.0;


    private:
        std::shared_ptr<cellar::Camera> _camera;
    };

    inline PROP3D_EXPORT std::string toString(const char* ip, int port)
    {
        return QString("(IP %1, PORT %2)").arg(ip).arg(port).toStdString();
    }

    inline PROP3D_EXPORT std::string toString(const std::string& ip , int port)
    {
        return toString(ip.c_str(), port);
    }

    inline PROP3D_EXPORT std::string toString(const QString& ip , int port)
    {
        return toString(ip.toStdString(), port);
    }


    // IMPLEMENTATION //
    inline void AbstractArtDirector::resize(int width, int height)
    {
        _camera->updateViewport(width, height);
    }

    inline const std::shared_ptr<cellar::Camera>& AbstractArtDirector::camera() const
    {
        return _camera;
    }

    inline void AbstractArtDirector::setCamera(const std::shared_ptr<cellar::Camera>& camera)
    {
        if(_camera)
            _camera->unregisterObserver(*this);

        _camera = camera;

        if(_camera)
            _camera->registerObserver(*this);
    }
}

#endif // PROPROOM3D_ABSTRACTARTDIRECTOR_H
