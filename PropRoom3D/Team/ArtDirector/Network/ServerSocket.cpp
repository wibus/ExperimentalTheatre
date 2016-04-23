#include "ServerSocket.h"

#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Misc/Log.h>

using namespace cellar;


namespace prop3
{
    ServerSocket::ServerSocket(QTcpSocket* socket,
                    const std::shared_ptr<Film>& film) :
        _socket(socket),
        _film(film)
    {
        connect(_socket, &QTcpSocket::readyRead,
                this, &ServerSocket::readTile);

        connect(_socket, &QTcpSocket::disconnected,
                this, &ServerSocket::disconected);
    }

    ServerSocket::~ServerSocket()
    {
        if(_socket->isOpen())
            _socket->disconnectFromHost();

        _socket->deleteLater();
    }

    bool ServerSocket::isOpen() const
    {
        return _socket->isOpen();
    }

    int ServerSocket::tcpPort() const
    {
        return _socket->peerPort();
    }

    QString ServerSocket::ipAddress() const
    {
        return _socket->peerAddress().toString();
    }

    void ServerSocket::readTile()
    {

    }

    void ServerSocket::disconected()
    {
        getLog().postMessage(new Message('I', false,
            "Client disconnected from server",
            "ServerSocket"));
    }
}
