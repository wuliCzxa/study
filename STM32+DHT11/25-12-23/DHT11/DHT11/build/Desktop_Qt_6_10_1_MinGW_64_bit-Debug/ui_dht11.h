/********************************************************************************
** Form generated from reading UI file 'dht11.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DHT11_H
#define UI_DHT11_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dht11
{
public:
    QLabel *time_label;
    QPushButton *closeButton;
    QPushButton *minButton;
    QLabel *background;
    QWidget *operator_widget;
    QPushButton *btn_reading;
    QLabel *label_background;
    QPushButton *btn_history;
    QListView *time_list;
    QLabel *label_dht11_1;
    QLabel *labelTemperature;
    QLabel *Temperature_position;
    QLabel *Humidity_position;
    QLabel *labelHumidity;
    QLabel *Humidity;
    QLabel *Temperature;
    QPushButton *btn_stoping;
    QComboBox *comboPort;
    QLabel *Temperature_2;
    QLabel *label_dht11_2;
    QPushButton *Button_last;
    QWidget *charts_widget;
    QLabel *label_background_3;
    QChartView *line_chart;
    QWidget *login_widget;
    QLabel *label_Password;
    QLabel *label_Username;
    QLineEdit *Username_LineEdit;
    QLineEdit *Password_LineEdit;
    QLabel *label_4;
    QPushButton *loginButton;
    QPushButton *displayButton;
    QPushButton *hideButton;
    QWidget *enroll_widget;
    QLabel *label_password;
    QPushButton *enrollButton;
    QLineEdit *password_LineEdit;
    QLabel *label_comfirm;
    QLabel *label_username;
    QLineEdit *username_LineEdit;
    QLineEdit *comfirm_LineEdit;
    QLabel *label_2;
    QPushButton *displayButton2;
    QPushButton *hideButton2;
    QPushButton *displayButton1;
    QPushButton *hideButton1;
    QWidget *menu_widget;
    QLabel *label;
    QPushButton *login_pushButton;
    QPushButton *enroll_pushButton;

    void setupUi(QWidget *dht11)
    {
        if (dht11->objectName().isEmpty())
            dht11->setObjectName("dht11");
        dht11->resize(810, 550);
        time_label = new QLabel(dht11);
        time_label->setObjectName("time_label");
        time_label->setGeometry(QRect(60, -10, 351, 51));
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\245\267\344\275\223")});
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        time_label->setFont(font);
        time_label->setStyleSheet(QString::fromUtf8("#time_label {\n"
"    font-size: 13px;\n"
"	font: 16pt \"\346\245\267\344\275\223\";\n"
"    color:  rgb(0, 0, 60);\n"
"    border-radius: 10px;\n"
"    padding: 10px;\n"
"    qproperty-alignment: AlignCenter;\n"
"}\n"
""));
        closeButton = new QPushButton(dht11);
        closeButton->setObjectName("closeButton");
        closeButton->setGeometry(QRect(770, 0, 41, 28));
        closeButton->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"	background-color: rgba(255, 0, 0, 100);\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditClear));
        closeButton->setIcon(icon);
        closeButton->setIconSize(QSize(26, 26));
        closeButton->setAutoExclusive(true);
        minButton = new QPushButton(dht11);
        minButton->setObjectName("minButton");
        minButton->setGeometry(QRect(730, 0, 41, 28));
        minButton->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgba(244, 239, 231,220);\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
        minButton->setIcon(icon1);
        minButton->setIconSize(QSize(22, 22));
        background = new QLabel(dht11);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 810, 550));
        background->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/parent.jpg")));
        background->setScaledContents(true);
        operator_widget = new QWidget(dht11);
        operator_widget->setObjectName("operator_widget");
        operator_widget->setGeometry(QRect(0, 29, 810, 521));
        btn_reading = new QPushButton(operator_widget);
        btn_reading->setObjectName("btn_reading");
        btn_reading->setGeometry(QRect(170, 400, 111, 41));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_reading->sizePolicy().hasHeightForWidth());
        btn_reading->setSizePolicy(sizePolicy);
        btn_reading->setMinimumSize(QSize(0, 0));
        btn_reading->setMaximumSize(QSize(16777215, 16777215));
        btn_reading->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\345\215\216\346\226\207\346\226\260\351\255\217\";\n"
"	border: 1px solid #A7A8B6;\n"
"	border-radius: 8px;\n"
"	background-color: rgb(255, 179, 23);\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgb(255, 212, 123);\n"
"	border: 1px solid #726f6b;\n"
"}\n"
"QPushButton:pressed {	\n"
"	border: 1px solid #A7A8B6;\n"
"}"));
        label_background = new QLabel(operator_widget);
        label_background->setObjectName("label_background");
        label_background->setGeometry(QRect(0, 0, 810, 521));
        label_background->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        label_background->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/background.jpg")));
        label_background->setScaledContents(true);
        btn_history = new QPushButton(operator_widget);
        btn_history->setObjectName("btn_history");
        btn_history->setGeometry(QRect(540, 90, 111, 41));
        sizePolicy.setHeightForWidth(btn_history->sizePolicy().hasHeightForWidth());
        btn_history->setSizePolicy(sizePolicy);
        btn_history->setMinimumSize(QSize(0, 0));
        btn_history->setMaximumSize(QSize(16777215, 16777215));
        btn_history->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\345\215\216\346\226\207\346\226\260\351\255\217\";\n"
"	border: 1px solid #A7A8B6;\n"
"	border-radius: 8px;\n"
"	background-color: rgb(255, 179, 23);\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgb(255, 212, 123);\n"
"	border: 1px solid #726f6b;\n"
"}\n"
"QPushButton:pressed {	\n"
"	border: 1px solid #A7A8B6;\n"
"}"));
        time_list = new QListView(operator_widget);
        time_list->setObjectName("time_list");
        time_list->setGeometry(QRect(80, 128, 321, 251));
        time_list->setStyleSheet(QString::fromUtf8("#time_list {\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-top-left-radius: 18px;\n"
"    border-bottom-right-radius: 18px;\n"
"    padding: 5px;\n"
"}\n"
""));
        label_dht11_1 = new QLabel(operator_widget);
        label_dht11_1->setObjectName("label_dht11_1");
        label_dht11_1->setGeometry(QRect(450, 150, 281, 279));
        label_dht11_1->setStyleSheet(QString::fromUtf8("#label {\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-top-left-radius: 18px;\n"
"    border-bottom-right-radius: 18px;\n"
"    padding: 5px;\n"
"}\n"
""));
        label_dht11_1->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/111.png")));
        label_dht11_1->setScaledContents(true);
        labelTemperature = new QLabel(operator_widget);
        labelTemperature->setObjectName("labelTemperature");
        labelTemperature->setGeometry(QRect(230, 150, 71, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\345\247\232\344\275\223")});
        font1.setPointSize(45);
        font1.setBold(false);
        font1.setItalic(false);
        labelTemperature->setFont(font1);
        labelTemperature->setStyleSheet(QString::fromUtf8("color:rgba(31, 31, 46, 226);\n"
"font: 45pt \"\346\226\271\346\255\243\345\247\232\344\275\223\";"));
        Temperature_position = new QLabel(operator_widget);
        Temperature_position->setObjectName("Temperature_position");
        Temperature_position->setGeometry(QRect(300, 160, 61, 61));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("\351\232\266\344\271\246")});
        font2.setPointSize(23);
        font2.setBold(false);
        font2.setItalic(false);
        Temperature_position->setFont(font2);
        Temperature_position->setStyleSheet(QString::fromUtf8("font: 23pt \"\351\232\266\344\271\246\";\n"
"color: rgb(85, 85, 127);"));
        Humidity_position = new QLabel(operator_widget);
        Humidity_position->setObjectName("Humidity_position");
        Humidity_position->setGeometry(QRect(300, 270, 91, 61));
        Humidity_position->setFont(font2);
        Humidity_position->setStyleSheet(QString::fromUtf8("font: 23pt \"\351\232\266\344\271\246\";\n"
"color: rgb(85, 85, 127);"));
        labelHumidity = new QLabel(operator_widget);
        labelHumidity->setObjectName("labelHumidity");
        labelHumidity->setGeometry(QRect(230, 260, 71, 71));
        labelHumidity->setFont(font1);
        labelHumidity->setStyleSheet(QString::fromUtf8("color:rgba(31, 31, 46, 226);\n"
"font: 45pt \"\346\226\271\346\255\243\345\247\232\344\275\223\";"));
        Humidity = new QLabel(operator_widget);
        Humidity->setObjectName("Humidity");
        Humidity->setGeometry(QRect(100, 260, 101, 71));
        Humidity->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";\n"
"color: rgb(69, 69, 103);"));
        Temperature = new QLabel(operator_widget);
        Temperature->setObjectName("Temperature");
        Temperature->setGeometry(QRect(100, 150, 111, 71));
        Temperature->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";\n"
"color: rgb(69, 69, 103);"));
        btn_stoping = new QPushButton(operator_widget);
        btn_stoping->setObjectName("btn_stoping");
        btn_stoping->setGeometry(QRect(170, 400, 111, 41));
        sizePolicy.setHeightForWidth(btn_stoping->sizePolicy().hasHeightForWidth());
        btn_stoping->setSizePolicy(sizePolicy);
        btn_stoping->setMinimumSize(QSize(0, 0));
        btn_stoping->setMaximumSize(QSize(16777215, 16777215));
        btn_stoping->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\345\215\216\346\226\207\346\226\260\351\255\217\";\n"
"	border: 1px solid #A7A8B6;\n"
"	border-radius: 8px;\n"
"	background-color: rgb(255, 179, 23);\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgb(255, 212, 123);\n"
"	border: 1px solid #726f6b;\n"
"}\n"
"QPushButton:pressed {	\n"
"	border: 1px solid #A7A8B6;\n"
"}"));
        comboPort = new QComboBox(operator_widget);
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->setObjectName("comboPort");
        comboPort->setGeometry(QRect(230, 70, 171, 41));
        comboPort->setStyleSheet(QString::fromUtf8("#comboBox {\n"
"	color: rgb(41, 41, 41);\n"
"	font: 14pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-top-left-radius: 10px;\n"
"	border-bottom-left-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
""));
        Temperature_2 = new QLabel(operator_widget);
        Temperature_2->setObjectName("Temperature_2");
        Temperature_2->setGeometry(QRect(170, 60, 61, 61));
        Temperature_2->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";\n"
"color: rgb(69, 69, 103);"));
        label_dht11_2 = new QLabel(operator_widget);
        label_dht11_2->setObjectName("label_dht11_2");
        label_dht11_2->setGeometry(QRect(450, 150, 281, 279));
        label_dht11_2->setStyleSheet(QString::fromUtf8("#label {\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-top-left-radius: 18px;\n"
"    border-bottom-right-radius: 18px;\n"
"    padding: 5px;\n"
"}\n"
""));
        label_dht11_2->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/000.png")));
        label_dht11_2->setScaledContents(true);
        label_background->raise();
        btn_reading->raise();
        btn_history->raise();
        time_list->raise();
        label_dht11_1->raise();
        labelTemperature->raise();
        Temperature_position->raise();
        Humidity_position->raise();
        labelHumidity->raise();
        Humidity->raise();
        Temperature->raise();
        btn_stoping->raise();
        comboPort->raise();
        Temperature_2->raise();
        label_dht11_2->raise();
        Button_last = new QPushButton(dht11);
        Button_last->setObjectName("Button_last");
        Button_last->setGeometry(QRect(690, 0, 40, 25));
        Button_last->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgba(244, 239, 231,220);\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
        Button_last->setIcon(icon2);
        Button_last->setIconSize(QSize(22, 22));
        charts_widget = new QWidget(dht11);
        charts_widget->setObjectName("charts_widget");
        charts_widget->setGeometry(QRect(0, 29, 810, 521));
        label_background_3 = new QLabel(charts_widget);
        label_background_3->setObjectName("label_background_3");
        label_background_3->setGeometry(QRect(0, 0, 810, 521));
        label_background_3->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        label_background_3->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/background.jpg")));
        label_background_3->setScaledContents(true);
        line_chart = new QChartView(charts_widget);
        line_chart->setObjectName("line_chart");
        line_chart->setGeometry(QRect(79, 79, 651, 361));
        line_chart->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        login_widget = new QWidget(dht11);
        login_widget->setObjectName("login_widget");
        login_widget->setGeometry(QRect(0, 29, 810, 521));
        label_Password = new QLabel(login_widget);
        label_Password->setObjectName("label_Password");
        label_Password->setGeometry(QRect(30, 230, 105, 47));
        label_Password->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";"));
        label_Username = new QLabel(login_widget);
        label_Username->setObjectName("label_Username");
        label_Username->setGeometry(QRect(30, 110, 103, 47));
        label_Username->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";"));
        Username_LineEdit = new QLineEdit(login_widget);
        Username_LineEdit->setObjectName("Username_LineEdit");
        Username_LineEdit->setGeometry(QRect(130, 110, 261, 45));
        Username_LineEdit->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223\";"));
        Password_LineEdit = new QLineEdit(login_widget);
        Password_LineEdit->setObjectName("Password_LineEdit");
        Password_LineEdit->setGeometry(QRect(130, 230, 261, 45));
        Password_LineEdit->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223\";"));
        label_4 = new QLabel(login_widget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(0, 0, 810, 521));
        label_4->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/login.jpg")));
        label_4->setScaledContents(true);
        loginButton = new QPushButton(login_widget);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(160, 340, 153, 49));
        loginButton->setStyleSheet(QString::fromUtf8("\n"
"                    font: 17pt \"\345\215\216\346\226\207\346\245\267\344\275\223\";\n"
"                "));
        displayButton = new QPushButton(login_widget);
        displayButton->setObjectName("displayButton");
        displayButton->setGeometry(QRect(350, 240, 41, 28));
        displayButton->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/jpeg/display.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        displayButton->setIcon(icon3);
        displayButton->setIconSize(QSize(26, 26));
        displayButton->setAutoExclusive(false);
        hideButton = new QPushButton(login_widget);
        hideButton->setObjectName("hideButton");
        hideButton->setGeometry(QRect(350, 240, 41, 28));
        hideButton->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/jpeg/hide.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        hideButton->setIcon(icon4);
        hideButton->setIconSize(QSize(26, 26));
        hideButton->setAutoExclusive(false);
        label_4->raise();
        label_Password->raise();
        label_Username->raise();
        Username_LineEdit->raise();
        Password_LineEdit->raise();
        loginButton->raise();
        displayButton->raise();
        hideButton->raise();
        enroll_widget = new QWidget(dht11);
        enroll_widget->setObjectName("enroll_widget");
        enroll_widget->setGeometry(QRect(0, 29, 810, 521));
        label_password = new QLabel(enroll_widget);
        label_password->setObjectName("label_password");
        label_password->setGeometry(QRect(30, 180, 115, 47));
        label_password->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";"));
        enrollButton = new QPushButton(enroll_widget);
        enrollButton->setObjectName("enrollButton");
        enrollButton->setGeometry(QRect(190, 360, 153, 49));
        enrollButton->setStyleSheet(QString::fromUtf8("\n"
"     font: 17pt \"\345\215\216\346\226\207\346\245\267\344\275\223\";"));
        password_LineEdit = new QLineEdit(enroll_widget);
        password_LineEdit->setObjectName("password_LineEdit");
        password_LineEdit->setGeometry(QRect(140, 180, 261, 45));
        password_LineEdit->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223\";"));
        label_comfirm = new QLabel(enroll_widget);
        label_comfirm->setObjectName("label_comfirm");
        label_comfirm->setGeometry(QRect(30, 270, 111, 47));
        label_comfirm->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";"));
        label_username = new QLabel(enroll_widget);
        label_username->setObjectName("label_username");
        label_username->setGeometry(QRect(30, 90, 127, 47));
        label_username->setStyleSheet(QString::fromUtf8("font: 500 15pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";"));
        username_LineEdit = new QLineEdit(enroll_widget);
        username_LineEdit->setObjectName("username_LineEdit");
        username_LineEdit->setGeometry(QRect(140, 90, 261, 45));
        username_LineEdit->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223\";"));
        comfirm_LineEdit = new QLineEdit(enroll_widget);
        comfirm_LineEdit->setObjectName("comfirm_LineEdit");
        comfirm_LineEdit->setGeometry(QRect(140, 270, 261, 45));
        comfirm_LineEdit->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223\";"));
        label_2 = new QLabel(enroll_widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 0, 810, 521));
        label_2->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/login.jpg")));
        label_2->setScaledContents(true);
        displayButton2 = new QPushButton(enroll_widget);
        displayButton2->setObjectName("displayButton2");
        displayButton2->setGeometry(QRect(360, 190, 41, 28));
        displayButton2->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        displayButton2->setIcon(icon3);
        displayButton2->setIconSize(QSize(26, 26));
        displayButton2->setAutoExclusive(false);
        hideButton2 = new QPushButton(enroll_widget);
        hideButton2->setObjectName("hideButton2");
        hideButton2->setGeometry(QRect(360, 190, 41, 28));
        hideButton2->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        hideButton2->setIcon(icon4);
        hideButton2->setIconSize(QSize(26, 26));
        hideButton2->setAutoExclusive(false);
        displayButton1 = new QPushButton(enroll_widget);
        displayButton1->setObjectName("displayButton1");
        displayButton1->setGeometry(QRect(360, 280, 41, 28));
        displayButton1->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        displayButton1->setIcon(icon3);
        displayButton1->setIconSize(QSize(26, 26));
        displayButton1->setAutoExclusive(false);
        hideButton1 = new QPushButton(enroll_widget);
        hideButton1->setObjectName("hideButton1");
        hideButton1->setGeometry(QRect(360, 280, 41, 28));
        hideButton1->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}"));
        hideButton1->setIcon(icon4);
        hideButton1->setIconSize(QSize(26, 26));
        hideButton1->setAutoExclusive(false);
        label_2->raise();
        label_password->raise();
        enrollButton->raise();
        password_LineEdit->raise();
        label_comfirm->raise();
        label_username->raise();
        username_LineEdit->raise();
        comfirm_LineEdit->raise();
        displayButton2->raise();
        displayButton1->raise();
        hideButton1->raise();
        hideButton2->raise();
        menu_widget = new QWidget(dht11);
        menu_widget->setObjectName("menu_widget");
        menu_widget->setGeometry(QRect(0, 29, 810, 521));
        label = new QLabel(menu_widget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 810, 521));
        label->setContextMenuPolicy(Qt::ContextMenuPolicy::PreventContextMenu);
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/login.jpg")));
        label->setScaledContents(true);
        login_pushButton = new QPushButton(menu_widget);
        login_pushButton->setObjectName("login_pushButton");
        login_pushButton->setGeometry(QRect(90, 300, 201, 53));
        login_pushButton->setStyleSheet(QString::fromUtf8("\n"
"                    font: 17pt \"\345\215\216\346\226\207\346\245\267\344\275\223\";\n"
"                "));
        enroll_pushButton = new QPushButton(menu_widget);
        enroll_pushButton->setObjectName("enroll_pushButton");
        enroll_pushButton->setGeometry(QRect(90, 134, 201, 53));
        enroll_pushButton->setStyleSheet(QString::fromUtf8("\n"
"     font: 17pt \"\345\215\216\346\226\207\346\245\267\344\275\223\";"));
        enroll_pushButton->setAutoExclusive(false);
        background->raise();
        closeButton->raise();
        minButton->raise();
        time_label->raise();
        Button_last->raise();
        charts_widget->raise();
        menu_widget->raise();
        enroll_widget->raise();
        login_widget->raise();
        operator_widget->raise();

        retranslateUi(dht11);

        QMetaObject::connectSlotsByName(dht11);
    } // setupUi

    void retranslateUi(QWidget *dht11)
    {
        dht11->setWindowTitle(QCoreApplication::translate("dht11", "dht11", nullptr));
        time_label->setText(QString());
        closeButton->setText(QString());
#if QT_CONFIG(shortcut)
        closeButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        minButton->setText(QString());
#if QT_CONFIG(shortcut)
        minButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        background->setText(QString());
#if QT_CONFIG(tooltip)
        btn_reading->setToolTip(QCoreApplication::translate("dht11", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_reading->setText(QCoreApplication::translate("dht11", "\345\256\236\346\227\266\350\257\273\345\217\226", nullptr));
        label_background->setText(QString());
#if QT_CONFIG(tooltip)
        btn_history->setToolTip(QCoreApplication::translate("dht11", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_history->setText(QCoreApplication::translate("dht11", "\345\216\206\345\217\262\346\225\260\346\215\256", nullptr));
        label_dht11_1->setText(QString());
        labelTemperature->setText(QCoreApplication::translate("dht11", "00", nullptr));
        Temperature_position->setText(QCoreApplication::translate("dht11", "\342\204\203", nullptr));
        Humidity_position->setText(QCoreApplication::translate("dht11", "\357\274\205RH", nullptr));
        labelHumidity->setText(QCoreApplication::translate("dht11", "00", nullptr));
        Humidity->setText(QCoreApplication::translate("dht11", "Humidity\357\274\232", nullptr));
        Temperature->setText(QCoreApplication::translate("dht11", "Temperature\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        btn_stoping->setToolTip(QCoreApplication::translate("dht11", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_stoping->setText(QCoreApplication::translate("dht11", "\345\201\234\346\255\242\351\207\207\351\233\206", nullptr));
        comboPort->setItemText(0, QCoreApplication::translate("dht11", "COM1", nullptr));
        comboPort->setItemText(1, QCoreApplication::translate("dht11", "COM2", nullptr));
        comboPort->setItemText(2, QCoreApplication::translate("dht11", "COM3", nullptr));
        comboPort->setItemText(3, QCoreApplication::translate("dht11", "COM4", nullptr));
        comboPort->setItemText(4, QCoreApplication::translate("dht11", "COM5", nullptr));
        comboPort->setItemText(5, QCoreApplication::translate("dht11", "COM6", nullptr));
        comboPort->setItemText(6, QCoreApplication::translate("dht11", "COM7", nullptr));
        comboPort->setItemText(7, QCoreApplication::translate("dht11", "COM8", nullptr));

        Temperature_2->setText(QCoreApplication::translate("dht11", "Num\357\274\232", nullptr));
        label_dht11_2->setText(QString());
        Button_last->setText(QString());
#if QT_CONFIG(shortcut)
        Button_last->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        label_background_3->setText(QString());
        label_Password->setText(QCoreApplication::translate("dht11", "PassWord\357\274\232", nullptr));
        label_Username->setText(QCoreApplication::translate("dht11", "UserName\357\274\232", nullptr));
        label_4->setText(QString());
        loginButton->setText(QCoreApplication::translate("dht11", "\347\231\273\345\275\225", nullptr));
        displayButton->setText(QString());
#if QT_CONFIG(shortcut)
        displayButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        hideButton->setText(QString());
#if QT_CONFIG(shortcut)
        hideButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        label_password->setText(QCoreApplication::translate("dht11", "PassWord \357\274\232", nullptr));
        enrollButton->setText(QCoreApplication::translate("dht11", "\346\263\250\345\206\214", nullptr));
        label_comfirm->setText(QCoreApplication::translate("dht11", "Comfirm  \357\274\232", nullptr));
        label_username->setText(QCoreApplication::translate("dht11", "UserName\357\274\232", nullptr));
        label_2->setText(QString());
        displayButton2->setText(QString());
#if QT_CONFIG(shortcut)
        displayButton2->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        hideButton2->setText(QString());
#if QT_CONFIG(shortcut)
        hideButton2->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        displayButton1->setText(QString());
#if QT_CONFIG(shortcut)
        displayButton1->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        hideButton1->setText(QString());
#if QT_CONFIG(shortcut)
        hideButton1->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        label->setText(QString());
        login_pushButton->setText(QCoreApplication::translate("dht11", "\347\231\273\345\275\225", nullptr));
        enroll_pushButton->setText(QCoreApplication::translate("dht11", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dht11: public Ui_dht11 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DHT11_H
