#include "mainwidget.h"

#include <QApplication>
#include <SerialPortReader.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SerialPortReader serialReader;

    // 初始化串口（根据你的实际串口名称修改，Windows为COMx，Linux为/dev/ttyUSBx）
    // 示例：Windows系统 COM3，波特率9600
    bool isOpen = serialReader.initSerialPort("COM3",115200);
    // Linux系统示例：serialReader.initSerialPort("/dev/ttyUSB0");

    if (!isOpen) {
        qDebug() << "串口初始化失败，程序退出";
        return -1;
    }

    MainWidget w;
    w.show();
    return a.exec();
}
