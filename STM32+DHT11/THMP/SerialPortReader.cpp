#include "SerialPortReader.h"

SerialPortReader::SerialPortReader(QObject *parent)
    : QObject(parent)
{
    // 初始化串口对象
    m_serialPort = new QSerialPort(this);

    // 绑定数据接收信号与处理函数
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::onSerialPortReadyRead);
    // 绑定错误信号与处理函数
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::onSerialPortErrorOccurred);
}

SerialPortReader::~SerialPortReader()
{
    // 析构时关闭串口
    closeSerialPort();
}

bool SerialPortReader::initSerialPort(const QString &portName, qint32 baudRate,
                                      QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
                                      QSerialPort::StopBits stopBits)
{
    // 如果串口已打开，先关闭
    if (m_serialPort->isOpen()) {
        closeSerialPort();
    }

    // 设置串口参数
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(dataBits);
    m_serialPort->setParity(parity);
    m_serialPort->setStopBits(stopBits);
    // 关闭流控（默认）
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    // 打开串口（读写模式）
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "串口打开成功：" << portName;
        return true;
    } else {
        qDebug() << "串口打开失败：" << m_serialPort->errorString();
        return false;
    }
}

void SerialPortReader::closeSerialPort()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "串口已关闭";
    }
}

bool SerialPortReader::isSerialPortOpen() const
{
    return m_serialPort->isOpen();
}

void SerialPortReader::onSerialPortReadyRead()
{
    QByteArray readData = m_serialPort->readAll();
    if (!readData.isEmpty()) {
        qDebug() << QString::fromUtf8(readData);
    }
}

void SerialPortReader::onSerialPortErrorOccurred(QSerialPort::SerialPortError error)
{
    // 忽略“无错误”和“读取完成”的情况
    if (error == QSerialPort::NoError || error == QSerialPort::ReadError) {
        return;
    }
    qDebug() << "串口错误：" << m_serialPort->errorString();
    // 错误发生时关闭串口
    closeSerialPort();
}
