#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :QDialog(parent),ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    ui->pass_show->SetState("unvisible","unvisible_hover","","visible","visible_hover","");


    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

    connect(ui->pass_show, &ClickedLabel::clicked,this,[this](){
       auto state =  ui->pass_show->GetCurState();
       if(state == ClickLabelState::Normal)
       {
           ui->pass_edit->setEchoMode(QLineEdit::Password);
       }
       else if(state == ClickLabelState::Selected)
       {
           ui->pass_edit->setEchoMode(QLineEdit::Normal);
       }
    });

    initHead();
    initHttpHandlers();

    connect(HttpManager::GetInstance().get(), &HttpManager::sig_login_mod_finish, this,
            &LoginDialog::slot_login_mod_finish);
    connect(this, &LoginDialog::sig_connect_tcp, TcpManager::GetInstance().get(),&TcpManager::slot_tcp_connect);
    connect(TcpManager::GetInstance().get(),&TcpManager::sig_con_success,this,&LoginDialog::slot_tcp_con_finish);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initHttpHandlers()
{
    _handlers.insert(ReqId::ID_LOGIN_USER,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"),false);
            EnableBtn(false);
            return;
        }

        auto email = jsonObj["email"].toString();

        ServerInfo si;
        si.uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.uid;
        _token = si.Token;

        emit sig_connect_tcp(si);
    });

}

void LoginDialog::initHead()
{
    // 加载图片
    QPixmap originalPixmap(":/res/ice.png");

    originalPixmap = originalPixmap.scaled(ui->head_label->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 创建一个和原始图片相同大小的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent); // 用透明色填充

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 使用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10); // 最后两个参数分别是x和y方向的圆角半径
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap上
    painter.drawPixmap(0, 0, originalPixmap);

    // 设置绘制好的圆角图片到QLabel上
    ui->head_label->setPixmap(roundedPixmap);

}

bool LoginDialog::CheckEmailValid()
{
    auto email = ui->email_edit->text();
    if(email.isEmpty())
    {
        AddTipErr(TipErr::TIP_EMAIL_ERR,tr("邮箱不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::CheckPasswordValid()
{
    auto password = ui->pass_edit->text();

    if(password.length() < 6 || password.length()>15)
    {
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(password).hasMatch();
    if(!match)
    {
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;

}

void LoginDialog::slot_forget_pwd()
{
    emit switchReset();
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }

    // 解析 JSON 字符串,res需转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());

    //json解析错误
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析错误"),false);
        return;
    }
    //json解析错误
    if(!jsonDoc.isObject())
    {
        showTip(tr("json解析错误"),false);
        return;
    }

    //调用对应的逻辑,根据id回调。
    _handlers[id](jsonDoc.object());
    return;
}


void LoginDialog::on_login_btn_clicked()
{
    qDebug()<<"login btn clicked";
    if(CheckEmailValid() == false)
    {
        return;
    }
    if(CheckPasswordValid() == false)
    {
        return ;
    }

    EnableBtn(false);

    auto email = ui->email_edit->text();
    auto pwd = ui->pass_edit->text();

    //发送http请求登录
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);
    HttpManager::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj, ReqId::ID_LOGIN_USER,Modules::LOGINMOD);

}

void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{
    if(bsuccess)
    {
        showTip(tr("服务器连接成功，正在登录..."),true);
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;

        QJsonDocument doc(jsonObj);
        QString jsonString = doc.toJson(QJsonDocument::Indented);

        TcpManager::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN,jsonString);
    }
    else
    {
        showTip(tr("网络异常"),false);
        EnableBtn(true);

    }

}

void LoginDialog::showTip(QString str,bool b_ok)
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

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);

    if(_tip_errs.empty())
    {
      ui->err_tip->clear();
      return;
    }

    showTip(_tip_errs.first(), false);
}

void LoginDialog::EnableBtn(bool isEnable)
{
    ui->login_btn->setEnabled(isEnable);
    ui->reg_btn->setEnabled(isEnable);
}

