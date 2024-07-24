#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dialog = new LoginDialog();
    setCentralWidget(_login_dialog);
    _login_dialog->show();


    // 创建和注册 消息链接
    connect(_login_dialog, &LoginDialog::switchRegister,this, &MainWindow::SlotSwitchReg);
    _reg_dialog = new RegisterDialog();

}

MainWindow::~MainWindow()
{
    delete ui;
    if(_login_dialog)
    {
        delete _login_dialog;
        _login_dialog = nullptr;
    }
    if(_reg_dialog)
    {
        delete _reg_dialog;
        _reg_dialog = nullptr;
    }
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_reg_dialog);
    _login_dialog->hide();
    _reg_dialog->show();
}

