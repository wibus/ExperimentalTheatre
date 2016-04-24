#include "ServerSocket.h"

#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Misc/Log.h>

#include "UpdateMessage.h"

using namespace cellar;


namespace prop3
{
    ServerSocket::ServerSocket(QTcpSocket* socket,
                    const std::shared_ptr<Film>& film) :
        _isConnected(true),
        _socket(socket),
        _film(film)
    {
        connect(_socket, &QTcpSocket::readyRead,
                this, &ServerSocket::readTile);

        connect(_socket, &QTcpSocket::disconnected,
                this, &ServerSocket::disconnected);
    }

    ServerSocket::~ServerSocket()
    {
        if(_socket->isOpen())
            _socket->disconnectFromHost();

        _socket->deleteLater();
    }

    int ServerSocket::tcpPort() const
    {
        return _socket->peerPort();
    }

    QString ServerSocket::ipAddress() const
    {
        return _socket->peerAddress().toString();
    }

    bool ServerSocket::isConnected() const
    {
        return _isConnected;
    }

    void ServerSocket::sendUpdate(const UpdateMessage& message)
    {
        message.writeMessage(*_socket);
    }

    void ServerSocket::readTile()
    {

    }

    void ServerSocket::disconnected()
    {
        _isConnected = false;
        getLog().postMessage(new Message('I', false,
            "Client disconnected from server",
            "ServerSocket"));
    }
}
