#include "tcpmanager.h"
#include <QAbstractSocket>

TcpManager::~TcpManager()
{

}

TcpManager::TcpManager():_host(""),_port(0),_b_recv_pending(false),_message_id(0),_message_len(0)
{
    connect(&_socket, &QTcpSocket::connected, [&]() {
           qDebug() << "Connected to server!";
           emit sig_con_success(true);
       });
    connect(&_socket, &QTcpSocket::readyRead, [&]() {
           // 读取所有数据并追加到缓冲区
           _buffer.append(_socket.readAll());
           QDataStream stream(&_buffer, QIODevice::ReadOnly);
           stream.setVersion(QDataStream::Qt_5_0);
           forever
           {
                if(_b_recv_pending == false)
                {
                    if(_buffer.size() < static_cast<int>(sizeof (quint16) * 2))
                        return ;

                    stream >> _message_id >> _message_len;
                    _buffer = _buffer.mid(sizeof (quint16)*2);
                }

                if(_buffer.size() < _message_len)
                {
                    _b_recv_pending = true;
                    return;
                }
                _b_recv_pending = false;
                QByteArray messageBody = _buffer.mid(0,_message_len);
                qDebug()<<"接收到的body 消息是:" << messageBody;
                _buffer = _buffer.mid(_message_len);
                HandleMsg(ReqId(_message_id),_message_len,messageBody);
           }
       });
                                            // 返回值        // 参数
      connect(&_socket, static_cast<void(QTcpSocket::*)(QTcpSocket::SocketError)>(&QTcpSocket::error),
              [&](QTcpSocket::SocketError socketError){
               qDebug() << "Error:" << _socket.errorString() ;
               switch (socketError)
               {
                   case QTcpSocket::ConnectionRefusedError:
                       qDebug() << "Connection Refused!";
                       emit sig_con_success(false);
                       break;
                   case QTcpSocket::RemoteHostClosedError:
                       qDebug() << "Remote Host Closed Connection!";
                       break;
                   case QTcpSocket::HostNotFoundError:
                       qDebug() << "Host Not Found!";
                       emit sig_con_success(false);
                       break;
                   case QTcpSocket::SocketTimeoutError:
                       qDebug() << "Connection Timeout!";
                       emit sig_con_success(false);
                       break;
                   case QTcpSocket::NetworkError:
                       qDebug() << "Network Error!";
                       break;
                   default:
                       qDebug() << "Other Error!";
                       break;
               }
         });

        // 处理连接断开
        connect(&_socket, &QTcpSocket::disconnected, [&]() {
            qDebug() << "Disconnected from server.";
        });
        connect(this, &TcpManager::sig_send_data, this, &TcpManager::slot_send_data);

        InitHandlers();
}

void TcpManager::InitHandlers()
{
    _handlers.insert(ReqId::ID_CHAT_LOGIN_RSP,[this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug()<< "handle id is " << id << "data is " << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if(jsonDoc.isNull())
        {
            qDebug()<<"Failed to  create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(jsonObj.contains("error") == false)
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Login Failed, err is Json Parse Err" << err;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS)
        {
            qDebug()<< "Login Failed, err is" << err;
            emit sig_login_failed(err);
            return;
        }

//        UserManager::GetInstance()->SetUid(jsonObj["uid"].toInt());
//        UserManager::GetInstance()->SetName(jsonObj["name"].toString());
//        UserManager::GetInstance()->SetToken(jsonObj["token"].toString());

        emit sig_switch_chatDialog();
    });
}

void TcpManager::HandleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if(find_iter == _handlers.end())
    {
        return;
    }

    find_iter.value()(id,len,data);
}


void TcpManager::slot_tcp_connect(ServerInfo si)
{
    qDebug()<< "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}

void TcpManager::slot_send_data(ReqId reqId, QString data)
{
    uint16_t id = reqId;
    // 将字符串转换为UTF-8编码的字节数组
    QByteArray dataBytes = data.toUtf8();
    // 计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(data.size());
    // 创建一个QByteArray用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);
    // 写入ID和长度
    out << id << len;
    // 添加字符串数据
    block.append(data);
    // 发送数据
    _socket.write(block);
}
