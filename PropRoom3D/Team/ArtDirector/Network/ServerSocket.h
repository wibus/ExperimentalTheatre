#ifndef PROPROOM3D_SERVERSOCKET_H
#define PROPROOM3D_SERVERSOCKET_H

#include <mutex>
#include <memory>

#include <QObject>
class QTcpSocket;

#include <PropRoom3D/libPropRoom3D_global.h>


namespace prop3
{
    class ConvergentFilm;
    class UpdateMessage;

    class PROP3D_EXPORT ServerSocket : public QObject
    {
        Q_OBJECT

    public:
        ServerSocket(qintptr socketId,
            const std::shared_ptr<ConvergentFilm>& film,
            const std::shared_ptr<UpdateMessage>& msg);
        ~ServerSocket();

        bool isConnected() const;

        void sendUpdate(const std::shared_ptr<UpdateMessage>& msg);

    signals:
        void finished();
        void sendMsgSig();

    public slots:
        void start();
        void readyRead();
        void sendMsgSlot();
        void disconnected();

    private:
        bool _isConnected;
        QTcpSocket* _socket;
        qintptr _socketDescriptor;
        std::shared_ptr<ConvergentFilm> _film;
        std::shared_ptr<UpdateMessage> _msg;
    };
}

#endif // PROPROOM3D_SERVERSOCKET_H
