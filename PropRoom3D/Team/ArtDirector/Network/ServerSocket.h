#ifndef PROPROOM3D_SERVERSOCKET_H
#define PROPROOM3D_SERVERSOCKET_H

#include <memory>

#include <QObject>
class QTcpSocket;

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Film;

    class PROP3D_EXPORT ServerSocket : public QObject
    {
        Q_OBJECT

    public:
        ServerSocket(QTcpSocket* socket,
            const std::shared_ptr<Film>& film);
        ~ServerSocket();

        bool isOpen() const;
        int tcpPort() const;
        QString ipAddress() const;


    protected slots:
        void readTile();
        void disconected();

    private:
        QTcpSocket* _socket;
        std::shared_ptr<Film> _film;
    };
}

#endif // PROPROOM3D_SERVERSOCKET_H
