# -*- coding: utf-8 -*-
"""
dial_data.py
数据库存储模块：任务表、图片记录表、矩形框表、旋转框表
"""
import os
import re
import sqlite3
from datetime import datetime


class DialDatabase:
    def __init__(self, db_name="dial_data.db"):
        """初始化数据库连接，并创建表"""
        self.conn = sqlite3.connect(db_name)
        self.cursor = self.conn.cursor()
        self.create_tables()

    def create_tables(self):
        """创建数据表"""

        # 用户表
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS user (
                user_id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL UNIQUE,
                password TEXT NOT NULL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        # 任务表
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS task (
                task_id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER,
                timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                result_value REAL
            )
        """)

        # 图片记录表
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS image_record (
                image_id INTEGER PRIMARY KEY AUTOINCREMENT,
                task_id INTEGER,
                stage TEXT,
                image_path TEXT,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        # 矩形框表
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS rectangle (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                task_id INTEGER,
                classes_id INTEGER,
                cx REAL, cy REAL,
                x1 REAL, y1 REAL,
                x2 REAL, y2 REAL,
                x3 REAL, y3 REAL,
                x4 REAL, y4 REAL
            )
        """)

        # 旋转框表
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS rotate (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                task_id INTEGER,
                classes_id INTEGER,
                cx REAL, cy REAL,
                x1 REAL, y1 REAL,
                x2 REAL, y2 REAL,
                x3 REAL, y3 REAL,
                x4 REAL, y4 REAL
            )
        """)

        # 新增 output 表，存储仪表结果
        self.cursor.execute("""
            CREATE TABLE IF NOT EXISTS output (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                task_id INTEGER,
                start_cx REAL, start_cy REAL,
                end_cx REAL, end_cy REAL,
                mid_cx REAL, mid_cy REAL,
                pointer_x REAL, pointer_y REAL,
                center_x REAL, center_y REAL,
                angle1 REAL, angle2 REAL,
                result_value REAL,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """)

        self.conn.commit()

    # 插入用户
    def insert_user(self, username, password):
        self.cursor.execute("""
            INSERT INTO user (username, password) VALUES (?, ?)
        """, (username, password))
        self.conn.commit()
        return self.cursor.lastrowid

    def get_user(self, username):
        self.cursor.execute("SELECT user_id, password FROM user WHERE username=?", (username,))
        return self.cursor.fetchone()

    # 插入任务
    def insert_task(self, user_id, result_value=None):
        self.cursor.execute("""
            INSERT INTO task (user_id, result_value) VALUES (?, ?)
        """, (user_id, result_value))
        self.conn.commit()
        return self.cursor.lastrowid  # 返回 task_id

    # 更新任务结果
    def update_task_result(self, task_id, result_value):
        self.cursor.execute("""
            UPDATE task SET result_value=? WHERE task_id=?
        """, (result_value, task_id))
        self.conn.commit()

    # 插入图片记录
    def insert_image(self, task_id, stage, image_path):
        self.cursor.execute("""
            INSERT INTO image_record (task_id, stage, image_path) 
            VALUES (?, ?, ?)
        """, (task_id, stage, image_path))
        self.conn.commit()
        return self.cursor.lastrowid

    # 插入矩形框
    def insert_rectangle(self, task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4):
        self.cursor.execute("""
            INSERT INTO rectangle (task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4)
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        """, (task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4))
        self.conn.commit()
        return self.cursor.lastrowid

    # 插入旋转框
    def insert_rotate(self, task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4):
        self.cursor.execute(""" 
            INSERT INTO rotate (task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4) 
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) 
        """,(task_id, classes_id, cx, cy, x1, y1, x2, y2, x3, y3, x4, y4))
        self.conn.commit()
        return self.cursor.lastrowid
    # def insert_rotate_from_file(self, task_id, file_path="./output/scale_latest.txt"):
    #     """
    #     解析 scale_latest.txt 并将检测框写入 rotate 表
    #     每个目标框按文件内容存储为一条 rotate 记录
    #     """
    #     if not os.path.exists(file_path):
    #         print(f"文件不存在: {file_path}")
    #         return
    #
    #     with open(file_path, "r", encoding="utf-8") as f:
    #         content = f.read()
    #
    #     # 使用正则匹配每个框的数据
    #     pattern = re.compile(
    #         r"Num: (\d+)\s*"
    #         r"Center: \(([-\d.]+), ([-\d.]+)\)\s*"
    #         r"Corner_Lower_Left \(x1,y1\):\(([-\d.]+), ([-\d.]+)\)\s*"
    #         r"Corner_Lower_Right \(x2,y2\):\(([-\d.]+), ([-\d.]+)\)\s*"
    #         r"Corner_Upper_Right \(x3,y3\):\(([-\d.]+), ([-\d.]+)\)\s*"
    #         r"Corner_Upper_Left \(x4,y4\):\(([-\d.]+), ([-\d.]+)\)",
    #         re.MULTILINE
    #     )
    #
    #     matches = pattern.findall(content)
    #     if not matches:
    #         print("未在文件中找到目标框信息")
    #         return
    #
    #     for m in matches:
    #         classes_id = int(m[0])
    #         cx, cy = float(m[1]), float(m[2])
    #         x1, y1 = float(m[3]), float(m[4])
    #         x2, y2 = float(m[5]), float(m[6])
    #         x3, y3 = float(m[7]), float(m[8])
    #         x4, y4 = float(m[9]), float(m[10])
    #
    #         self.insert_rotate(
    #             task_id=task_id,
    #             classes_id=classes_id,
    #             cx=cx, cy=cy,
    #             x1=x1, y1=y1,
    #             x2=x2, y2=y2,
    #             x3=x3, y3=y3,
    #             x4=x4, y4=y4
    #         )

    # ------------------ 新增方法：解析 output_dial_logs.txt ------------------
    def insert_output_from_file(self, task_id, file_path="output/output_dial_logs.txt"):
        """
        读取 output_dial_logs.txt 最新时间戳的数据，并插入 output 表
        """
        if not os.path.exists(file_path):
            print(f"文件不存在: {file_path}")
            return

        with open(file_path, "r", encoding="utf-8") as f:
            content = f.read()

        # 使用正则匹配所有运行时间块
        blocks = re.findall(r"==== 运行时间: (.*?) ====(.*?)(?==== 运行时间:|$)", content, re.S)
        if not blocks:
            print("未在文件中找到任何数据")
            return

        # 获取最新时间戳的数据
        latest_time_str, latest_data = blocks[-1]
        latest_time = datetime.strptime(latest_time_str.strip(), "%Y-%m-%d %H:%M:%S")

        # 匹配各个字段
        def parse_point(pattern, text):
            m = re.search(pattern, text)
            return tuple(map(float, m.groups())) if m else (None, None)

        start_cx, start_cy = parse_point(r"起始刻度线中心: \(([-\d.]+), ([-\d.]+)\)", latest_data)
        end_cx, end_cy = parse_point(r"终止刻度线中心: \(([-\d.]+), ([-\d.]+)\)", latest_data)
        mid_cx, mid_cy = parse_point(r"中间刻度中心: \(([-\d.]+), ([-\d.]+)\)", latest_data)
        pointer_x, pointer_y = parse_point(r"指针端点: \(([-\d.]+), ([-\d.]+)\)", latest_data)
        center_x, center_y = parse_point(r"计算得出的圆心: \(([-\d.]+), ([-\d.]+)\)", latest_data)

        angle1 = None
        m = re.search(r"夹角 a1: ([-\d.]+)°", latest_data)
        if m:
            angle1 = float(m.group(1))

        angle2 = None
        m = re.search(r"夹角 a2: ([-\d.]+)°", latest_data)
        if m:
            angle2 = float(m.group(1))

        result_value = None
        m = re.search(r"最终仪表读数: ([-\d.]+)", latest_data)
        if m:
            result_value = float(m.group(1))

        # 插入数据库
        self.cursor.execute("""
            INSERT INTO output (
                task_id, start_cx, start_cy, end_cx, end_cy, mid_cx, mid_cy,
                pointer_x, pointer_y, center_x, center_y, angle1, angle2, result_value
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        """, (
            task_id, start_cx, start_cy, end_cx, end_cy, mid_cx, mid_cy,
            pointer_x, pointer_y, center_x, center_y, angle1, angle2, result_value
        ))
        self.conn.commit()
        print(f"已将最新仪表数据插入 output 表 (task_id={task_id})")

    # ------------------ 新增方法：获取用户最新 task_id ------------------
    def get_last_task_id(self, user_id):
        """
        返回指定用户的最新 task_id
        """
        self.cursor.execute("""
            SELECT task_id FROM task
            WHERE user_id=?
            ORDER BY timestamp DESC
            LIMIT 1
        """, (user_id,))
        row = self.cursor.fetchone()
        return row[0] if row else None

    def close(self):
        """关闭数据库连接"""
        self.conn.close()

