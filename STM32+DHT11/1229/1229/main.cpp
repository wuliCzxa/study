#include "dht11.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dht11 w;
    w.show();
    return a.exec();
}
