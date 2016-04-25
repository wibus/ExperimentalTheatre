#include "TcpServer.h"

#include <QThread>

#include <CellarWorkbench/Misc/Log.h>

#include "ServerSocket.h"

using namespace cellar;


namespace prop3
{
    TcpServer::TcpServer(const std::shared_ptr<ConvergentFilm> &film,
                         QObject *parent) :
        QTcpServer(parent),
        _film(film)
    {

    }

    TcpServer::~TcpServer()
    {

    }

    void TcpServer::dispatchUpdateMessage(
        const std::shared_ptr<UpdateMessage>& msg)
    {
        _updateMessage = msg;
        for(ServerSocket* consumer : _sockets)
            consumer->sendUpdate(_updateMessage);;
    }

    void TcpServer::incomingConnection(qintptr socketDescriptor)
    {
        getLog().postMessage(new Message('I', false,
            "New client connected", "TcpServer"));

        _sockets.push_back(new ServerSocket(
            socketDescriptor, _film, _updateMessage));
        ServerSocket* consumer = _sockets.back();

        QThread* thread = new QThread;
        consumer->moveToThread(thread);

        connect(thread,     &QThread::started,
                consumer,   &ServerSocket::start);

        connect(consumer,   &ServerSocket::finished,
                this,       &TcpServer::clientConnectionClosed);

        connect(consumer,   &ServerSocket::finished,
                thread,     &QThread::quit);

        connect(consumer,   &ServerSocket::finished,
                consumer,   &ServerSocket::deleteLater);

        connect(thread,     &QThread::finished,
                thread,     &QThread::deleteLater);

        thread->start();
    }

    void TcpServer::clientConnectionClosed()
    {
        auto it = _sockets.begin();
        while(it != _sockets.end())
        {
            if(!(*it)->isConnected())
            {
                getLog().postMessage(new Message('I', false,
                    "Client connection removed", "TcpServer"));
                it = _sockets.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}
