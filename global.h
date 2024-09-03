#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include "QStyle"
#include <QRegularExpression>
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>

/**
 * @brief repolish: 用来刷新qss
 */
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;
extern QString gate_url_prefix;

enum ReqId
{
    ID_GET_VARIFY_CODE = 1001, // 获取验证码
    ID_REG_USER = 1002, // 注册用户
    ID_RESET_PWD = 1003,
};

enum Modules
{
    REGISTERMOD = 0,
    RESETMOD = 1,
};

enum ErrorCodes
{
    SUCCESS = 0,
    ERR_JSON = 1, // JSON解析失败
    ERR_NETWORK = 2, // 网络错误
};

enum TipErr
{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6
};
enum ClickLabelState
{
    Normal = 0,
    Selected = 1
};



#endif // GLOBAL_H
