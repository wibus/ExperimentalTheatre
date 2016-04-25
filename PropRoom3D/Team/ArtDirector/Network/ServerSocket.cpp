#include "ServerSocket.h"

#include <QTcpSocket>
#include <QNetworkInterface>

#include <CellarWorkbench/Misc/Log.h>

#include "../Film/ConvergentFilm.h"
#include "../Film/Tile.h"
#include "TileMessage.h"
#include "UpdateMessage.h"

using namespace cellar;


namespace prop3
{
    ServerSocket::ServerSocket(qintptr socketDescriptor,
                    const std::shared_ptr<ConvergentFilm>& film,
                    const std::shared_ptr<UpdateMessage>& msg) :
        _socketDescriptor(socketDescriptor),
        _isConnected(true),
        _film(film),
        _msg(msg)
    {

    }

    ServerSocket::~ServerSocket()
    {
    }

    bool ServerSocket::isConnected() const
    {
        return _isConnected;
    }

    void ServerSocket::sendUpdate(const std::shared_ptr<UpdateMessage>& msg)
    {
        _msg = msg;
        emit sendMsgSig();
    }

    void ServerSocket::readyRead()
    {
        while(_socket->bytesAvailable())
        {
            std::shared_ptr<TileMessage> msg(new TileMessage(*_film));
            msg->read(*_socket);

            if(!msg->isValid())
            {
                getLog().postMessage(new Message('E', false,
                    "Received tile is invalid",
                    "ServerSocket"));
            }
            else if(msg->uid() == _film->stateUid())
            {
                _film->addIncomingTileMessage(msg);
            }
        }
    }

    void ServerSocket::sendMsgSlot()
    {
        if(_msg.get() != nullptr)
        {
            _msg->writeMessage(*_socket);
            _msg.reset();
        }
    }

    void ServerSocket::disconnected()
    {
        _isConnected = false;

        getLog().postMessage(new Message('I', false,
            "Client disconnected from server",
            "ServerSocket"));

        emit finished();
    }

    void ServerSocket::start()
    {
        _socket = new QTcpSocket();

        if(!_socket->setSocketDescriptor(_socketDescriptor))
        {
            getLog().postMessage(new Message('E', false,
                "Could not retreive client's socket",
                "ServerSocket"));

            _isConnected = false;
            return;
        }

        connect(_socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
        connect(this, &ServerSocket::sendMsgSig, this, &ServerSocket::sendMsgSlot, Qt::QueuedConnection);
        connect(_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

        sendMsgSlot();
    }
}
