#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmanager.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    ui->pwd1_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
    connect(HttpManager::GetInstance().get(), &HttpManager::sig_reg_mod_finish,
            this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getCode_btn_clicked()
{
    // 匹配邮箱
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match)
    {
        // 发送http验证码
    }
    else
    {
        showTip(tr("邮箱地址不正确"),false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }

    // 解析 JSON 字符串， (反)序列化 QString -> ByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());

    if(jsonDoc.isNull())
    {
        showTip(tr("json解析失败"),false);
        return;
    }

    // json 解析错误
    if(!jsonDoc.isObject())
    {
        showTip(tr("json解析失败"),false);
        return;
    }


    _handlers[id](jsonDoc.object());
    return;

}

void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"),false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，注意查收"),true);
        qDebug() << "email is " << email;
    });
}

void RegisterDialog::showTip(QString str,bool b_ok)
{
    if(b_ok)
    {
        ui->err_tip->setProperty("state","normal");
    }
    else
    {
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

