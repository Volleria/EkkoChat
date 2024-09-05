#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "httpmanager.h"
#include "tcpmanager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void initHttpHandlers();
    void initHead();
    bool CheckEmailValid();
    bool CheckPasswordValid();

    void showTip(QString str, bool b_ok);
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr re);

    void EnableBtn(bool isEnable);

    Ui::LoginDialog *ui;
    QMap<TipErr,QString> _tip_errs;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    QString _token;
    int _uid;



public slots:
    void slot_forget_pwd();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_login_btn_clicked();
    void slot_tcp_con_finish(bool bsuccess);
signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo);

};

#endif // LOGINDIALOG_H
