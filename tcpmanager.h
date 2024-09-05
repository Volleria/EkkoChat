#ifndef TCPMANAGER_H
#define TCPMANAGER_H
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <functional>
#include <QObject>


class TcpManager:public QObject, public Singleton<TcpManager>, public std::enable_shared_from_this<TcpManager>
{
    Q_OBJECT
public:
    ~TcpManager();

private:
    friend class Singleton<TcpManager>;

    TcpManager();
    void InitHandlers();
    void HandleMsg(ReqId,int,QByteArray);
    QMap<ReqId,std::function<void(ReqId,int,QByteArray)>> _handlers;

    QTcpSocket _socket;
    QString _host;
    uint16_t _port;

    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
    QByteArray _buffer;

public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QString data);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QString data);
    void sig_switch_chatDialog();
    void sig_login_failed(int);
};

#endif // TCPMANAGER_H
