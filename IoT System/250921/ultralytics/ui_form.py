# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'formGuiyuo.ui'
##
## Created by: Qt User Interface Compiler version 6.9.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QLabel, QLineEdit, QMainWindow,
    QPushButton, QSizePolicy, QWidget)
import rec_rc

class Ui_GUI(object):
    def setupUi(self, GUI):
        if not GUI.objectName():
            GUI.setObjectName(u"GUI")
        GUI.resize(810, 550)
        GUI.setStyleSheet(u"")
        self.centralwidget = QWidget(GUI)
        self.centralwidget.setObjectName(u"centralwidget")
        self.background = QLabel(self.centralwidget)
        self.background.setObjectName(u"background")
        self.background.setGeometry(QRect(0, 0, 810, 550))
        self.background.setPixmap(QPixmap(u":/new/prefix1/jpeg/parent.jpg"))
        self.background.setScaledContents(True)
        self.time_label = QLabel(self.centralwidget)
        self.time_label.setObjectName(u"time_label")
        self.time_label.setGeometry(QRect(45, 0, 321, 31))
        font = QFont()
        font.setFamilies([u"\u96b6\u4e66"])
        font.setPointSize(16)
        font.setBold(True)
        font.setItalic(False)
        self.time_label.setFont(font)
        self.time_label.setStyleSheet(u"font: 16pt \"\u96b6\u4e66\";\n"
"font-weight: bold;\n"
"color: rgb(0, 0, 0);")
        self.Button_last = QPushButton(self.centralwidget)
        self.Button_last.setObjectName(u"Button_last")
        self.Button_last.setGeometry(QRect(650, 0, 40, 28))
        self.Button_last.setStyleSheet(u"QPushButton {	\n"
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
"}")
        icon = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.EditUndo))
        self.Button_last.setIcon(icon)
        self.Button_last.setIconSize(QSize(22, 22))
        self.closeButton = QPushButton(self.centralwidget)
        self.closeButton.setObjectName(u"closeButton")
        self.closeButton.setGeometry(QRect(770, 0, 41, 28))
        self.closeButton.setStyleSheet(u"QPushButton {	\n"
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
"}")
        icon1 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.EditClear))
        self.closeButton.setIcon(icon1)
        self.closeButton.setIconSize(QSize(26, 26))
        self.closeButton.setAutoExclusive(True)
        self.minButton = QPushButton(self.centralwidget)
        self.minButton.setObjectName(u"minButton")
        self.minButton.setGeometry(QRect(690, 0, 41, 28))
        self.minButton.setStyleSheet(u"QPushButton {	\n"
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
"}")
        icon2 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.ListRemove))
        self.minButton.setIcon(icon2)
        self.minButton.setIconSize(QSize(22, 22))
        self.maxButton = QPushButton(self.centralwidget)
        self.maxButton.setObjectName(u"maxButton")
        self.maxButton.setGeometry(QRect(730, 0, 41, 28))
        self.maxButton.setStyleSheet(u"QPushButton {	\n"
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
"}")
        icon3 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.ViewFullscreen))
        self.maxButton.setIcon(icon3)
        self.maxButton.setIconSize(QSize(22, 22))
        self.menu_widget = QWidget(self.centralwidget)
        self.menu_widget.setObjectName(u"menu_widget")
        self.menu_widget.setGeometry(QRect(0, 29, 810, 521))
        self.label = QLabel(self.menu_widget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(0, 0, 810, 521))
        self.label.setContextMenuPolicy(Qt.ContextMenuPolicy.PreventContextMenu)
        self.label.setPixmap(QPixmap(u":/new/prefix1/jpeg/login.jpg"))
        self.label.setScaledContents(True)
        self.login_pushButton = QPushButton(self.menu_widget)
        self.login_pushButton.setObjectName(u"login_pushButton")
        self.login_pushButton.setGeometry(QRect(90, 300, 201, 53))
        self.login_pushButton.setStyleSheet(u"\n"
"                    font: 17pt \"\u534e\u6587\u6977\u4f53\";\n"
"                ")
        self.enroll_pushButton = QPushButton(self.menu_widget)
        self.enroll_pushButton.setObjectName(u"enroll_pushButton")
        self.enroll_pushButton.setGeometry(QRect(90, 134, 201, 53))
        self.enroll_pushButton.setStyleSheet(u"\n"
"     font: 17pt \"\u534e\u6587\u6977\u4f53\";")
        self.enroll_pushButton.setAutoExclusive(False)
        self.enroll_widget = QWidget(self.centralwidget)
        self.enroll_widget.setObjectName(u"enroll_widget")
        self.enroll_widget.setGeometry(QRect(0, 29, 810, 521))
        self.label_password = QLabel(self.enroll_widget)
        self.label_password.setObjectName(u"label_password")
        self.label_password.setGeometry(QRect(30, 180, 115, 47))
        self.label_password.setStyleSheet(u"font: 500 15pt \"\u7231\u5947\u827a\u9ed1\u4f53 Medium\";")
        self.enrollButton = QPushButton(self.enroll_widget)
        self.enrollButton.setObjectName(u"enrollButton")
        self.enrollButton.setGeometry(QRect(190, 360, 153, 49))
        self.enrollButton.setStyleSheet(u"\n"
"     font: 17pt \"\u534e\u6587\u6977\u4f53\";")
        self.password_LineEdit = QLineEdit(self.enroll_widget)
        self.password_LineEdit.setObjectName(u"password_LineEdit")
        self.password_LineEdit.setGeometry(QRect(140, 180, 261, 45))
        self.password_LineEdit.setStyleSheet(u"font: 16pt \"\u65b9\u6b63\u7c97\u9ed1\u5b8b\u7b80\u4f53\";")
        self.label_comfirm = QLabel(self.enroll_widget)
        self.label_comfirm.setObjectName(u"label_comfirm")
        self.label_comfirm.setGeometry(QRect(30, 270, 111, 47))
        self.label_comfirm.setStyleSheet(u"font: 500 15pt \"\u7231\u5947\u827a\u9ed1\u4f53 Medium\";")
        self.label_username = QLabel(self.enroll_widget)
        self.label_username.setObjectName(u"label_username")
        self.label_username.setGeometry(QRect(30, 90, 127, 47))
        self.label_username.setStyleSheet(u"font: 500 15pt \"\u7231\u5947\u827a\u9ed1\u4f53 Medium\";")
        self.username_LineEdit = QLineEdit(self.enroll_widget)
        self.username_LineEdit.setObjectName(u"username_LineEdit")
        self.username_LineEdit.setGeometry(QRect(140, 90, 261, 45))
        self.username_LineEdit.setStyleSheet(u"font: 16pt \"\u65b9\u6b63\u7c97\u9ed1\u5b8b\u7b80\u4f53\";")
        self.comfirm_LineEdit = QLineEdit(self.enroll_widget)
        self.comfirm_LineEdit.setObjectName(u"comfirm_LineEdit")
        self.comfirm_LineEdit.setGeometry(QRect(140, 270, 261, 45))
        self.comfirm_LineEdit.setStyleSheet(u"font: 16pt \"\u65b9\u6b63\u7c97\u9ed1\u5b8b\u7b80\u4f53\";")
        self.label_2 = QLabel(self.enroll_widget)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(0, 0, 810, 521))
        self.label_2.setContextMenuPolicy(Qt.ContextMenuPolicy.PreventContextMenu)
        self.label_2.setPixmap(QPixmap(u":/new/prefix1/jpeg/login.jpg"))
        self.label_2.setScaledContents(True)
        self.displayButton2 = QPushButton(self.enroll_widget)
        self.displayButton2.setObjectName(u"displayButton2")
        self.displayButton2.setGeometry(QRect(360, 190, 41, 28))
        self.displayButton2.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        icon4 = QIcon()
        icon4.addFile(u":/new/prefix1/jpeg/display.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.displayButton2.setIcon(icon4)
        self.displayButton2.setIconSize(QSize(26, 26))
        self.displayButton2.setAutoExclusive(False)
        self.hideButton2 = QPushButton(self.enroll_widget)
        self.hideButton2.setObjectName(u"hideButton2")
        self.hideButton2.setGeometry(QRect(360, 190, 41, 28))
        self.hideButton2.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        icon5 = QIcon()
        icon5.addFile(u":/new/prefix1/jpeg/hide.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.hideButton2.setIcon(icon5)
        self.hideButton2.setIconSize(QSize(26, 26))
        self.hideButton2.setAutoExclusive(False)
        self.displayButton1 = QPushButton(self.enroll_widget)
        self.displayButton1.setObjectName(u"displayButton1")
        self.displayButton1.setGeometry(QRect(360, 280, 41, 28))
        self.displayButton1.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        self.displayButton1.setIcon(icon4)
        self.displayButton1.setIconSize(QSize(26, 26))
        self.displayButton1.setAutoExclusive(False)
        self.hideButton1 = QPushButton(self.enroll_widget)
        self.hideButton1.setObjectName(u"hideButton1")
        self.hideButton1.setGeometry(QRect(360, 280, 41, 28))
        self.hideButton1.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        self.hideButton1.setIcon(icon5)
        self.hideButton1.setIconSize(QSize(26, 26))
        self.hideButton1.setAutoExclusive(False)
        self.label_2.raise_()
        self.label_password.raise_()
        self.enrollButton.raise_()
        self.password_LineEdit.raise_()
        self.label_comfirm.raise_()
        self.label_username.raise_()
        self.username_LineEdit.raise_()
        self.comfirm_LineEdit.raise_()
        self.displayButton2.raise_()
        self.hideButton2.raise_()
        self.displayButton1.raise_()
        self.hideButton1.raise_()
        self.login_widget = QWidget(self.centralwidget)
        self.login_widget.setObjectName(u"login_widget")
        self.login_widget.setGeometry(QRect(0, 29, 810, 521))
        self.label_Password = QLabel(self.login_widget)
        self.label_Password.setObjectName(u"label_Password")
        self.label_Password.setGeometry(QRect(30, 230, 105, 47))
        self.label_Password.setStyleSheet(u"font: 500 15pt \"\u7231\u5947\u827a\u9ed1\u4f53 Medium\";")
        self.label_Username = QLabel(self.login_widget)
        self.label_Username.setObjectName(u"label_Username")
        self.label_Username.setGeometry(QRect(30, 110, 103, 47))
        self.label_Username.setStyleSheet(u"font: 500 15pt \"\u7231\u5947\u827a\u9ed1\u4f53 Medium\";")
        self.Username_LineEdit = QLineEdit(self.login_widget)
        self.Username_LineEdit.setObjectName(u"Username_LineEdit")
        self.Username_LineEdit.setGeometry(QRect(130, 110, 261, 45))
        self.Username_LineEdit.setStyleSheet(u"font: 16pt \"\u65b9\u6b63\u7c97\u9ed1\u5b8b\u7b80\u4f53\";")
        self.Password_LineEdit = QLineEdit(self.login_widget)
        self.Password_LineEdit.setObjectName(u"Password_LineEdit")
        self.Password_LineEdit.setGeometry(QRect(130, 230, 261, 45))
        self.Password_LineEdit.setStyleSheet(u"font: 16pt \"\u65b9\u6b63\u7c97\u9ed1\u5b8b\u7b80\u4f53\";")
        self.label_4 = QLabel(self.login_widget)
        self.label_4.setObjectName(u"label_4")
        self.label_4.setGeometry(QRect(0, 0, 810, 521))
        self.label_4.setContextMenuPolicy(Qt.ContextMenuPolicy.PreventContextMenu)
        self.label_4.setPixmap(QPixmap(u":/new/prefix1/jpeg/login.jpg"))
        self.label_4.setScaledContents(True)
        self.loginButton = QPushButton(self.login_widget)
        self.loginButton.setObjectName(u"loginButton")
        self.loginButton.setGeometry(QRect(160, 340, 153, 49))
        self.loginButton.setStyleSheet(u"\n"
"                    font: 17pt \"\u534e\u6587\u6977\u4f53\";\n"
"                ")
        self.displayButton = QPushButton(self.login_widget)
        self.displayButton.setObjectName(u"displayButton")
        self.displayButton.setGeometry(QRect(350, 240, 41, 28))
        self.displayButton.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        self.displayButton.setIcon(icon4)
        self.displayButton.setIconSize(QSize(26, 26))
        self.displayButton.setAutoExclusive(False)
        self.hideButton = QPushButton(self.login_widget)
        self.hideButton.setObjectName(u"hideButton")
        self.hideButton.setGeometry(QRect(350, 240, 41, 28))
        self.hideButton.setStyleSheet(u"QPushButton {	\n"
"	border: none;\n"
"	border-radius: 8px;\n"
"	background-color: transparent;\n"
"     padding: 10px;\n"
"     border-radius: 5px;\n"
"}\n"
"QPushButton:pressed {	\n"
"	background-color: transparent;\n"
"}")
        self.hideButton.setIcon(icon5)
        self.hideButton.setIconSize(QSize(26, 26))
        self.hideButton.setAutoExclusive(False)
        self.label_4.raise_()
        self.label_Password.raise_()
        self.label_Username.raise_()
        self.Username_LineEdit.raise_()
        self.Password_LineEdit.raise_()
        self.loginButton.raise_()
        self.displayButton.raise_()
        self.hideButton.raise_()
        self.operator_widget = QWidget(self.centralwidget)
        self.operator_widget.setObjectName(u"operator_widget")
        self.operator_widget.setGeometry(QRect(0, 29, 810, 521))
        self.image_board = QLabel(self.operator_widget)
        self.image_board.setObjectName(u"image_board")
        self.image_board.setGeometry(QRect(80, 90, 281, 279))
        self.image_board.setStyleSheet(u"     background-color: rgba(244, 239, 231,120);")
        self.btn_open_image = QPushButton(self.operator_widget)
        self.btn_open_image.setObjectName(u"btn_open_image")
        self.btn_open_image.setGeometry(QRect(80, 400, 111, 41))
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.btn_open_image.sizePolicy().hasHeightForWidth())
        self.btn_open_image.setSizePolicy(sizePolicy)
        self.btn_open_image.setMinimumSize(QSize(0, 0))
        self.btn_open_image.setMaximumSize(QSize(16777215, 16777215))
        font1 = QFont()
        font1.setFamilies([u"\u534e\u6587\u65b0\u9b4f"])
        font1.setPointSize(16)
        font1.setBold(False)
        font1.setItalic(False)
        self.btn_open_image.setFont(font1)
        self.btn_open_image.setStyleSheet(u"QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\u534e\u6587\u65b0\u9b4f\";\n"
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
"}")
        self.btn_capture_frame = QPushButton(self.operator_widget)
        self.btn_capture_frame.setObjectName(u"btn_capture_frame")
        self.btn_capture_frame.setGeometry(QRect(250, 400, 111, 41))
        sizePolicy.setHeightForWidth(self.btn_capture_frame.sizePolicy().hasHeightForWidth())
        self.btn_capture_frame.setSizePolicy(sizePolicy)
        self.btn_capture_frame.setMinimumSize(QSize(0, 0))
        self.btn_capture_frame.setMaximumSize(QSize(16777215, 16777215))
        self.btn_capture_frame.setStyleSheet(u"QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\u534e\u6587\u65b0\u9b4f\";\n"
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
"}")
        self.output_board = QLabel(self.operator_widget)
        self.output_board.setObjectName(u"output_board")
        self.output_board.setGeometry(QRect(450, 90, 281, 279))
        self.output_board.setStyleSheet(u"     background-color: rgba(244, 239, 231,120);")
        self.label_5 = QLabel(self.operator_widget)
        self.label_5.setObjectName(u"label_5")
        self.label_5.setGeometry(QRect(0, 0, 810, 521))
        self.label_5.setContextMenuPolicy(Qt.ContextMenuPolicy.PreventContextMenu)
        self.label_5.setPixmap(QPixmap(u":/new/prefix1/jpeg/background.jpg"))
        self.label_5.setScaledContents(True)
        self.btn_result = QPushButton(self.operator_widget)
        self.btn_result.setObjectName(u"btn_result")
        self.btn_result.setGeometry(QRect(620, 400, 111, 41))
        sizePolicy.setHeightForWidth(self.btn_result.sizePolicy().hasHeightForWidth())
        self.btn_result.setSizePolicy(sizePolicy)
        self.btn_result.setMinimumSize(QSize(0, 0))
        self.btn_result.setMaximumSize(QSize(16777215, 16777215))
        self.btn_result.setStyleSheet(u"QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\u534e\u6587\u65b0\u9b4f\";\n"
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
"}")
        self.btn_dial_proofread = QPushButton(self.operator_widget)
        self.btn_dial_proofread.setObjectName(u"btn_dial_proofread")
        self.btn_dial_proofread.setGeometry(QRect(450, 400, 111, 41))
        sizePolicy.setHeightForWidth(self.btn_dial_proofread.sizePolicy().hasHeightForWidth())
        self.btn_dial_proofread.setSizePolicy(sizePolicy)
        self.btn_dial_proofread.setMinimumSize(QSize(0, 0))
        self.btn_dial_proofread.setMaximumSize(QSize(16777215, 16777215))
        self.btn_dial_proofread.setFont(font1)
        self.btn_dial_proofread.setStyleSheet(u"QPushButton {	\n"
"	color:black;\n"
"	font:16pt\"\u534e\u6587\u65b0\u9b4f\";\n"
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
"}")
        self.label_5.raise_()
        self.image_board.raise_()
        self.btn_open_image.raise_()
        self.btn_capture_frame.raise_()
        self.output_board.raise_()
        self.btn_result.raise_()
        self.btn_dial_proofread.raise_()
        GUI.setCentralWidget(self.centralwidget)
        self.background.raise_()
        self.time_label.raise_()
        self.Button_last.raise_()
        self.closeButton.raise_()
        self.minButton.raise_()
        self.maxButton.raise_()
        self.menu_widget.raise_()
        self.login_widget.raise_()
        self.enroll_widget.raise_()
        self.operator_widget.raise_()

        self.retranslateUi(GUI)

        QMetaObject.connectSlotsByName(GUI)
    # setupUi

    def retranslateUi(self, GUI):
        GUI.setWindowTitle(QCoreApplication.translate("GUI", u"GUI", None))
        self.background.setText("")
        self.time_label.setText("")
        self.Button_last.setText("")
#if QT_CONFIG(shortcut)
        self.Button_last.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.closeButton.setText("")
#if QT_CONFIG(shortcut)
        self.closeButton.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.minButton.setText("")
#if QT_CONFIG(shortcut)
        self.minButton.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.maxButton.setText("")
#if QT_CONFIG(shortcut)
        self.maxButton.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.label.setText("")
        self.login_pushButton.setText(QCoreApplication.translate("GUI", u"\u767b\u5f55", None))
        self.enroll_pushButton.setText(QCoreApplication.translate("GUI", u"\u6ce8\u518c", None))
        self.label_password.setText(QCoreApplication.translate("GUI", u"PassWord \uff1a", None))
        self.enrollButton.setText(QCoreApplication.translate("GUI", u"\u6ce8\u518c", None))
        self.label_comfirm.setText(QCoreApplication.translate("GUI", u"Comfirm  \uff1a", None))
        self.label_username.setText(QCoreApplication.translate("GUI", u"UserName\uff1a", None))
        self.label_2.setText("")
        self.displayButton2.setText("")
#if QT_CONFIG(shortcut)
        self.displayButton2.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.hideButton2.setText("")
#if QT_CONFIG(shortcut)
        self.hideButton2.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.displayButton1.setText("")
#if QT_CONFIG(shortcut)
        self.displayButton1.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.hideButton1.setText("")
#if QT_CONFIG(shortcut)
        self.hideButton1.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.label_Password.setText(QCoreApplication.translate("GUI", u"PassWord\uff1a", None))
        self.label_Username.setText(QCoreApplication.translate("GUI", u"UserName\uff1a", None))
        self.label_4.setText("")
        self.loginButton.setText(QCoreApplication.translate("GUI", u"\u767b\u5f55", None))
        self.displayButton.setText("")
#if QT_CONFIG(shortcut)
        self.displayButton.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.hideButton.setText("")
#if QT_CONFIG(shortcut)
        self.hideButton.setShortcut(QCoreApplication.translate("GUI", u"Esc", None))
#endif // QT_CONFIG(shortcut)
        self.image_board.setText("")
#if QT_CONFIG(tooltip)
        self.btn_open_image.setToolTip(QCoreApplication.translate("GUI", u"Close", None))
#endif // QT_CONFIG(tooltip)
        self.btn_open_image.setText(QCoreApplication.translate("GUI", u"\u8bfb\u53d6\u56fe\u7247", None))
#if QT_CONFIG(tooltip)
        self.btn_capture_frame.setToolTip(QCoreApplication.translate("GUI", u"Close", None))
#endif // QT_CONFIG(tooltip)
        self.btn_capture_frame.setText(QCoreApplication.translate("GUI", u"\u5b9e\u65f6\u62cd\u6444", None))
        self.output_board.setText("")
        self.label_5.setText("")
#if QT_CONFIG(tooltip)
        self.btn_result.setToolTip(QCoreApplication.translate("GUI", u"Close", None))
#endif // QT_CONFIG(tooltip)
        self.btn_result.setText(QCoreApplication.translate("GUI", u"\u8bc6\u522b\u7ed3\u679c", None))
#if QT_CONFIG(tooltip)
        self.btn_dial_proofread.setToolTip(QCoreApplication.translate("GUI", u"Close", None))
#endif // QT_CONFIG(tooltip)
        self.btn_dial_proofread.setText(QCoreApplication.translate("GUI", u"\u8868\u76d8\u6821\u6b63", None))
    # retranslateUi

