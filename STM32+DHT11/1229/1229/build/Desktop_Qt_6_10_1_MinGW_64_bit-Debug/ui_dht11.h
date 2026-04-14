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
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dht11
{
public:
    QWidget *operator_widget;
    QLabel *background;
    QPushButton *btn_reading;
    QChartView *line_chart;
    QLabel *Humidity_position;
    QLabel *Humidity;
    QListView *time_list;
    QLabel *labelHumidity;
    QLabel *Temperature_position;
    QLabel *labelTemperature;
    QLabel *Temperature_2;
    QComboBox *comboPort;
    QLabel *Temperature;
    QPushButton *btn_stoping;
    QLabel *Temperature_3;
    QComboBox *baudRate;
    QLabel *time_label;
    QPushButton *closeButton;
    QLabel *label;
    QLabel *parent;
    QPushButton *minButton;

    void setupUi(QWidget *dht11)
    {
        if (dht11->objectName().isEmpty())
            dht11->setObjectName("dht11");
        dht11->resize(650, 800);
        operator_widget = new QWidget(dht11);
        operator_widget->setObjectName("operator_widget");
        operator_widget->setGeometry(QRect(0, 35, 650, 765));
        operator_widget->setStyleSheet(QString::fromUtf8("\n"
"background-color: transparent;\n"
""));
        background = new QLabel(operator_widget);
        background->setObjectName("background");
        background->setGeometry(QRect(0, 0, 650, 765));
        background->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/background2.jpg")));
        background->setScaledContents(true);
        btn_reading = new QPushButton(operator_widget);
        btn_reading->setObjectName("btn_reading");
        btn_reading->setGeometry(QRect(80, 260, 141, 43));
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
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgba(244, 239, 231,180);\n"
"	border: 1px solid #726f6b;\n"
"}\n"
"QPushButton:pressed {	\n"
"	border: 1px solid #A7A8B6;\n"
"}"));
        line_chart = new QChartView(operator_widget);
        line_chart->setObjectName("line_chart");
        line_chart->setGeometry(QRect(20, 338, 601, 400));
        line_chart->setStyleSheet(QString::fromUtf8("background-color: transparent;"));
        Humidity_position = new QLabel(operator_widget);
        Humidity_position->setObjectName("Humidity_position");
        Humidity_position->setGeometry(QRect(540, 210, 91, 61));
        QFont font;
        font.setFamilies({QString::fromUtf8("\351\232\266\344\271\246")});
        font.setPointSize(23);
        font.setBold(false);
        font.setItalic(false);
        Humidity_position->setFont(font);
        Humidity_position->setStyleSheet(QString::fromUtf8("font: 23pt \"\351\232\266\344\271\246\";\n"
"color: rgb(85, 85, 127);"));
        Humidity = new QLabel(operator_widget);
        Humidity->setObjectName("Humidity");
        Humidity->setGeometry(QRect(330, 200, 101, 71));
        Humidity->setStyleSheet(QString::fromUtf8("font: 500 16pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";\n"
"color: rgb(69, 69, 103);"));
        time_list = new QListView(operator_widget);
        time_list->setObjectName("time_list");
        time_list->setGeometry(QRect(300, 40, 321, 281));
        time_list->setStyleSheet(QString::fromUtf8("#time_list {\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-radius: 18px;\n"
"    padding: 5px;\n"
"}\n"
""));
        labelHumidity = new QLabel(operator_widget);
        labelHumidity->setObjectName("labelHumidity");
        labelHumidity->setGeometry(QRect(470, 200, 71, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\345\247\232\344\275\223")});
        font1.setPointSize(45);
        font1.setBold(false);
        font1.setItalic(false);
        labelHumidity->setFont(font1);
        labelHumidity->setStyleSheet(QString::fromUtf8("color:rgba(31, 31, 46, 226);\n"
"font: 45pt \"\346\226\271\346\255\243\345\247\232\344\275\223\";"));
        Temperature_position = new QLabel(operator_widget);
        Temperature_position->setObjectName("Temperature_position");
        Temperature_position->setGeometry(QRect(540, 90, 61, 61));
        Temperature_position->setFont(font);
        Temperature_position->setStyleSheet(QString::fromUtf8("font: 23pt \"\351\232\266\344\271\246\";\n"
"color: rgb(85, 85, 127);"));
        labelTemperature = new QLabel(operator_widget);
        labelTemperature->setObjectName("labelTemperature");
        labelTemperature->setGeometry(QRect(470, 80, 71, 71));
        labelTemperature->setFont(font1);
        labelTemperature->setStyleSheet(QString::fromUtf8("color:rgba(31, 31, 46, 226);\n"
"font: 45pt \"\346\226\271\346\255\243\345\247\232\344\275\223\";"));
        Temperature_2 = new QLabel(operator_widget);
        Temperature_2->setObjectName("Temperature_2");
        Temperature_2->setGeometry(QRect(30, 70, 121, 71));
        Temperature_2->setStyleSheet(QString::fromUtf8("font: 700 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(85, 85, 127);"));
        comboPort = new QComboBox(operator_widget);
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->addItem(QString());
        comboPort->setObjectName("comboPort");
        comboPort->setGeometry(QRect(170, 89, 101, 41));
        comboPort->setStyleSheet(QString::fromUtf8("#comboPort{\n"
"	color: rgb(38, 38, 38);\n"
"	font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
""));
        Temperature = new QLabel(operator_widget);
        Temperature->setObjectName("Temperature");
        Temperature->setGeometry(QRect(330, 80, 131, 71));
        Temperature->setStyleSheet(QString::fromUtf8("font: 500 16pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223 Medium\";\n"
"color: rgb(69, 69, 103);"));
        btn_stoping = new QPushButton(operator_widget);
        btn_stoping->setObjectName("btn_stoping");
        btn_stoping->setGeometry(QRect(80, 260, 141, 43));
        sizePolicy.setHeightForWidth(btn_stoping->sizePolicy().hasHeightForWidth());
        btn_stoping->setSizePolicy(sizePolicy);
        btn_stoping->setMinimumSize(QSize(0, 0));
        btn_stoping->setMaximumSize(QSize(16777215, 16777215));
        btn_stoping->setStyleSheet(QString::fromUtf8("QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\345\215\216\346\226\207\346\226\260\351\255\217\";\n"
"	border: 1px solid #A7A8B6;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"     background-color: rgba(244, 239, 231,180);\n"
"	border: 1px solid #726f6b;\n"
"}\n"
"QPushButton:pressed {	\n"
"	border: 1px solid #A7A8B6;\n"
"}"));
        Temperature_3 = new QLabel(operator_widget);
        Temperature_3->setObjectName("Temperature_3");
        Temperature_3->setGeometry(QRect(30, 160, 121, 71));
        Temperature_3->setStyleSheet(QString::fromUtf8("font: 700 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(85, 85, 127);"));
        baudRate = new QComboBox(operator_widget);
        baudRate->addItem(QString());
        baudRate->addItem(QString());
        baudRate->addItem(QString());
        baudRate->addItem(QString());
        baudRate->setObjectName("baudRate");
        baudRate->setGeometry(QRect(170, 178, 101, 41));
        baudRate->setStyleSheet(QString::fromUtf8("#baudRate{\n"
"	color: rgb(38, 38, 38);\n"
"	font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"    background-color: rgba(244, 239, 231,120);\n"
"    border: 1px solid white; \n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"}\n"
""));
        background->raise();
        time_list->raise();
        btn_reading->raise();
        line_chart->raise();
        Humidity_position->raise();
        Humidity->raise();
        labelHumidity->raise();
        Temperature_position->raise();
        labelTemperature->raise();
        Temperature_2->raise();
        comboPort->raise();
        Temperature->raise();
        btn_stoping->raise();
        Temperature_3->raise();
        baudRate->raise();
        time_label = new QLabel(dht11);
        time_label->setObjectName("time_label");
        time_label->setGeometry(QRect(70, -8, 291, 51));
        time_label->setStyleSheet(QString::fromUtf8("color: rgb(77, 77, 77);\n"
"font: 15pt \"\346\245\267\344\275\223\";"));
        closeButton = new QPushButton(dht11);
        closeButton->setObjectName("closeButton");
        closeButton->setGeometry(QRect(610, 2, 41, 31));
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
        label = new QLabel(dht11);
        label->setObjectName("label");
        label->setGeometry(QRect(560, 761, 83, 38));
        label->setStyleSheet(QString::fromUtf8("font: 900 14pt \"\347\210\261\345\245\207\350\211\272\351\273\221\344\275\223\";\n"
"color: rgba(103, 110, 152, 180);\n"
""));
        parent = new QLabel(dht11);
        parent->setObjectName("parent");
        parent->setGeometry(QRect(0, 0, 650, 800));
        parent->setMaximumSize(QSize(1920, 1080));
        parent->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/jpeg/parent.jpg")));
        parent->setScaledContents(true);
        minButton = new QPushButton(dht11);
        minButton->setObjectName("minButton");
        minButton->setGeometry(QRect(570, 2, 41, 31));
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
        minButton->setIconSize(QSize(26, 26));
        parent->raise();
        operator_widget->raise();
        time_label->raise();
        label->raise();
        minButton->raise();
        closeButton->raise();

        retranslateUi(dht11);

        QMetaObject::connectSlotsByName(dht11);
    } // setupUi

    void retranslateUi(QWidget *dht11)
    {
        dht11->setWindowTitle(QCoreApplication::translate("dht11", "dht11", nullptr));
        background->setText(QString());
#if QT_CONFIG(tooltip)
        btn_reading->setToolTip(QCoreApplication::translate("dht11", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_reading->setText(QCoreApplication::translate("dht11", "\345\256\236\346\227\266\351\207\207\351\233\206", nullptr));
        Humidity_position->setText(QCoreApplication::translate("dht11", "\357\274\205RH", nullptr));
        Humidity->setText(QCoreApplication::translate("dht11", "Humidity\357\274\232", nullptr));
        labelHumidity->setText(QCoreApplication::translate("dht11", "00", nullptr));
        Temperature_position->setText(QCoreApplication::translate("dht11", "\342\204\203", nullptr));
        labelTemperature->setText(QCoreApplication::translate("dht11", "00", nullptr));
        Temperature_2->setText(QCoreApplication::translate("dht11", "Serial port\357\274\232", nullptr));
        comboPort->setItemText(0, QCoreApplication::translate("dht11", "COM1", nullptr));
        comboPort->setItemText(1, QCoreApplication::translate("dht11", "COM2", nullptr));
        comboPort->setItemText(2, QCoreApplication::translate("dht11", "COM3", nullptr));
        comboPort->setItemText(3, QCoreApplication::translate("dht11", "COM4", nullptr));
        comboPort->setItemText(4, QCoreApplication::translate("dht11", "COM5", nullptr));
        comboPort->setItemText(5, QCoreApplication::translate("dht11", "COM6", nullptr));

        Temperature->setText(QCoreApplication::translate("dht11", "Temperature\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        btn_stoping->setToolTip(QCoreApplication::translate("dht11", "Close", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_stoping->setText(QCoreApplication::translate("dht11", "\345\201\234\346\255\242\351\207\207\351\233\206", nullptr));
        Temperature_3->setText(QCoreApplication::translate("dht11", "Baud rate\357\274\232", nullptr));
        baudRate->setItemText(0, QCoreApplication::translate("dht11", "4800", nullptr));
        baudRate->setItemText(1, QCoreApplication::translate("dht11", "9600", nullptr));
        baudRate->setItemText(2, QCoreApplication::translate("dht11", "38400", nullptr));
        baudRate->setItemText(3, QCoreApplication::translate("dht11", "115200", nullptr));

        time_label->setText(QString());
        closeButton->setText(QString());
#if QT_CONFIG(shortcut)
        closeButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
        label->setText(QCoreApplication::translate("dht11", "V 2.6.1.0", nullptr));
        parent->setText(QString());
        minButton->setText(QString());
#if QT_CONFIG(shortcut)
        minButton->setShortcut(QCoreApplication::translate("dht11", "Esc", nullptr));
#endif // QT_CONFIG(shortcut)
    } // retranslateUi

};

namespace Ui {
    class dht11: public Ui_dht11 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DHT11_H
