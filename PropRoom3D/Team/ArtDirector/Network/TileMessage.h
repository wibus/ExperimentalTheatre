#ifndef PROPROOM3D_TILEMESSAGE_H
#define PROPROOM3D_TILEMESSAGE_H

#include <memory>
#include <string>

class QIODevice;
#include <QBuffer>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Tile;
    class Film;


    class PROP3D_EXPORT TileMessage
    {
    public:
        TileMessage(Film& film, int tileId, int uid);
        TileMessage(Film& film);
        ~TileMessage();

        int uid() const;
        bool isValid() const;
        size_t size() const;

        void encode();
        void decode();

        void write(QIODevice& device);
        void read(QIODevice& device);


    private:
        int _uid;
        int _tileId;
        Film& _film;

        QBuffer _buffer;
    };
}

#endif // PROPROOM3D_TILEMESSAGE_H
