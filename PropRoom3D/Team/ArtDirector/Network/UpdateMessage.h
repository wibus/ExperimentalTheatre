#ifndef PROPROOM3D_UPDATEMESSAGE_H
#define PROPROOM3D_UPDATEMESSAGE_H

#include <memory>
#include <string>

class QObject;
class QIODevice;
class QByteArray;

#include <CellarWorkbench/Camera/Camera.h>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class PROP3D_EXPORT UpdateMessage
    {
    public:
        UpdateMessage(cellar::Camera& camera,
                      const std::string& stageSet);
        UpdateMessage(QIODevice& device);
        ~UpdateMessage();

        void writeMessage(QIODevice& device) const;

        bool isComplete() const;

        int uid;
        glm::mat4 view;
        glm::mat4 proj;
        glm::ivec2 viewport;
        std::string stageSetStream;

    private:
        static size_t __nextUid;
        static size_t __genUid();

        bool _isComplete;
    };
}

#endif // PROPROOM3D_UPDATEMESSAGE_H
