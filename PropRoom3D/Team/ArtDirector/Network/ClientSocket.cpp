#include "ClientSocket.h"

#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Misc/Log.h>

using namespace cellar;


namespace prop3
{
    ClientSocket::ClientSocket(QTcpSocket* socket,
                    const std::shared_ptr<Film>& film) :
        _isConnected(false),
        _socket(socket),
        _film(film)
    {
        connect(_socket, &QTcpSocket::connected,
                this, &ClientSocket::connected);

        connect(_socket, &QTcpSocket::readyRead,
                this, &ClientSocket::readMessage);

        connect(_socket, &QTcpSocket::disconnected,
                this, &ClientSocket::disconected);
    }

    ClientSocket::~ClientSocket()
    {
        if(_socket->isOpen())
            _socket->disconnectFromHost();

        _socket->deleteLater();
    }

    int ClientSocket::tcpPort() const
    {
        return _socket->peerPort();
    }

    QString ClientSocket::ipAddress() const
    {
        return _socket->peerAddress().toString();
    }

    bool ClientSocket::isConnected() const
    {
        return _isConnected;
    }

    void ClientSocket::readMessage()
    {

    }

    void ClientSocket::connected()
    {
        _isConnected = true;

        getLog().postMessage(new Message('I', false,
            "New connection established with server",
            "ArtDirectorClient"));
    }

    void ClientSocket::disconected()
    {
        _isConnected = false;
        _socket->close();

        getLog().postMessage(new Message('I', false,
            "Server disconnected from client",
            "ClientSocket"));
    }
}
