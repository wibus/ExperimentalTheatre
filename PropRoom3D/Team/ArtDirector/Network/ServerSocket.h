#ifndef PROPROOM3D_SERVERSOCKET_H
#define PROPROOM3D_SERVERSOCKET_H

#include <memory>

#include <QObject>
class QTcpSocket;

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Film;
    class UpdateMessage;

    class PROP3D_EXPORT ServerSocket : public QObject
    {
        Q_OBJECT

    public:
        ServerSocket(QTcpSocket* socket,
            const std::shared_ptr<Film>& film);
        ~ServerSocket();

        int tcpPort() const;
        QString ipAddress() const;
        bool isConnected() const;

        void sendUpdate(const UpdateMessage& message);


    protected slots:
        void readTile();
        void disconnected();

    private:
        bool _isConnected;
        QTcpSocket* _socket;
        std::shared_ptr<Film> _film;
    };
}

#endif // PROPROOM3D_SERVERSOCKET_H
