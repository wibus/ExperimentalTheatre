#include "UpdateMessage.h"

#include <QBuffer>
#include <QIODevice>
#include <QByteArray>
#include <QDataStream>
#include <QCoreApplication>

#include <CellarWorkbench/Misc/Log.h>

using namespace cellar;


namespace prop3
{
    size_t UpdateMessage::__nextUid = 0;

    size_t UpdateMessage::__genUid()
    {
        return __nextUid++;
    }

    UpdateMessage::UpdateMessage(
            cellar::Camera& camera,
            const std::string &stageSet) :
        _isComplete(true),
        uid(__genUid()),
        view(camera.viewMatrix()),
        proj(camera.projectionMatrix()),
        viewport(camera.viewport()),
        stageSetStream(stageSet)
    {
    }

    UpdateMessage::UpdateMessage(QIODevice& device) :
        _isComplete(false)
    {
        int size = 0;

        QBuffer buffer;
        buffer.open(QIODevice::ReadWrite);

        QDataStream stream(&buffer);
        while(buffer.size() < sizeof(size))
        {
            if(!device.bytesAvailable())
            {
                if(!device.waitForReadyRead(30000))
                {
                    getLog().postMessage(new Message('E', false,
                        "Message size could not be read",
                        "UpdateMessage"));
                    return;
                }
            }

            buffer.write(device.read(
                sizeof(size) - buffer.size()));
        }

        buffer.seek(0);
        stream.readRawData((char*)&size, sizeof(size));

        while(buffer.size() < size)
        {
            if(!device.bytesAvailable())
            {
                if(!device.waitForReadyRead(30000))
                {
                    getLog().postMessage(new Message('E', false,
                        "Message body could not me read",
                        "UpdateMessage"));
                    return;
                }
            }
            else
            {
                buffer.write(device.read(size - buffer.size()));
            }
        }

        buffer.seek(sizeof(size));
        stream.readRawData((char*)&uid,      sizeof(uid));
        stream.readRawData((char*)&view,     sizeof(view));
        stream.readRawData((char*)&proj,     sizeof(proj));
        stream.readRawData((char*)&viewport, sizeof(viewport));

        stageSetStream.resize(buffer.size() - buffer.pos());
        stream.readRawData(const_cast<char*>(stageSetStream.data()),
                         stageSetStream.size());

        _isComplete = true;

        getLog().postMessage(new Message('I', false,
            "UpdateMessage successfully read (UID="
             + std::to_string(uid) + ", size=" +
             std::to_string(buffer.size()) + ")",
             "UpdateMessage"));
    }

    UpdateMessage::~UpdateMessage()
    {

    }

    void UpdateMessage::writeMessage(QIODevice& device) const
    {
        int size = 0;

        QBuffer bytes;
        bytes.open(QIODevice::WriteOnly);

        QDataStream stream(&bytes);
        stream.writeRawData((char*)&size, sizeof(size));
        stream.writeRawData((char*)&uid,  sizeof(uid));
        stream.writeRawData((char*)&view, sizeof(view));
        stream.writeRawData((char*)&proj, sizeof(proj));
        stream.writeRawData((char*)&viewport, sizeof(viewport));
        stream.writeRawData(stageSetStream.data(), stageSetStream.size());

        size = bytes.size();
        stream.device()->seek(0);
        stream.writeRawData((char*)&size, sizeof(size));

        if(device.write(bytes.data()) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not write UpdateMessage", "UpdateMessage"));
        }
        else
        {
            getLog().postMessage(new Message('I', false,
                "UpdateMessage successfully writen (UID="
                + std::to_string(uid) + ", size=" +
                std::to_string(bytes.size()) + ")",
                "UpdateMessage"));
        }
    }

    bool UpdateMessage::isComplete() const
    {
        return _isComplete;
    }
}
