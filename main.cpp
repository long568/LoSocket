#include "losocket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    QCoreApplication::setOrganizationDomain("cn.long");
#endif
    QCoreApplication::setOrganizationName("long");
    QCoreApplication::setApplicationName("LoSocket");

    QApplication a(argc, argv);
    LoSocket w;
    w.show();
    return a.exec();
}
