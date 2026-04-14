#include "dht11.h"
#include "ui_dht11.h"

dht11::dht11(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dht11)
{
    ui->setupUi(this);

    // --- 1. 初始化串口（去除重复初始化） ---
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &dht11::onSerialPortReadyRead);

    //ui->comboPort->installEventFilter(this);


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

    // X 轴初始范围 0~30s
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
    m_chart->setAnimationOptions(QChart::AllAnimations);
    // 线条抗锯齿已开启，补充 OpenGL 让动画更丝滑
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->setAnimationDuration(300); // 300ms 过渡，比默认更自然

    // 背景美化（静态颜色，已改为你提供的 RGBA）
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

    // --- 6. 绑定到 UI 的 QChartView（控件叫 line_chart）---
    ui->line_chart->setChart(m_chart);
    ui->line_chart->setRenderHint(QPainter::Antialiasing);

    // --- 7. 启用鼠标悬停支持（去重安装 eventFilter）---
    ui->line_chart->setMouseTracking(true);
    ui->line_chart->viewport()->setMouseTracking(true);
    ui->line_chart->installEventFilter(this);

    // --- 8. 轴自动扩展（安全增强，避免空序列越界）---
    connect(m_tempSeries, &QSplineSeries::pointAdded, this, [=](int){
        qreal maxY = m_axisY->max();
        if (m_tempSeries->count() > 0)
            maxY = qMax(maxY, m_tempSeries->at(m_tempSeries->count()-1).y());
        if (m_humSeries->count() > 0)
            maxY = qMax(maxY, m_humSeries->at(m_humSeries->count()-1).y());

        if (maxY > m_axisY->max())
            m_axisY->setMax(maxY + 5);
    });

    // --- 9. 初始化 X 轴滚动窗口定时器（更新图表 1s 追加 + 10s 窗口滚动）---
    QTimer *chartTimer = new QTimer(this);
    connect(chartTimer, &QTimer::timeout, this, [=]() {
        if (m_xPos > 10)
            m_axisX->setRange(m_xPos - 10, m_xPos);
    });
    chartTimer->start(1000);

    // --- 10. 移除标题栏 & 设置密码框隐藏（放在一起，不重复）---
    this->setWindowFlags(Qt::FramelessWindowHint);

    // --- 11. 界面初始化状态（保持你原有逻辑，去重）---
    ui->operator_widget->show();
    ui->charts_widget->hide();
    ui->btn_stoping->hide();

    ui->menu_widget->show();
    ui->menu_widget->raise();
    ui->menu_widget->setFocus();
    ui->login_widget->hide();
    ui->enroll_widget->hide();
    ui->operator_widget->hide();
    ui->charts_widget->hide();

    ui->displayButton->hide();
    ui->displayButton1->hide();
    ui->displayButton2->hide();

    ui->hideButton->show();
    ui->hideButton1->show();
    ui->hideButton2->show();
    ui->Password_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见
    ui->password_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见
    ui->comfirm_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见

    // --- 12. 初始化数据库（你已有功能保留）---
    initDatabase();

    // --- 13. 启动实时时钟（保留原功能）---
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        QDateTime now = QDateTime::currentDateTime();
        ui->time_label->setText(now.toString("yyyy-MM-dd  HH:mm:ss  dddd"));
    });
    timer->start(1000);

    // 构造函数中，添加定时器初始化
    m_labelSwitchTimer = new QTimer(this);
    connect(m_labelSwitchTimer, &QTimer::timeout, this, [=]() {
        if (m_showLabel1) {
            ui->label_dht11_1->hide();
            ui->label_dht11_2->show();
        } else {
            ui->label_dht11_1->show();
            ui->label_dht11_2->hide();
        }
        m_showLabel1 = !m_showLabel1; // 切换状态
    });
    m_labelSwitchTimer->start(500); // 每秒触发一次

    // --- 14. 初始化可用串口列表 ---
    // 首次启动时扫描并填充下拉框
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
        // Qt6 用 globalPosition()
        m_dragPos = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void dht11::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPosition().toPoint() - m_dragPos); event->accept();
    }
}

void dht11::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false; event->accept();
    }
}

void dht11::on_time_label_windowTitleChanged(const QString &title)
{
    static QTimer *timer = nullptr;
    if (!timer)
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]()
                { ui->time_label->setText( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") );
                });
        timer->start(1000);
    }
}

// 页面切换规则
void dht11::on_Button_last_clicked()
{
    // 规则1：如果当前是 login 或 enroll → 显示 menu
    if (ui->login_widget->isVisible() || ui->enroll_widget->isVisible()) {
        ui->menu_widget->show();
        ui->menu_widget->raise();
        ui->menu_widget->setFocus();
        ui->login_widget->hide();
        ui->enroll_widget->hide();
        ui->operator_widget->hide();
        ui->charts_widget->hide();
        return;
    }

    // 规则2：如果当前是 operator → 显示 login
    if (ui->operator_widget->isVisible()) {
        ui->login_widget->show();
        ui->login_widget->raise();
        ui->login_widget->setFocus();
        ui->operator_widget->hide();
        ui->menu_widget->hide();
        ui->charts_widget->hide();
        return;
    }

    // 规则3：如果当前是 charts → 显示 operator
    if (ui->charts_widget->isVisible()) {
        ui->operator_widget->show();
        ui->operator_widget->raise();
        ui->operator_widget->setFocus();
        ui->charts_widget->hide();
        return;
    }

    // 兜底：如果以上都不满足，不做跳转，避免界面逻辑混乱
    qDebug() << "⚠ 未命中任何跳转规则，当前界面状态未知";
}

// 数据库连接
void dht11::initDatabase()
{
    QSqlDatabase db;
    if (QSqlDatabase::contains("user_conn"))
        db = QSqlDatabase::database("user_conn");
    else
        db = QSqlDatabase::addDatabase("QSQLITE", "user_conn");

    // 统一放在可写目录，确保创建和访问是同一个文件
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/users.db");

    if (!db.open()) {
        QMessageBox::critical(this, "数据库错误", "数据库打开失败：" + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE,
            password TEXT,
            enroll_time TEXT
        )
    )");

    db.close();
}

void dht11::on_enroll_pushButton_clicked()
{
    ui->enroll_widget->show();
    ui->enroll_widget->raise();
    ui->enroll_widget->setFocus();
    ui->login_widget->hide();
    ui->menu_widget->hide();
    ui->operator_widget->hide();
    ui->charts_widget->hide();
}

void dht11::on_login_pushButton_clicked()
{
    ui->login_widget->show();
    ui->login_widget->raise();
    ui->login_widget->setFocus();
    ui->enroll_widget->hide();
    ui->menu_widget->hide();
    ui->operator_widget->hide();
    ui->charts_widget->hide();
}

void dht11::on_hideButton_clicked()
{
    ui->hideButton->hide();
    ui->displayButton->show();
    ui->Password_LineEdit->setEchoMode(QLineEdit::Normal); // 密码可见
}

void dht11::on_displayButton_clicked()
{
    ui->displayButton->hide();
    ui->hideButton->show();
    ui->Password_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见
}

void dht11::on_hideButton2_clicked()
{
    ui->hideButton2->hide();
    ui->displayButton2->show();
    ui->password_LineEdit->setEchoMode(QLineEdit::Normal); // 密码可见
}

void dht11::on_hideButton1_clicked()
{
    ui->hideButton1->hide();
    ui->displayButton1->show();
    ui->comfirm_LineEdit->setEchoMode(QLineEdit::Normal); // 密码可见
}

void dht11::on_displayButton2_clicked()
{
    ui->displayButton2->hide();
    ui->hideButton2->show();
    ui->password_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见
}

void dht11::on_displayButton1_clicked()
{
    ui->displayButton1->hide();
    ui->hideButton1->show();
    ui->comfirm_LineEdit->setEchoMode(QLineEdit::Password); // 密码不可见
}

// 登陆操作
void dht11::on_loginButton_clicked()
{
    QString username = ui->Username_LineEdit->text().trimmed();
    QString password = ui->Password_LineEdit->text();
    // 失败情况 ①：输入为空
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空！");
        ui->Username_LineEdit->clear();
        ui->Password_LineEdit->clear();
        return;
    }
    // 连接数据库（复用）
    QSqlDatabase db = QSqlDatabase::database("user_conn");
    if (!db.open())
    {
        QMessageBox::critical(this, "错误", "数据库打开失败！");
        ui->Username_LineEdit->clear();
        ui->Password_LineEdit->clear();
        return;
    }
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    // 失败情况 ②：查询失败
    if (!query.exec())
    {
        QMessageBox::critical(this, "错误", "查询失败！");
        ui->Username_LineEdit->clear();
        ui->Password_LineEdit->clear();
        return; }
    // 失败情况 ③：用户不存在
    if (!query.next())
    {
        QMessageBox::information(this, "提示", "用户不存在！");
        ui->Username_LineEdit->clear();
        ui->Password_LineEdit->clear();
        return;
    }
    // ④：比对密码
    QString dbPass = query.value(0).toString();
    if (password == dbPass)
    {
        QMessageBox::information(this, "成功", "登录成功！");
        ui->enroll_widget->hide();
        ui->login_widget->hide();
        ui->menu_widget->hide();
        ui->charts_widget->hide();
        ui->operator_widget->show();
        ui->operator_widget->raise();
        ui->operator_widget->setFocus();
    } else
    {
        QMessageBox::warning(this, "失败", "密码错误！");
    }
    // 无论成功/失败/存在/不存在都清空所有输入框
    ui->Username_LineEdit->clear();
    ui->Password_LineEdit->clear();
}

// 注册操作
void dht11::on_enrollButton_clicked()
{
    QString username = ui->username_LineEdit->text().trimmed();
    QString password = ui->password_LineEdit->text();
    QString confirm = ui->comfirm_LineEdit->text();
    // 失败情况 ①：密码不一致
    if (password != confirm)
    {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致！");
        ui->username_LineEdit->clear();
        ui->password_LineEdit->clear();
        ui->comfirm_LineEdit->clear(); return;
    }
    // 失败情况 ②：用户名或密码为空
    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空！");
        ui->username_LineEdit->clear();
        ui->password_LineEdit->clear();
        ui->comfirm_LineEdit->clear();
        return;
    }
    // 连接数据库（复用连接，避免重复添加）
    QSqlDatabase db;
    if (QSqlDatabase::contains("user_conn"))
        db = QSqlDatabase::database("user_conn");
    else {
        db = QSqlDatabase::addDatabase("QSQLITE", "user_conn");
        db.setDatabaseName(QCoreApplication::applicationDirPath() + "/users.db");
    } if (!db.open()) {
        QMessageBox::critical(this, "错误", "数据库打开失败！");
        ui->username_LineEdit->clear();
        ui->password_LineEdit->clear();
        ui->comfirm_LineEdit->clear();
        return;
    } QSqlQuery query(db);
    // 失败情况 ③：查询失败
    query.prepare("SELECT username FROM users WHERE username = ?");
    query.addBindValue(username);
    if (!query.exec())
    {
        QMessageBox::critical(this, "错误", "查询数据库失败！");
        ui->username_LineEdit->clear();
        ui->password_LineEdit->clear();
        ui->comfirm_LineEdit->clear(); return;
    }
    // 失败情况 ④：用户已存在
    if (query.next())
    {
        QMessageBox::information(this, "提示", "该用户已存在，请勿重复注册！");
        ui->username_LineEdit->clear();
        ui->password_LineEdit->clear();
        ui->comfirm_LineEdit->clear();
        return;
    }
    // 插入用户
    query.prepare("INSERT INTO users (username, password, enroll_time) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    // 插入成功/失败都要清空
    if (query.exec())
    {
        QMessageBox::information(this, "成功", "注册成功！");
        ui->enroll_widget->hide();
        ui->login_widget->hide();
        ui->menu_widget->hide();
        ui->operator_widget->show();
        ui->operator_widget->raise();
        ui->operator_widget->setFocus();
    } else {
        QMessageBox::critical(this, "失败", "注册信息存储失败！");
    }
    // 无论成功与否清空所有输入框
    ui->username_LineEdit->clear();
    ui->password_LineEdit->clear();
    ui->comfirm_LineEdit->clear();
}

// 按钮点击：控制开始采集/停止采集
void dht11::on_btn_reading_clicked()
{
    // 获取当前选中的纯端口名（存储在 UserData 中）
    QString portName = ui->comboPort->currentData().toString();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "串口错误", "请先选择有效的串口！");
        return;
    }

    // 检查串口当前是否已经打开
    if (m_serialPort->isOpen()) {
        // 停止采集
        m_serialPort->close();
        ui->btn_reading->setText("开始采集");
        ui->btn_reading->show();
        ui->btn_stoping->hide();
        qDebug() << "数据采集已停止";
    }
    else {
        // --- 状态：未运行 -> 执行开始 ---
        // 配置串口参数
        m_serialPort->setPortName(portName);
        m_serialPort->setBaudRate(115200);           // 波特率
        m_serialPort->setDataBits(QSerialPort::Data8);  // 8位数据
        m_serialPort->setParity(QSerialPort::NoParity);    // 无校验
        m_serialPort->setStopBits(QSerialPort::OneStop);   // 1位停止
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

        // 尝试打开串口
        if (m_serialPort->open(QIODevice::ReadWrite))
        {
            // 串口打开成功后暂停端口刷新，避免刷新时干扰通信
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

// 读取串口数据（追加到图表）
void dht11::onSerialPortReadyRead()
{
    // 1. 读取所有数据
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

    // 2. 更新 UI Label（原功能保留）
    if (ui->labelTemperature)
        ui->labelTemperature->setText(tempText);
    if (ui->labelHumidity)
        ui->labelHumidity->setText(humText);

    // 3. 追加到图表（0.15s 更新一次）
    m_tempSeries->append(m_xPos, temperature);
    m_humSeries->append(m_xPos, humidity);
    m_xPos += 1;

    // 4. 保留更长历史数据（5分钟 ≈ 1200点），但只渲染最近10s
    const int maxHistoryPoints = 1200;
    if (m_tempSeries->count() > maxHistoryPoints) m_tempSeries->remove(0);
    if (m_humSeries->count()  > maxHistoryPoints) m_humSeries->remove(0);

    // 5. X 轴初始窗口 10s，之后保持最近10s滚动窗口
    if (m_xPos > 10) {
        m_axisX->setRange(m_xPos - 10, m_xPos);
    }

    // 6. Y 轴自动扩展（安全版，避免越界）
    qreal maxY = m_axisY->max();
    if (m_tempSeries->count() > 0)
        maxY = qMax(maxY, m_tempSeries->at(m_tempSeries->count()-1).y());
    if (m_humSeries->count() > 0)
        maxY = qMax(maxY, m_humSeries->at(m_humSeries->count()-1).y());

    if (maxY > m_axisY->max())
        m_axisY->setMax(maxY + 5);
}

// --- 图表高频平滑更新绘制 ---
void dht11::updateChart()
{
    if (m_tempBuffer.isEmpty()) return;

    // 追加缓存点到曲线
    for (auto &p : m_tempBuffer) m_tempSeries->append(p);
    for (auto &p : m_humBuffer)  m_humSeries->append(p);
    m_tempBuffer.clear();
    m_humBuffer.clear();

    // 只保留最近 10s 视口，但历史不删除
    qreal now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    qreal start = now - 10;
    if (start < 0) start = 0;
    m_axisX->setRange(start, now);

    // Y 轴自动扩展（防止曲线超出后固定不变）
    qreal minY = 999, maxY = -999;
    for (auto &pt : m_tempSeries->points()) {
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }
    for (auto &pt : m_humSeries->points()) {
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }
    if (minY < 999 && maxY > -999) {
        m_axisY->setRange(minY - 5, maxY + 5);
    }
}

// 停止采集更新控件
void dht11::on_btn_stoping_clicked()
{
    // 1. 停止串口数据采集（停止实时读取温湿度）
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        qDebug() << "温湿度实时读取已停止";
    }

    // 2. 串口关闭后重启端口刷新定时器，恢复热插拔检测
    m_portRefreshTimer->start(3000);

    // 3. 切换按钮显示状态
    ui->btn_stoping->hide();  // 隐藏停止采集按钮
    ui->btn_reading->show();  // 显示开始采集按钮
    ui->btn_reading->raise();     // 置顶显示
    ui->btn_reading->setFocus();  // 重新获取焦点

    // 4. 还原按钮文字
    ui->btn_reading->setText("开始采集");
}

// 历史数据汇总
void dht11::on_btn_history_clicked()
{
    ui->charts_widget->show();
    ui->charts_widget->raise();
    ui->charts_widget->setFocus();
    ui->operator_widget->hide();
}

// ============================================================
// 刷新可用串口列表（只显示当前系统中实际存在的端口）
// ============================================================
void dht11::refreshAvailablePorts()
{
    // 如果串口正在使用中，不刷新，避免干扰通信
    if (m_serialPort->isOpen()) return;

    // 记录当前选中的端口名，刷新后尝试恢复选中项
    QString currentPort = ui->comboPort->currentData().toString();

    // 暂时屏蔽信号，防止 clear() 触发 activated 信号
    ui->comboPort->blockSignals(true);
    ui->comboPort->clear();

    const QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();

    if (availablePorts.isEmpty()) {
        // 没有可用端口：显示提示项，并禁用下拉框
        ui->comboPort->addItem("无可用串口");
        ui->comboPort->setEnabled(false);
        ui->comboPort->blockSignals(false);
        qDebug() << "未检测到任何可用串口";
        return;
    }

    ui->comboPort->setEnabled(true);

    int restoreIndex = 0; // 默认选第一项
    for (const QSerialPortInfo &portInfo : availablePorts) {
        // 显示文本：端口名 + 设备描述（方便用户识别 USB 转串口型号等）
        QString displayName = portInfo.portName();
        if (!portInfo.description().isEmpty())
            displayName += "  (" + portInfo.description() + ")";

        // UserData 存纯端口名，取值时不受显示文本影响
        ui->comboPort->addItem(displayName, portInfo.portName());

        // 如果与之前选中的端口一致，记录其索引以便恢复
        if (portInfo.portName() == currentPort)
            restoreIndex = ui->comboPort->count() - 1;
    }

    // 恢复之前的选中项
    ui->comboPort->setCurrentIndex(restoreIndex);

    // 同步更新串口对象的端口名
    QString selectedPort = ui->comboPort->currentData().toString();
    if (!selectedPort.isEmpty())
        m_serialPort->setPortName(selectedPort);

    ui->comboPort->blockSignals(false);

    qDebug() << "串口列表已刷新，共检测到" << availablePorts.size() << "个端口，当前选中:" << selectedPort;
}

// ============================================================
// 串口下拉框选中事件
// ============================================================
void dht11::on_comboPort_activated(int index)
{
    if (index < 0) return;

    // 从 UserData 获取纯端口名（不受显示文本中的描述信息干扰）
    QString portName = ui->comboPort->itemData(index).toString();

    if (portName.isEmpty()) {
        qDebug() << "当前无有效端口可选";
        return;
    }

    // 二次验证：确认该端口在激活时仍然可用（防止拔出后还被选中）
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
        refreshAvailablePorts(); // 立即刷新列表
        return;
    }

    // 保存端口名到串口对象（不自动打开，等待用户点击"开始采集"）
    m_serialPort->setPortName(portName);
    qDebug() << "已选择串口:" << portName;
}
