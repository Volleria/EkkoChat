#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dialog = new LoginDialog(this);
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(_login_dialog);


    // 创建和注册消息链接
    connect(_login_dialog, &LoginDialog::switchRegister,this, &MainWindow::SlotSwitchReg);

}

MainWindow::~MainWindow()
{
    delete ui;
//    if(_login_dialog)
//    {
//        delete _login_dialog;
//        _login_dialog = nullptr;
//    }
//    if(_reg_dialog)
//    {
//        delete _reg_dialog;
//        _reg_dialog = nullptr;
//    }
}

void MainWindow::SlotSwitchReg()
{
    _reg_dialog = new RegisterDialog(this);

    // 登录和注册嵌入主界面
    _reg_dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_reg_dialog, &RegisterDialog::sigSwitchLogin,this,&MainWindow::SlotSwitchLogin);
    setCentralWidget(_reg_dialog);
    _login_dialog->hide();
    _reg_dialog->show();
}

void MainWindow::SlotSwitchLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    _login_dialog = new LoginDialog(this);
    _login_dialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_login_dialog);
   _reg_dialog->hide();
    _login_dialog->show();

    //连接登录界面注册信号
    connect(_login_dialog, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);

    //连接登录界面忘记密码信号
    connect(_login_dialog, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);

}

void MainWindow::SlotSwitchReset()
{
    _reset_

}

