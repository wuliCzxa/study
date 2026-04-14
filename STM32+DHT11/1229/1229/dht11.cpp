#include "dht11.h"
#include "ui_dht11.h"

dht11::dht11(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dht11)
{
    ui->setupUi(this);

    // 设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint);

    // 设置工具提示
    ui->closeButton->setToolTip("Close");
    //ui->btn_minimize->setToolTip("Minimize");


    // --- 1. 初始化串口 ---
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &dht11::onSerialPortReadyRead);


    // --- 2. 初始化折线图 ---
    m_chart = new QChart();

    // 设置标题字体样式（爱奇艺黑体 23pt）
    QFont titleFont;
    titleFont.setFamily("爱奇艺黑体 Medium");
    titleFont.setPointSize(23);
    titleFont.setWeight(QFont::Medium);
    m_chart->setTitleFont(titleFont);
    m_chart->setTitleBrush(QBrush(QColor(69, 69, 103)));
    m_chart->setTitle("温湿度实时检测曲线");

    // --- 3. 初始化曲线序列 ---
    m_tempSeries = new QSplineSeries();
    m_humSeries  = new QSplineSeries();

    m_tempSeries->setName("Temperature (°C)");
    m_humSeries->setName("Humidity (%)");

    m_chart->addSeries(m_tempSeries);
    m_chart->addSeries(m_humSeries);

    // --- 4. 初始化坐标轴 ---
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();

    // X 轴初始范围 0~15s
    m_axisX->setRange(0, 15);

    // Y 轴初始范围（后续自动扩展）
    m_axisY->setRange(0, 100);

    QFont axisFont;
    axisFont.setFamily("隶书");
    axisFont.setPointSize(10);

    m_axisX->setTitleFont(axisFont);
    m_axisY->setTitleFont(axisFont);
    m_axisX->setLabelsFont(axisFont);
    m_axisY->setLabelsFont(axisFont);

    QBrush axisBrush(QColor(85, 85, 127));
    m_axisX->setTitleBrush(axisBrush);
    m_axisY->setTitleBrush(axisBrush);
    m_axisX->setLabelsBrush(axisBrush);
    m_axisY->setLabelsBrush(axisBrush);

    m_axisX->setLabelFormat("%.1f");
    m_axisY->setLabelFormat("%.1f");
    m_axisX->setTitleText("Time (s)");
    m_axisY->setTitleText("Value");

    // 网格
    m_axisX->setGridLineVisible(true);
    m_axisY->setGridLineVisible(true);
    m_axisX->setMinorGridLineVisible(true);
    m_axisY->setMinorGridLineVisible(true);
    m_axisX->setTickCount(16);
    m_axisY->setTickCount(11);

    // --- 5. 绑定坐标轴 ---
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_tempSeries->attachAxis(m_axisX);
    m_tempSeries->attachAxis(m_axisY);
    m_humSeries->attachAxis(m_axisX);
    m_humSeries->attachAxis(m_axisY);

    // 动画
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->setAnimationDuration(300); // 300ms 过渡，比默认更自然

    // 背景美化
    QColor bgColor;
    bgColor.setRgb(244, 239, 231);
    bgColor.setAlpha(100);

    m_chart->setBackgroundVisible(true);
    m_chart->setBackgroundRoundness(12);
    m_chart->setBackgroundBrush(QBrush(bgColor));
    m_chart->setPlotAreaBackgroundVisible(true);
    m_chart->setPlotAreaBackgroundBrush(QBrush(bgColor));
    m_chart->setPlotAreaBackgroundPen(QPen(Qt::NoPen));

    // 图例
    m_chart->legend()->setFont(axisFont);
    m_chart->legend()->setLabelBrush(axisBrush);
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignTop);

    // --- 6. 绑定到 UI 的 QChartView ---
    ui->line_chart->setChart(m_chart);
    ui->line_chart->setRenderHint(QPainter::Antialiasing);

    // --- 7. 启用鼠标悬停支持 ---
    ui->line_chart->setMouseTracking(true);
    ui->line_chart->viewport()->setMouseTracking(true);
    ui->line_chart->installEventFilter(this);

    // --- 8. 轴自动扩展 ---
    connect(m_tempSeries, &QSplineSeries::pointAdded, this, [=](int){
        qreal maxY = m_axisY->max();
        if (m_tempSeries->count() > 0)
            maxY = qMax(maxY, m_tempSeries->at(m_tempSeries->count()-1).y());
        if (m_humSeries->count() > 0)
            maxY = qMax(maxY, m_humSeries->at(m_humSeries->count()-1).y());
        if (maxY > m_axisY->max())
            m_axisY->setMax(maxY + 5);
    });

    // --- 9. X 轴滚动窗口定时器 ---
    QTimer *chartTimer = new QTimer(this);
    connect(chartTimer, &QTimer::timeout, this, [=]() {
        if (m_xPos > 10)
            m_axisX->setRange(m_xPos - 10, m_xPos);
    });
    chartTimer->start(1000);

    // --- 10. 界面初始化状态 ---
    ui->operator_widget->show();
    ui->btn_stoping->hide();

    // --- 11. 启动实时时钟 ---
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        QDateTime now = QDateTime::currentDateTime();
        ui->time_label->setText(now.toString("yyyy-MM-dd  HH:mm:ss  dddd"));
    });
    timer->start(1000);

    // --- 12. 初始化可用串口列表 ---
    // 首次启动扫描填充下拉框
    refreshAvailablePorts();

    // 每 3 秒自动刷新一次，支持 USB 转串口热插拔
    m_portRefreshTimer = new QTimer(this);
    connect(m_portRefreshTimer, &QTimer::timeout, this, &dht11::refreshAvailablePorts);
    m_portRefreshTimer->start(3000);
}

dht11::~dht11()
{
    delete ui;
}

void dht11::on_closeButton_clicked()
{
    this->close();
}

void dht11::on_minButton_clicked()
{
    this->showMinimized();
}

void dht11::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void dht11::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPos);
        event->accept();
    }
}

void dht11::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}

// ============================================================
// 刷新可用串口列表（只显示当前系统中实际存在的端口）
// ============================================================
void dht11::refreshAvailablePorts()
{
    // 串口正在使用中时不刷新，避免干扰通信
    if (m_serialPort->isOpen()) return;

    // 记录当前选中的端口名，刷新后尝试恢复
    QString currentPort = ui->comboPort->currentData().toString();

    // 屏蔽信号，防止 clear() 误触 activated
    ui->comboPort->blockSignals(true);
    ui->comboPort->clear();

    const QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    if (availablePorts.isEmpty()) {
        ui->comboPort->addItem("无可用串口");
        ui->comboPort->setEnabled(false);
        ui->comboPort->blockSignals(false);
        qDebug() << "未检测到任何可用串口";
        return;
    }

    ui->comboPort->setEnabled(true);

    int restoreIndex = 0; // 默认选第一项
    for (const QSerialPortInfo &portInfo : availablePorts) {
        // 显示文本和 UserData 均为纯端口名，如 COM4、COM7
        ui->comboPort->addItem(portInfo.portName(), portInfo.portName());

        // 记录之前选中端口的索引以便恢复
        if (portInfo.portName() == currentPort)
            restoreIndex = ui->comboPort->count() - 1;
    }

    ui->comboPort->setCurrentIndex(restoreIndex);

    // 同步更新串口对象的端口名
    QString selectedPort = ui->comboPort->currentData().toString();
    if (!selectedPort.isEmpty())
        m_serialPort->setPortName(selectedPort);

    ui->comboPort->blockSignals(false);

    qDebug() << "串口列表已刷新，共" << availablePorts.size() << "个，当前选中:" << selectedPort;
}

// ============================================================
// 串口下拉框选中事件
// ============================================================
void dht11::on_comboPort_activated(int index)
{
    if (index < 0) return;

    // 从 UserData 获取纯端口名
    QString portName = ui->comboPort->itemData(index).toString();

    if (portName.isEmpty()) {
        qDebug() << "当前无有效端口可选";
        return;
    }

    // 二次验证：确认激活时端口仍然可用
    bool isStillAvailable = false;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        if (info.portName() == portName) {
            isStillAvailable = true;
            break;
        }
    }

    if (!isStillAvailable) {
        QMessageBox::warning(this, "端口不可用",
                             QString("串口 %1 已断开，请重新选择。").arg(portName));
        refreshAvailablePorts();
        return;
    }

    // 保存端口名（不自动打开，等待"开始采集"）
    m_serialPort->setPortName(portName);
    qDebug() << "已选择串口:" << portName;
}

// ============================================================
// 开始/停止采集
// ============================================================
void dht11::on_btn_reading_clicked()
{
    // 从 UserData 获取纯端口名
    QString portName = ui->comboPort->currentData().toString();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "串口错误", "请先选择有效的串口！");
        return;
    }

    if (m_serialPort->isOpen()) {
        // 停止采集
        m_serialPort->close();
        // 串口关闭后恢复端口刷新
        m_portRefreshTimer->start(3000);
        ui->btn_reading->setText("开始采集");
        ui->btn_reading->show();
        ui->btn_stoping->hide();
        qDebug() << "数据采集已停止";
    } else {
        // 配置串口参数
        m_serialPort->setPortName(portName);
        m_serialPort->setBaudRate(m_baudRate);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

        if (m_serialPort->open(QIODevice::ReadWrite)) {
            // 串口打开后暂停刷新，避免采集中途列表变动
            m_portRefreshTimer->stop();
            ui->btn_reading->setText("停止采集");
            ui->btn_reading->hide();
            ui->btn_stoping->show();
            ui->btn_stoping->raise();
            ui->btn_stoping->setFocus();
            qDebug() << "串口打开成功，开始采集:" << portName;
        } else {
            QMessageBox::critical(this, "串口错误",
                                  QString("无法打开串口 %1！\n请检查设备是否连接或被占用。").arg(portName));
        }
    }
}

// ============================================================
// 停止采集
// ============================================================
void dht11::on_btn_stoping_clicked()
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "温湿度实时读取已停止";
    }

    // 恢复端口刷新定时器
    m_portRefreshTimer->start(3000);

    ui->btn_stoping->hide();
    ui->btn_reading->show();
    ui->btn_reading->raise();
    ui->btn_reading->setFocus();
    ui->btn_reading->setText("开始采集");
}

// ============================================================
// 读取串口数据（追加到图表）
// ============================================================
void dht11::onSerialPortReadyRead()
{
    QByteArray readData = m_serialPort->readAll();
    if (readData.isEmpty()) return;

    QString th = QString::fromUtf8(readData).trimmed();
    QList<QString> parts = th.split(' ');
    if (parts.size() < 2) return;

    bool ok1, ok2;
    qreal temperature = parts[0].toDouble(&ok1);
    qreal humidity    = parts[1].toDouble(&ok2);
    if (!ok1 || !ok2) return;

    QString tempText = parts[0];
    QString humText  = parts[1];

    qDebug() << "温度:" << tempText << " 湿度:" << humText;

    if (ui->labelTemperature) ui->labelTemperature->setText(tempText);
    if (ui->labelHumidity)    ui->labelHumidity->setText(humText);

    m_tempSeries->append(m_xPos, temperature);
    m_humSeries->append(m_xPos, humidity);
    m_xPos += 1;

    const int maxHistoryPoints = 1200;
    if (m_tempSeries->count() > maxHistoryPoints) m_tempSeries->remove(0);
    if (m_humSeries->count()  > maxHistoryPoints) m_humSeries->remove(0);

    if (m_xPos > 10)
        m_axisX->setRange(m_xPos - 10, m_xPos);

    qreal maxY = m_axisY->max();
    if (m_tempSeries->count() > 0)
        maxY = qMax(maxY, m_tempSeries->at(m_tempSeries->count()-1).y());
    if (m_humSeries->count() > 0)
        maxY = qMax(maxY, m_humSeries->at(m_humSeries->count()-1).y());
    if (maxY > m_axisY->max())
        m_axisY->setMax(maxY + 5);
}

// ============================================================
// 图表高频平滑更新绘制
// ============================================================
void dht11::updateChart()
{
    if (m_tempBuffer.isEmpty()) return;

    for (auto &p : m_tempBuffer) m_tempSeries->append(p);
    for (auto &p : m_humBuffer)  m_humSeries->append(p);
    m_tempBuffer.clear();
    m_humBuffer.clear();

    qreal now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    qreal start = now - 10;
    if (start < 0) start = 0;
    m_axisX->setRange(start, now);

    qreal minY = 999, maxY = -999;
    for (auto &pt : m_tempSeries->points()) {
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }
    for (auto &pt : m_humSeries->points()) {
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }
    if (minY < 999 && maxY > -999)
        m_axisY->setRange(minY - 5, maxY + 5);
}

// ============================================================
// 波特率选择
// ============================================================
void dht11::on_baudRate_activated(int index)
{
    if (index < 0) return;

    QString baudText = ui->baudRate->itemText(index);
    bool ok;
    int baud = baudText.toInt(&ok);
    if (ok)
        m_baudRate = baud;
}
