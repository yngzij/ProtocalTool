#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include "nicmsg.h"
#include "singleton.h"
#include "netmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    Singleton<NICMsg>::Instance().init();
    Singleton<NetModel>::Instance().init();

    MainWindow w;
    w.show();

    return a.exec();
}
