#include "losocket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoSocket w;
    w.show();

    return a.exec();
}
