#ifndef PROPROOM3D_CLIENTSOCKET_H
#define PROPROOM3D_CLIENTSOCKET_H

#include <memory>

#include <QObject>
class QTcpSocket;

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class Film;

    class PROP3D_EXPORT ClientSocket : public QObject
    {
        Q_OBJECT

    public:
        ClientSocket(QTcpSocket* socket,
            const std::shared_ptr<Film>& film);
        ~ClientSocket();

        int tcpPort() const;
        QString ipAddress() const;
        bool isConnected() const;


    protected slots:
        void connected();
        void readMessage();
        void disconected();

    private:
        bool _isConnected;
        QTcpSocket* _socket;
        std::shared_ptr<Film> _film;
    };
}

#endif // PROPROOM3D_CLIENTSOCKET_H
