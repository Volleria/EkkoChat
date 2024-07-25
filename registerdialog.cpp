#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pwd_edit->setEchoMode(QLineEdit::Password);
    ui->pwd1_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
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

