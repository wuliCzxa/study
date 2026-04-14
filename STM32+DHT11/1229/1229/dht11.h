#ifndef DHT11_H
#define DHT11_H
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>
#include <QDateTime>
#include <QComboBox>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
// --- 串口头文件 ---
#include <QSerialPort>
#include <QSerialPortInfo>
// --- 图表头文件 ---
#include <QtCharts>
//QT_CHARTS_USE_NAMESPACE
QT_BEGIN_NAMESPACE
namespace Ui {
class dht11;
}
QT_END_NAMESPACE
class dht11 : public QWidget
{
    Q_OBJECT
public:
    dht11(QWidget *parent = nullptr);
    ~dht11();
private slots:
    void on_closeButton_clicked();
    void on_btn_stoping_clicked();
    // --- 串口接收数据的槽函数 ---
    void onSerialPortReadyRead();
    // --- 图表刷新绘制 ---
    void updateChart();
    //bool eventFilter(QObject *watched, QEvent *event);
    void on_comboPort_activated(int index);
    void on_minButton_clicked();
    void on_btn_reading_clicked();
    void on_baudRate_activated(int index);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Ui::dht11 *ui;
    int m_baudRate;               // 波特率
    // --- 串口对象指针 ---
    QSerialPort *m_serialPort;
    // 图表相关
    QChart *m_chart;
    QSplineSeries *m_tempSeries;
    QSplineSeries *m_humSeries;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_xPos = 0;             // X 轴动态推进位置
    QTimer *m_timerChart;         // 用于高频更新曲线，让动画更自然
    QList<QPointF> m_tempBuffer;  // 先缓存数据再绘制（避免一顿一顿）
    QList<QPointF> m_humBuffer;
    // 窗口拖动
    QPoint m_dragPos;
    bool m_dragging = false;
    // --- 串口列表刷新 ---
    void refreshAvailablePorts(); // 扫描并填充当前可用串口到下拉框
    QTimer *m_portRefreshTimer;   // 定时刷新串口列表（支持热插拔）
};
#endif // DHT11_H
