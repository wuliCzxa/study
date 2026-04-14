#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QByteArray>

class SerialPortReader : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortReader(QObject *parent = nullptr);
    ~SerialPortReader();

    // 初始化串口参数并打开串口
    bool initSerialPort(const QString &portName,         // 串口名称（如COM3、/dev/ttyUSB0）
                        qint32 baudRate = QSerialPort::Baud115200,  // 波特率，默认9600
                        QSerialPort::DataBits dataBits = QSerialPort::Data8, // 数据位，默认8位
                        QSerialPort::Parity parity = QSerialPort::NoParity,  // 校验位，默认无
                        QSerialPort::StopBits stopBits = QSerialPort::OneStop); // 停止位，默认1位

    // 关闭串口
    void closeSerialPort();

    // 获取当前串口状态
    bool isSerialPortOpen() const;

private slots:
    // 串口数据接收槽函数
    void onSerialPortReadyRead();

    // 串口错误处理槽函数
    void onSerialPortErrorOccurred(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort; // 串口对象
};

#endif // SERIALPORTREADER_H
