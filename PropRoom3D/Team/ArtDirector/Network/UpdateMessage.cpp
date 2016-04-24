#include "UpdateMessage.h"

#include <QIODevice>
#include <QByteArray>

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
        size = 0;
        size += sizeof(uid);
        size += sizeof(size);
        size += sizeof(view);
        size += sizeof(proj);
        size += sizeof(viewport);
        size += sizeof(stageSetStream.front()) *
                    stageSetStream.size();
    }

    UpdateMessage::UpdateMessage(QIODevice& device) :
        _isComplete(false)
    {
         if(device.read((char*)&uid, sizeof(uid)) == -1)
         {
            getLog().postMessage(new Message('E', false,
                "Could not read UpdateMessage UID", "UpdateMessage"));
         }
         else if(device.read((char*)&size, sizeof(size)) == -1)
         {
            getLog().postMessage(new Message('E', false,
                 "Could not read UpdateMessage size", "UpdateMessage"));
         }
         else if(device.read((char*)&view, sizeof(view)) == -1)
         {
            getLog().postMessage(new Message('E', false,
                 "Could not read UpdateMessage view matrix", "UpdateMessage"));
         }
         else if(device.read((char*)&proj, sizeof(proj)) == -1)
         {
            getLog().postMessage(new Message('E', false,
                 "Could not read UpdateMessage proj matrix", "UpdateMessage"));
         }
         else if(device.read((char*)&viewport, sizeof(viewport)) == -1)
         {
            getLog().postMessage(new Message('E', false,
                 "Could not read UpdateMessage viewport dim", "UpdateMessage"));
         }
         else
         {
            size_t constLen = 0;
            constLen += sizeof(uid);
            constLen += sizeof(size);
            constLen += sizeof(view);
            constLen += sizeof(proj);
            constLen += sizeof(viewport);

            size_t stageSetBytes = size - constLen;

            if(stageSetBytes > 10000000)
            {
                getLog().postMessage(new Message('E', false,
                    "Message size exceeds 10MB", "UpdateMessage"));
            }
            else
            {
                stageSetStream.resize(stageSetBytes);

                if(device.read((char*)stageSetStream.data(), stageSetBytes) == -1)
                {
                     getLog().postMessage(new Message('E', false,
                         "Could not read UpdateMessage stage set stream", "UpdateMessage"));
                }
                else
                {
                 _isComplete = true;

                 getLog().postMessage(new Message('I', false,
                     "UpdateMessage successfully read ("
                     + std::to_string(uid) + ")", "UpdateMessage"));
                }
            }
         }
    }

    UpdateMessage::~UpdateMessage()
    {

    }

    void UpdateMessage::writeMessage(QIODevice& device) const
    {
        if(device.write((char*)&uid, sizeof(uid)) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage UID", "UpdateMessage"));
        }
        else if(device.write((char*)&size, sizeof(size)) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage size", "UpdateMessage"));
        }
        else if(device.write((char*)&view, sizeof(view)) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage view matrix", "UpdateMessage"));
        }
        else if(device.write((char*)&proj, sizeof(proj)) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage proj matrix", "UpdateMessage"));
        }
        else if(device.write((char*)&viewport, sizeof(viewport)) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage viewport dim", "UpdateMessage"));
        }
        else if(device.write(stageSetStream.data(), stageSetStream.size()) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not wrtie UpdateMessage stage set stream", "UpdateMessage"));
        }
        else
        {
            getLog().postMessage(new Message('I', false,
                "UpdateMessage successfully writen ("
                + std::to_string(uid) + ")", "UpdateMessage"));
        }
    }

    bool UpdateMessage::isComplete() const
    {
        return _isComplete;
    }
}
