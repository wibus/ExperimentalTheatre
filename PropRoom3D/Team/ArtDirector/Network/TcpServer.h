#ifndef PROPROOM3D_TCPSERVER_H
#define PROPROOM3D_TCPSERVER_H

#include <memory>

#include <QTcpServer>

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class ServerSocket;
    class UpdateMessage;
    class ConvergentFilm;

    class PROP3D_EXPORT TcpServer : public QTcpServer
    {
        Q_OBJECT

    public:
        TcpServer(const std::shared_ptr<ConvergentFilm>& film,
                  QObject* parent = 0);
        virtual ~TcpServer();

        void update();

        void dispatchUpdateMessage(
            const std::shared_ptr<UpdateMessage>& msg);

    protected slots:
        virtual void incomingConnection(qintptr socketDescriptor) override;
        virtual void clientConnectionClosed();

    private:
        std::list<ServerSocket*> _sockets;
        std::shared_ptr<ConvergentFilm> _film;
        std::shared_ptr<UpdateMessage> _updateMessage;
    };
}

#endif // PROPROOM3D_TCPSERVER_H
