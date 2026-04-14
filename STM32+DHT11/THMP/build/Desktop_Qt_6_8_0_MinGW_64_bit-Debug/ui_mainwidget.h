/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QLabel *labelTemperature;
    QLabel *labelHumidity;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName("MainWidget");
        MainWidget->resize(660, 477);
        labelTemperature = new QLabel(MainWidget);
        labelTemperature->setObjectName("labelTemperature");
        labelTemperature->setGeometry(QRect(250, 70, 331, 71));
        QFont font;
        font.setPointSize(36);
        labelTemperature->setFont(font);
        labelHumidity = new QLabel(MainWidget);
        labelHumidity->setObjectName("labelHumidity");
        labelHumidity->setGeometry(QRect(250, 170, 341, 81));
        labelHumidity->setFont(font);

        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "Temperature and Humidity Monitoring Program", nullptr));
        labelTemperature->setText(QCoreApplication::translate("MainWidget", "0 \302\260C", nullptr));
        labelHumidity->setText(QCoreApplication::translate("MainWidget", "0%", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
