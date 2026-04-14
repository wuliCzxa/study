# -*- coding: utf-8 -*-
import sqlite3
import hashlib
import os
import cv2
from PySide6.QtWidgets import QApplication, QMainWindow, QLineEdit, QMessageBox, QFileDialog
from PySide6.QtGui import QImage, QPixmap
from PySide6.QtCore import Qt, QTimer, QDateTime

from ui_form import Ui_GUI

import model1
import model2
import final
import dial_calibration
from dial_data import DialDatabase   # ⭐ 使用统一数据库模块


def hash_password(password: str) -> str:
    """返回密码的 SHA-256 哈希值"""
    return hashlib.sha256(password.encode("utf-8")).hexdigest()


class GUI(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.ui = Ui_GUI()
        self.ui.setupUi(self)

        # 鼠标拖拽
        self._is_dragging = False
        self._drag_position = None

        # 隐藏标题栏
        self.setWindowFlags(Qt.FramelessWindowHint)
        self.show_menu_page()

        # ⭐ 新增：初始化时间显示定时器
        self.time_timer = QTimer()
        self.time_timer.timeout.connect(self.update_time)
        self.time_timer.start(1000)
        self.update_time()

        # 摄像头
        self.cap = None
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_frame)
        self.is_camera_open = False
        self.current_frame = None

        # 按钮连接
        self.ui.closeButton.clicked.connect(self.close)
        self.ui.minButton.clicked.connect(self.showMinimized)
        self.ui.maxButton.clicked.connect(self.toggle_max_restore)
        self.ui.Button_last.clicked.connect(self.show_menu_page)

        self.ui.btn_open_image.clicked.connect(self.open_image)
        self.ui.btn_capture_frame.clicked.connect(self.capture_frame)

        self.ui.btn_dial_proofread.clicked.connect(self.run_dial_proofread)
        self.ui.btn_result.clicked.connect(self.run_final)

        # 初始化密码输入框
        self.ui.password_LineEdit.setEchoMode(QLineEdit.Password)
        self.ui.comfirm_LineEdit.setEchoMode(QLineEdit.Password)
        self.ui.Password_LineEdit.setEchoMode(QLineEdit.Password)

        # 密码按钮映射
        self.password_map = {
            "hideButton": (self.ui.Password_LineEdit, self.ui.hideButton, self.ui.displayButton),
            "displayButton": (self.ui.Password_LineEdit, self.ui.hideButton, self.ui.displayButton),
            "hideButton1": (self.ui.comfirm_LineEdit, self.ui.hideButton1, self.ui.displayButton1),
            "displayButton1": (self.ui.comfirm_LineEdit, self.ui.hideButton1, self.ui.displayButton1),
            "hideButton2": (self.ui.password_LineEdit, self.ui.hideButton2, self.ui.displayButton2),
            "displayButton2": (self.ui.password_LineEdit, self.ui.hideButton2, self.ui.displayButton2),
        }
        for btn_name in self.password_map:
            getattr(self.ui, btn_name).clicked.connect(self.toggle_password)

        # 页面切换按钮
        self.ui.enroll_pushButton.clicked.connect(self.show_enroll_page)
        self.ui.login_pushButton.clicked.connect(self.show_login_page)
        self.ui.loginButton.clicked.connect(self.login_user)
        self.ui.enrollButton.clicked.connect(self.register_user)

        self.is_maximized = False

        # ⭐ 初始化数据库
        self.db = DialDatabase()
        self.current_user_id = None
        self.current_task_id = None

    # ------------------ 窗口拖拽 ------------------
    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self._is_dragging = True
            self._drag_position = event.globalPosition().toPoint() - self.frameGeometry().topLeft()
            event.accept()

    def mouseMoveEvent(self, event):
        if self._is_dragging and event.buttons() & Qt.LeftButton:
            self.move(event.globalPosition().toPoint() - self._drag_position)
            event.accept()

    def mouseReleaseEvent(self, event):
        if event.button() == Qt.LeftButton:
            self._is_dragging = False
            event.accept()

    # ------------------ 时间更新 ------------------
    def update_time(self):
        current_time = QDateTime.currentDateTime()
        time_str = current_time.toString("yyyy-MM-dd HH:mm:ss")
        self.ui.time_label.setText(time_str)

    def toggle_max_restore(self):
        if self.is_maximized:
            self.showNormal()
            self.is_maximized = False
        else:
            self.showMaximized()
            self.is_maximized = True

    def toggle_password(self):
        sender = self.sender()
        btn_name = sender.objectName()
        line_edit, hide_btn, display_btn = self.password_map[btn_name]

        if "hide" in btn_name and line_edit.echoMode() == QLineEdit.Password:
            line_edit.setEchoMode(QLineEdit.Normal)
            hide_btn.hide()
            display_btn.show()
        elif "display" in btn_name and line_edit.echoMode() == QLineEdit.Normal:
            line_edit.setEchoMode(QLineEdit.Password)
            display_btn.hide()
            hide_btn.show()

    # ------------------ 页面切换 ------------------
    def hide_all_pages(self):
        self.ui.menu_widget.hide()
        self.ui.enroll_widget.hide()
        self.ui.login_widget.hide()
        self.ui.operator_widget.hide()

    def show_menu_page(self):
        self.hide_all_pages()
        self.ui.menu_widget.show()

    def show_enroll_page(self):
        self.hide_all_pages()
        self.ui.enroll_widget.show()

    def show_login_page(self):
        self.hide_all_pages()
        self.ui.login_widget.show()

    def show_operator_page(self):
        self.hide_all_pages()
        self.ui.operator_widget.show()

    # ------------------ 消息弹窗 ------------------
    def show_message(self, text, title="提示"):
        msg = QMessageBox(self)
        msg.setWindowTitle(title)
        msg.setText(text)
        msg.setIcon(QMessageBox.Information)
        msg.setStandardButtons(QMessageBox.Ok)
        msg.exec()

    # ------------------ 注册 / 登录 ------------------
    def register_user(self):
        username = self.ui.username_LineEdit.text().strip()
        password = self.ui.password_LineEdit.text()
        confirm = self.ui.comfirm_LineEdit.text()

        if password != confirm:
            self.show_message("两次输入的密码不一致")
            return
        if not username or not password:
            self.show_message("用户名和密码不能为空")
            return

        hashed_password = hash_password(password)
        try:
            user_id = self.db.insert_user(username, hashed_password)
            self.show_message("注册成功！")
            self.current_user_id = user_id
            self.show_operator_page()
        except sqlite3.IntegrityError:
            self.show_message("该用户名已存在")

    def login_user(self):
        username = self.ui.Username_LineEdit.text().strip()
        password = self.ui.Password_LineEdit.text()
        if not username or not password:
            self.show_message("用户名和密码不能为空")
            return

        hashed_password = hash_password(password)
        row = self.db.get_user(username)
        if row is None:
            self.show_message("该账号不存在")
        else:
            user_id, saved_password = row
            if saved_password != hashed_password:
                self.show_message("密码错误")
            else:
                self.show_message("登录成功！")
                self.current_user_id = user_id
                self.show_operator_page()

    # ------------------ 图像显示 ------------------
    def display_image(self, img_path):
        pixmap = QPixmap(img_path)
        self.ui.image_board.setPixmap(
            pixmap.scaled(
                self.ui.image_board.width(),
                self.ui.image_board.height(),
                Qt.KeepAspectRatio,
                Qt.SmoothTransformation
            )
        )

    # # ------------------ 打开图片 ------------------
    # def open_image(self):
    #     file_path, _ = QFileDialog.getOpenFileName(
    #         self, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)"
    #     )
    #     if file_path:
    #         self.display_image(file_path)
    #         detected = model1.process_image(file_path)
    #         if not detected:
    #             QMessageBox.warning(self, "警告", "输入图片非法")
    #         else:
    #             if self.current_user_id:
    #                 self.current_task_id = self.db.insert_task(self.current_user_id)
    #                 self.db.insert_image(self.current_task_id, "原图", file_path)
    #
    # # ------------------ 摄像头 ------------------
    # def capture_frame(self):
    #     if not self.is_camera_open:
    #         self.cap = cv2.VideoCapture(0)
    #         if not self.cap.isOpened():
    #             print("无法打开摄像头")
    #             return
    #         self.timer.start(30)
    #         self.is_camera_open = True
    #         self.ui.btn_capture_frame.setText("拍摄")
    #     else:
    #         if self.current_frame is not None:
    #             self.timer.stop()
    #             self.cap.release()
    #             self.is_camera_open = False
    #             self.ui.btn_capture_frame.setText("打开摄像头")
    #
    #             temp_path = "database/captured_frame.jpg"
    #             cv2.imwrite(temp_path, self.current_frame)
    #             self.display_image(temp_path)
    #
    #             detected = model1.process_image(temp_path)
    #             if not detected:
    #                 QMessageBox.warning(self, "警告", "输入图片非法")
    #             else:
    #                 if self.current_user_id:
    #                     self.current_task_id = self.db.insert_task(self.current_user_id)
    #                     self.db.insert_image(self.current_task_id, "原图", temp_path)
    # ------------------ 打开图片 ------------------
    def open_image(self):
        file_path, _ = QFileDialog.getOpenFileName(
            self, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)"
        )
        if file_path:
            self.display_image(file_path)
            if self.current_user_id:
                try:
                    # 调用 model1，自动创建 task 并写入 rectangle 表和 image_record
                    model1.model1(
                        user_id=self.current_user_id,
                        weights_path="./runs/detect/model1/weights/best.pt",
                        img_path=file_path,
                        save_dir="database"
                    )
                    # QMessageBox.information(self, "提示", "YOLO检测完成，数据已写入数据库")
                    self.current_task_id = self.db.insert_task(self.current_user_id)
                    self.db.insert_image(self.current_task_id, "原图", file_path)
                except Exception as e:
                    print("model1 运行出错:", e)
                    self.show_message(f"model1 运行出错: {e}", title="错误")
            else:
                QMessageBox.warning(self, "警告", "输入图片非法")

    # ------------------ 摄像头拍摄 ------------------
    def capture_frame(self):
        if not self.is_camera_open:
            self.cap = cv2.VideoCapture(0)
            if not self.cap.isOpened():
                print("无法打开摄像头")
                return
            self.timer.start(30)
            self.is_camera_open = True
            self.ui.btn_capture_frame.setText("拍摄")
        else:
            if self.current_frame is not None:
                self.timer.stop()
                self.cap.release()
                self.is_camera_open = False
                self.ui.btn_capture_frame.setText("打开摄像头")

                temp_path = "database/captured_frame.jpg"
                cv2.imwrite(temp_path, self.current_frame)
                self.display_image(temp_path)

                if self.current_user_id:
                    try:
                        # 调用 model1 处理拍摄图片
                        model1.model1(
                            user_id=self.current_user_id,
                            weights_path="./runs/detect/model1/weights/best.pt",
                            img_path=temp_path,
                            save_dir="database"
                        )
                        # QMessageBox.information(self, "提示", "YOLO检测完成，数据已写入数据库")
                        self.current_task_id = self.db.insert_task(self.current_user_id)
                        self.db.insert_image(self.current_task_id, "原图", temp_path)
                    except Exception as e:
                        print("model1 运行出错:", e)
                        self.show_message(f"model1 运行出错: {e}", title="错误")
                else:
                    QMessageBox.warning(self, "警告", "输入图片非法")

    def update_frame(self):
        ret, frame = self.cap.read()
        if ret:
            self.current_frame = frame.copy()
            rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            h, w, ch = rgb_image.shape
            bytes_per_line = ch * w
            qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
            pixmap = QPixmap.fromImage(qt_image)
            self.ui.image_board.setPixmap(
                pixmap.scaled(
                    self.ui.image_board.width(),
                    self.ui.image_board.height(),
                    Qt.KeepAspectRatio,
                    Qt.SmoothTransformation
                )
            )

    # ------------------ 校正 ------------------
    def run_dial_proofread(self):
        try:
            if hasattr(dial_calibration, "dial_calibration"):
                dial_calibration.dial_calibration()
            corrected_img_path = "database/dial_perspective_corrected.png"
            if os.path.exists(corrected_img_path):
                pixmap = QPixmap(corrected_img_path)
                self.ui.output_board.setPixmap(
                    pixmap.scaled(
                        self.ui.output_board.width(),
                        self.ui.output_board.height(),
                        Qt.KeepAspectRatio,
                        Qt.SmoothTransformation
                    )
                )
                if self.current_task_id:
                    self.db.insert_image(self.current_task_id, "校正增强", corrected_img_path)
            else:
                self.show_message("未找到 dial_perspective_corrected.png", title="错误")
        except Exception as e:
            print("运行校正出错:", e)
            self.show_message(f"运行校正出错: {e}", title="错误")

    # # ------------------ 最终结果 ------------------
    # def run_final(self):
    #     if hasattr(model2, "model2"):
    #         model2.model2()
    #     else:
    #         print("⚠️ model2.py 没有找到入口函数")
    #
    #     try:
    #         log_path = final.final()
    #         if not os.path.exists(log_path):
    #             self.show_message("未找到日志文件", title="错误")
    #             return
    #
    #         final_line = None
    #         with open(log_path, "r", encoding="utf-8") as f:
    #             for line in f:
    #                 if "仪表读数" in line:
    #                     final_line = line.strip()
    #
    #         if final_line is None:
    #             final_line = "日志中未找到仪表读数"
    #
    #         print("Final 结果:", final_line)
    #         self.show_message(final_line, title="最终结果")
    #
    #         # ⭐ 存储到任务表
    #         if self.current_task_id and "仪表读数" in final_line:
    #             try:
    #                 value = float(final_line.split(":")[-1])
    #                 self.db.update_task_result(self.current_task_id, value)
    #             except:
    #                 pass
    #
    #     except Exception as e:
    #         print("运行 final 出错:", e)
    #         self.show_message(f"运行 final 出错: {e}", title="错误")
    # ------------------ 最终结果 / OBB 检测 ------------------
    def run_final(self):
        if not self.current_user_id:
            QMessageBox.warning(self, "警告", "请先登录")
            return

        try:
            # 调用 model2，自动创建 rotate 表和 image_record
            model2.model2(user_id=self.current_user_id)
            # QMessageBox.information(self, "提示", "OBB检测完成，数据已写入数据库")
        except Exception as e:
            print("model2 运行出错:", e)
            self.show_message(f"model2 运行出错: {e}", title="错误")
            return

        # 读取 final.py 的日志文件，显示最终仪表读数
        try:
            log_path = final.final()
            if not os.path.exists(log_path):
                self.show_message("未找到日志文件", title="错误")
                return

            final_line = None
            with open(log_path, "r", encoding="utf-8") as f:
                for line in f:
                    if "仪表读数" in line:
                        final_line = line.strip()

            if final_line is None:
                final_line = "日志中未找到仪表读数"

            print("Final 结果:", final_line)
            self.show_message(final_line, title="最终结果")

            # ------------------ 保存到 task 表 ------------------
            # 如果当前 task_id 为空，则新建任务
            if not self.current_task_id:
                self.current_task_id = self.db.insert_task(user_id=self.current_user_id)

            # 提取数值并保存
            try:
                value_str = final_line.split(":")[-1].strip()
                value = float(value_str)
                self.db.update_task_result(self.current_task_id, value)
                print(f"已将最终仪表读数 {value} 保存到 task_id={self.current_task_id}")
            except Exception as e:
                print("保存最终仪表读数到 task 出错:", e)

        except Exception as e:
            print("运行 final 出错:", e)
            self.show_message(f"运行 final 出错: {e}", title="错误")

    # def run_final(self):
    #     if self.current_user_id:
    #         try:
    #             # 调用 model2，自动创建 task 并写入 rotate 表和 image_record
    #             model2.model2(user_id=self.current_user_id)
    #             QMessageBox.information(self, "提示", "OBB检测完成，数据已写入数据库")
    #         except Exception as e:
    #             print("model2 运行出错:", e)
    #             self.show_message(f"model2 运行出错: {e}", title="错误")
    #             return
    #     else:
    #         QMessageBox.warning(self, "警告", "请先登录")
    #         return
    #
    #     # 读取 final.py 的日志文件，显示最终仪表读数
    #     try:
    #         log_path = final.final()
    #         if not os.path.exists(log_path):
    #             self.show_message("未找到日志文件", title="错误")
    #             return
    #
    #         final_line = None
    #         with open(log_path, "r", encoding="utf-8") as f:
    #             for line in f:
    #                 if "仪表读数" in line:
    #                     final_line = line.strip()
    #
    #         if final_line is None:
    #             final_line = "日志中未找到仪表读数"
    #
    #         print("Final 结果:", final_line)
    #         self.show_message(final_line, title="最终结果")
    #
    #         # 更新最后一个 task 的 result_value
    #         if self.current_task_id and "仪表读数" in final_line:
    #             try:
    #                 value = float(final_line.split(":")[-1])
    #                 self.db.update_task_result(self.current_task_id, value)
    #             except:
    #                 pass
    #
    #     except Exception as e:
    #         print("运行 final 出错:", e)
    #         self.show_message(f"运行 final 出错: {e}", title="错误")

# # -*- coding: utf-8 -*-
# import sqlite3
# import hashlib
# import os
# import cv2
# from PySide6.QtWidgets import QApplication, QMainWindow, QLineEdit, QMessageBox, QFileDialog
# from PySide6.QtGui import QImage, QPixmap
# from PySide6.QtCore import Qt, QTimer, QDateTime
#
# from ui_form import Ui_GUI
#
# import model1
# import model2
# import final
# import dial_calibration
#
#
#
# def hash_password(password: str) -> str:
#     """返回密码的 SHA-256 哈希值"""
#     return hashlib.sha256(password.encode("utf-8")).hexdigest()
#
#
# class GUI(QMainWindow):
#     def __init__(self, parent=None):
#         super().__init__(parent)
#         self.ui = Ui_GUI()
#         self.ui.setupUi(self)
#
#         # 鼠标拖拽
#         self._is_dragging = False
#         self._drag_position = None
#
#         # 隐藏标题栏
#         self.setWindowFlags(Qt.FramelessWindowHint)
#         self.show_menu_page()
#
#         # ⭐ 新增：初始化时间显示定时器
#         self.time_timer = QTimer()
#         self.time_timer.timeout.connect(self.update_time)
#         self.time_timer.start(1000)  # 每秒更新一次
#         self.update_time()  # 启动时立即显示一次
#
#         # 初始化摄像头相关变量
#         self.cap = None
#         self.timer = QTimer()
#         self.timer.timeout.connect(self.update_frame)
#         self.is_camera_open = False
#         self.current_frame = None
#
#         # 按钮连接
#         self.ui.closeButton.clicked.connect(self.close)
#         self.ui.minButton.clicked.connect(self.showMinimized)
#         self.ui.maxButton.clicked.connect(self.toggle_max_restore)
#         self.ui.Button_last.clicked.connect(self.show_menu_page)
#
#         self.ui.btn_open_image.clicked.connect(self.open_image)
#         self.ui.btn_capture_frame.clicked.connect(self.capture_frame)
#
#         self.ui.btn_dial_proofread.clicked.connect(self.run_dial_proofread)
#         self.ui.btn_result.clicked.connect(self.run_final)
#
#         # 初始化密码输入框
#         self.ui.password_LineEdit.setEchoMode(QLineEdit.Password)
#         self.ui.comfirm_LineEdit.setEchoMode(QLineEdit.Password)
#         self.ui.Password_LineEdit.setEchoMode(QLineEdit.Password)
#
#         # 映射隐藏/显示密码按钮
#         self.password_map = {
#             "hideButton": (self.ui.Password_LineEdit, self.ui.hideButton, self.ui.displayButton),
#             "displayButton": (self.ui.Password_LineEdit, self.ui.hideButton, self.ui.displayButton),
#             "hideButton1": (self.ui.comfirm_LineEdit, self.ui.hideButton1, self.ui.displayButton1),
#             "displayButton1": (self.ui.comfirm_LineEdit, self.ui.hideButton1, self.ui.displayButton1),
#             "hideButton2": (self.ui.password_LineEdit, self.ui.hideButton2, self.ui.displayButton2),
#             "displayButton2": (self.ui.password_LineEdit, self.ui.hideButton2, self.ui.displayButton2),
#         }
#         for btn_name in self.password_map:
#             getattr(self.ui, btn_name).clicked.connect(self.toggle_password)
#
#         # 页面切换按钮
#         self.ui.enroll_pushButton.clicked.connect(self.show_enroll_page)
#         self.ui.login_pushButton.clicked.connect(self.show_login_page)
#         self.ui.loginButton.clicked.connect(self.login_user)
#         self.ui.enrollButton.clicked.connect(self.register_user)
#
#         self.is_maximized = False
#
#         # 初始化数据库
#         self.init_db()
#
#     def mousePressEvent(self, event):
#         """按下鼠标时记录位置"""
#         if event.button() == Qt.LeftButton:
#             self._is_dragging = True
#             self._drag_position = event.globalPosition().toPoint() - self.frameGeometry().topLeft()
#             event.accept()
#
#     def mouseMoveEvent(self, event):
#         """移动鼠标时更新窗口位置"""
#         if self._is_dragging and event.buttons() & Qt.LeftButton:
#             self.move(event.globalPosition().toPoint() - self._drag_position)
#             event.accept()
#
#     def mouseReleaseEvent(self, event):
#         """释放鼠标时停止拖动"""
#         if event.button() == Qt.LeftButton:
#             self._is_dragging = False
#             event.accept()
#
#     def update_time(self):
#         """更新时间到 time_label"""
#         current_time = QDateTime.currentDateTime()
#         time_str = current_time.toString("yyyy-MM-dd HH:mm:ss")  # ⭐ 年-月-日 时:分:秒
#         self.ui.time_label.setText(time_str)
#
#     def toggle_max_restore(self):
#         if self.is_maximized:
#             self.showNormal()
#             self.is_maximized = False
#         else:
#             self.showMaximized()
#             self.is_maximized = True
#
#     def toggle_password(self):
#         sender = self.sender()
#         btn_name = sender.objectName()
#         line_edit, hide_btn, display_btn = self.password_map[btn_name]
#
#         if "hide" in btn_name and line_edit.echoMode() == QLineEdit.Password:
#             line_edit.setEchoMode(QLineEdit.Normal)
#             hide_btn.hide()
#             display_btn.show()
#         elif "display" in btn_name and line_edit.echoMode() == QLineEdit.Normal:
#             line_edit.setEchoMode(QLineEdit.Password)
#             display_btn.hide()
#             hide_btn.show()
#
#     def hide_all_pages(self):
#         self.ui.menu_widget.hide()
#         self.ui.enroll_widget.hide()
#         self.ui.login_widget.hide()
#         self.ui.operator_widget.hide()
#
#     def show_menu_page(self):
#         self.hide_all_pages()
#         self.ui.menu_widget.show()
#
#     def show_enroll_page(self):
#         self.hide_all_pages()
#         self.ui.enroll_widget.show()
#
#     def show_login_page(self):
#         self.hide_all_pages()
#         self.ui.login_widget.show()
#
#     def show_operator_page(self):
#         self.hide_all_pages()
#         self.ui.operator_widget.show()
#
#     def init_db(self):
#         """初始化数据库"""
#         self.conn = sqlite3.connect("../user.db")
#         cursor = self.conn.cursor()
#         cursor.execute("""
#             CREATE TABLE IF NOT EXISTS users (
#                 username TEXT PRIMARY KEY,
#                 password TEXT NOT NULL
#             )
#         """)
#         self.conn.commit()
#
#     def show_message(self, text, title="提示"):
#         """手动关闭弹窗"""
#         msg = QMessageBox(self)
#         msg.setWindowTitle(title)
#         msg.setText(text)
#         msg.setIcon(QMessageBox.Information)
#         msg.setStandardButtons(QMessageBox.Ok)
#         msg.exec()
#
#     def register_user(self):
#         username = self.ui.username_LineEdit.text().strip()
#         password = self.ui.password_LineEdit.text()
#         confirm = self.ui.comfirm_LineEdit.text()
#
#         if password != confirm:
#             self.show_message("两次输入的密码不一致，请重新输入")
#             return
#
#         if not username or not password:
#             self.show_message("用户名和密码不能为空")
#             return
#
#         hashed_password = hash_password(password)
#
#         cursor = self.conn.cursor()
#         try:
#             cursor.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, hashed_password))
#             self.conn.commit()
#             self.show_message("注册成功！")
#             self.ui.username_LineEdit.clear()
#             self.ui.password_LineEdit.clear()
#             self.ui.comfirm_LineEdit.clear()
#             self.show_operator_page()
#         except sqlite3.IntegrityError:
#             self.show_message("该用户名已存在")
#
#     def login_user(self):
#         username = self.ui.Username_LineEdit.text().strip()
#         password = self.ui.Password_LineEdit.text()
#
#         if not username or not password:
#             self.show_message("用户名和密码不能为空")
#             return
#
#         hashed_password = hash_password(password)
#         cursor = self.conn.cursor()
#         cursor.execute("SELECT password FROM users WHERE username = ?", (username,))
#         result = cursor.fetchone()
#
#         if result is None:
#             self.show_message("该账号不存在")
#         else:
#             saved_password = result[0]
#             if saved_password != hashed_password:
#                 self.show_message("密码输入错误")
#             else:
#                 self.show_message("登录成功！")
#                 self.show_operator_page()
#
#     def display_image(self, img_path):
#         """在 image_board 显示图片"""
#         pixmap = QPixmap(img_path)
#         self.ui.image_board.setPixmap(
#             pixmap.scaled(
#                 self.ui.image_board.width(),
#                 self.ui.image_board.height(),
#                 Qt.KeepAspectRatio,
#                 Qt.SmoothTransformation
#             )
#         )
#
#     # 打开图片按钮
#     def open_image(self):
#         file_path, _ = QFileDialog.getOpenFileName(
#             self, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp)"
#         )
#         if file_path:
#             self.display_image(file_path)
#             # ✅ 调用 model1，并检查返回值
#             detected = model1.process_image(file_path)
#             if not detected:
#                 QMessageBox.warning(self, "警告", "输入图片非法")
#
#     def capture_frame(self):
#         if not self.is_camera_open:
#             # 第一次点击，打开摄像头
#             self.cap = cv2.VideoCapture(0)
#             if not self.cap.isOpened():
#                 print("无法打开摄像头")
#                 return
#             self.timer.start(30)  # 30ms 更新一次帧
#             self.is_camera_open = True
#             self.ui.btn_capture_frame.setText("拍摄")
#         else:
#             # 第二次点击，拍摄当前帧
#             if self.current_frame is not None:
#                 self.timer.stop()
#                 self.cap.release()
#                 self.is_camera_open = False
#                 self.ui.btn_capture_frame.setText("打开摄像头")
#
#                 # 保存拍摄图片
#                 temp_path = "database/captured_frame.jpg"
#                 cv2.imwrite(temp_path, self.current_frame)
#
#                 # 显示在 image_board
#                 self.display_image(temp_path)
#
#                 # ✅ 调用 model1 并检查返回值
#                 detected = model1.process_image(temp_path)
#                 if not detected:
#                     QMessageBox.warning(self, "警告", "输入图片非法")
#
#     def update_frame(self):
#         """定时器回调：显示摄像头实时帧"""
#         ret, frame = self.cap.read()
#         if ret:
#             self.current_frame = frame.copy()
#             rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
#             h, w, ch = rgb_image.shape
#             bytes_per_line = ch * w
#             qt_image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
#             pixmap = QPixmap.fromImage(qt_image)
#             self.ui.image_board.setPixmap(
#                 pixmap.scaled(
#                     self.ui.image_board.width(),
#                     self.ui.image_board.height(),
#                     Qt.KeepAspectRatio,
#                     Qt.SmoothTransformation
#                 )
#             )
#
#     def run_dial_proofread(self):
#         """依次调用 dial_calibration → model2，并显示校正后的图片"""
#         try:
#             # 调用 dial_calibration.py
#             if hasattr(dial_calibration, "dial_calibration"):
#                 dial_calibration.dial_calibration()
#             # elif hasattr(dial_calibration, "process"):
#             #     dial_calibration.process()
#             else:
#                 print("⚠️ dial_calibration.py 没有找到入口函数")
#
#             # 显示 dial_perspective_corrected.png
#             corrected_img_path = "database/dial_perspective_corrected.png"
#             if os.path.exists(corrected_img_path):
#                 pixmap = QPixmap(corrected_img_path)
#                 self.ui.output_board.setPixmap(
#                     pixmap.scaled(
#                         self.ui.output_board.width(),
#                         self.ui.output_board.height(),
#                         Qt.KeepAspectRatio,
#                         Qt.SmoothTransformation
#                     )
#                 )
#             else:
#                 self.show_message("未找到 dial_perspective_corrected.png", title="错误")
#         except Exception as e:
#             print("运行校正时出错:", e)
#             self.show_message(f"运行校正时出错: {e}", title="错误")
#
#     def run_final(self):
#
#         # 调用 model2.py
#         if hasattr(model2, "model2"):
#             model2.model2()
#         # elif hasattr(model2, "process"):
#         #     model2.process()
#         else:
#             print("⚠️ model2.py 没有找到入口函数")
#
#         """调用 final.py 并显示日志中的最终仪表读数"""
#         try:
#             log_path = final.final()   # 返回日志路径
#
#             if not os.path.exists(log_path):
#                 self.show_message("未找到日志文件", title="错误")
#                 return
#
#             # 读取日志文件最后一行包含"仪表读数"的内容
#             final_line = None
#             with open(log_path, "r", encoding="utf-8") as f:
#                 for line in f:
#                     if "仪表读数" in line:
#                         final_line = line.strip()
#
#             if final_line is None:
#                 final_line = "日志中未找到仪表读数"
#
#             # 打印到终端
#             print("Final 结果:", final_line)
#
#             # 弹窗显示
#             self.show_message(final_line, title="最终结果")
#
#         except Exception as e:
#             print("运行 final 出错:", e)
#             self.show_message(f"运行 final 出错: {e}", title="错误")



# if __name__ == "__main__":
#     app = QApplication(sys.argv)
#     widget = GUI()
#     widget.show()
#     sys.exit(app.exec())

