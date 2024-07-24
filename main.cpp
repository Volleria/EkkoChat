#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/styleSheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug("Open Success");
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else {
        qDebug("Open Failed");
    }

    MainWindow w;
    w.show();

    return a.exec();
}
