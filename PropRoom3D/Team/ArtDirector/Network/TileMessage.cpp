#include "TileMessage.h"

#include <QIODevice>
#include <QDataStream>

#include <CellarWorkbench/Misc/Log.h>

#include "../Film/Film.h"
#include "../Film/Tile.h"

using namespace cellar;


namespace prop3
{
    TileMessage::TileMessage(Film& film, int tileId, int uid) :
        _uid(uid),
        _tileId(tileId),
        _film(film)
    {

    }

    TileMessage::TileMessage(Film& film) :
        _uid(-1),
        _tileId(-1),
        _film(film)
    {
    }

    TileMessage::~TileMessage()
    {

    }

    int TileMessage::uid() const
    {
        return _uid;
    }

    bool TileMessage::isValid() const
    {
        return _tileId != -1;
    }

    void TileMessage::encode()
    {
        int size = 0;

        _buffer.open(QIODevice::WriteOnly);
        _buffer.seek(0);

        QDataStream stream(&_buffer);
        stream.writeRawData((char*)&size, sizeof(size));
        stream.writeRawData((char*)&_uid,  sizeof(_uid));
        stream.writeRawData((char*)&_tileId, sizeof(_tileId));

        std::shared_ptr<Tile> tile = _film.getTile(_tileId);
        glm::ivec2 tileMin = tile->minCorner();
        glm::ivec2 tileMax = tile->maxCorner();

        for(int y = tileMin.y; y < tileMax.y; ++y)
        {
            for(int x = tileMin.x; x < tileMax.x; ++x)
            {
                glm::vec4 sample = _film.pixelSample(x, y);
                stream.writeRawData((char*)&sample, sizeof(sample));
            }
        }

        size = _buffer.size();
        stream.device()->seek(0);
        stream.writeRawData((char*)&size, sizeof(size));

        _buffer.close();
    }

    void TileMessage::decode()
    {
        int size = 0;

        _buffer.open(QIODevice::ReadOnly);
        _buffer.seek(0);

        QDataStream stream(&_buffer);
        stream.readRawData((char*)&size,    sizeof(size));
        stream.readRawData((char*)&_uid,    sizeof(_uid));
        stream.readRawData((char*)&_tileId, sizeof(_tileId));

        if(_uid != _film.stateUid())
            return;

        std::shared_ptr<Tile> tile = _film.getTile(_tileId);
        if(tile.get() == nullptr)
        {
            getLog().postMessage(new Message('E', false,
                "Invalid tile ID reveived (id=" + std::to_string(_tileId) + ")",
                "TileMessage"));
            _tileId = -1;
            return;
        }

        glm::ivec2 tileMin = tile->minCorner();
        glm::ivec2 tileMax = tile->maxCorner();

        tile->lock();
        for(int y = tileMin.y; y < tileMax.y; ++y)
        {
            for(int x = tileMin.x; x < tileMax.x; ++x)
            {
                glm::vec4 sample;

                if(stream.readRawData((char*)&sample, sizeof(sample)) != sizeof(sample))
                {
                    getLog().postMessage(new Message('E', false,
                        "There were too few samples in this tile message",
                        "TileMessage"));

                    _tileId = -1;
                    tile->unlock();
                }
                else
                {
                    if(sample.w > 0.0)
                        tile->addSample(x, y, sample);
                }
            }
        }

        tile->unlock();
        _film.tileCompleted(*tile);

        _buffer.close();
    }

    void TileMessage::write(QIODevice& device)
    {
        if(device.write(_buffer.data()) == -1)
        {
            getLog().postMessage(new Message('E', false,
                "Could not send tile message on the network",
                "TileMessage"));
        }
    }

    void TileMessage::read(QIODevice& device)
    {
        int size = 0;

        _buffer.open(QIODevice::ReadWrite);

         QDataStream stream(&_buffer);
         while(_buffer.size() < sizeof(size))
         {
             if(!device.bytesAvailable())
             {
                 if(!device.waitForReadyRead(30000))
                 {
                     getLog().postMessage(new Message('E', false,
                         "Message size could not be read",
                         "TileMessage"));
                     return;
                 }
             }

             _buffer.write(device.read(
                               sizeof(size) - _buffer.size()));
         }

         _buffer.seek(0);
         stream.readRawData((char*)&size, sizeof(size));

         while(_buffer.size() < size)
         {
             if(!device.bytesAvailable())
             {
                 if(!device.waitForReadyRead(30000))
                 {
                     getLog().postMessage(new Message('E', false,
                         "Message body could not me read",
                         "TileMessage"));
                     return;
                 }
             }
             else
             {
                 _buffer.write(device.read(size - _buffer.size()));
             }
         }

         _buffer.seek(sizeof(size));
         stream.readRawData((char*)&_uid,    sizeof(_uid));
         stream.readRawData((char*)&_tileId, sizeof(_tileId));

         _buffer.close();
    }
}
