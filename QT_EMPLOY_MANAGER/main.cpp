#include "mainsys.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    MainSys w;
    w.Loggin();
    return a.exec();
}
