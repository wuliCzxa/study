# model1.py
import os
import cv2
from ultralytics import YOLO
from dial_data import DialDatabase

def process_image(img_path, save_dir="database"):
    """
    对传入的图片进行 YOLO 检测：
    1. 过滤置信度 >= 0.7
    2. 对每个检测框裁剪图片并保存
    3. 保存带框图片

    返回:
        detected: bool, 是否检测到至少一个框
        boxes_info: list, 每个框的坐标信息 (cx, cy, x1,y1,...,x4,y4, classes_id)
    """
    if not os.path.exists(img_path):
        print("图片路径不存在:", img_path)
        return False, []

    if not os.path.exists(save_dir):
        os.makedirs(save_dir)

    model = YOLO("./runs/detect/model1/weights/best.pt")
    results = model([img_path])
    detected = False
    boxes_info = []

    for result in results:
        boxes = result.boxes

        if boxes is not None and len(boxes) > 0:
            high_conf_indices = boxes.conf >= 0.7
            filtered_boxes = boxes[high_conf_indices]

            if len(filtered_boxes) > 0:
                detected = True
                result.boxes = filtered_boxes
                original_image = result.orig_img

                for i, box in enumerate(filtered_boxes):
                    x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
                    cropped_image = original_image[y1:y2, x1:x2]

                    crop_path = os.path.join(save_dir, f"new.jpg")
                    cv2.imwrite(crop_path, cropped_image)
                    print(f"已保存裁剪图: {crop_path}")

                    # 获取中心点
                    cx = float((x1 + x2) / 2)
                    cy = float((y1 + y2) / 2)
                    classes_id = int(box.cls[0])

                    # 矩形四角
                    corners = [
                        (x1, y2),  # 左下
                        (x2, y2),  # 右下
                        (x2, y1),  # 右上
                        (x1, y1)   # 左上
                    ]
                    boxes_info.append({
                        'cx': cx,
                        'cy': cy,
                        'corners': corners,
                        'classes_id': classes_id
                    })

        # 保存检测结果图
        result.save(save_dir)
        print(f"已保存检测结果到: {save_dir}")

    return detected, boxes_info

def model1(user_id, weights_path, img_path, save_dir="database"):
    """
    1. 加载 YOLO 模型
    2. 调用 process_image 处理图片
    3. 将结果写入 dial_data.db
    """
    if not os.path.exists(weights_path):
        print("权重文件不存在:", weights_path)
        return

    db = DialDatabase()
    # 创建任务
    task_id = db.insert_task(user_id)

    detected, boxes_info = process_image(img_path, save_dir)

    if not detected:
        print("输入图片非法")
        return

    # 插入矩形框信息
    for box in boxes_info:
        db.insert_rectangle(
            task_id=task_id,
            classes_id=box['classes_id'],
            cx=box['cx'],
            cy=box['cy'],
            x1=box['corners'][0][0],
            y1=box['corners'][0][1],
            x2=box['corners'][1][0],
            y2=box['corners'][1][1],
            x3=box['corners'][2][0],
            y3=box['corners'][2][1],
            x4=box['corners'][3][0],
            y4=box['corners'][3][1]
        )

    # 插入检测结果图路径
    detected_img_path = os.path.join(save_dir, "dial_detect.jpg")
    db.insert_image(task_id, stage="yolo检测", image_path=detected_img_path)
    db.close()
    print("model1 数据已写入数据库")

# import os
# import cv2
# from ultralytics import YOLO
#
# def process_image(img_path, save_dir="database"):
#     """
#     对传入的图片进行 YOLO 检测：
#     1. 过滤置信度 >= 0.7
#     2. 对每个检测框裁剪图片并保存
#     3. 保存带框图片
#
#     返回:
#         detected: bool, 是否检测到至少一个框
#     """
#     if not os.path.exists(img_path):
#         print("图片路径不存在:", img_path)
#         return False
#
#     if not os.path.exists(save_dir):
#         os.makedirs(save_dir)
#
#     model = YOLO("./runs/detect/model1/weights/best.pt")
#
#     results = model([img_path])
#     detected = False  # 标记是否检测到目标
#
#     # 遍历结果
#     for result in results:
#         boxes = result.boxes
#
#         if boxes is not None and len(boxes) > 0:
#             high_conf_indices = boxes.conf >= 0.7
#             filtered_boxes = boxes[high_conf_indices]
#
#             if len(filtered_boxes) > 0:
#                 detected = True  # 至少有一个框被识别
#                 result.boxes = filtered_boxes
#                 original_image = result.orig_img
#
#                 # 裁剪并保存每个目标
#                 for i, box in enumerate(filtered_boxes):
#                     x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
#                     cropped_image = original_image[y1:y2, x1:x2]
#
#                     crop_path = os.path.join(save_dir, f"new.jpg")
#                     cv2.imwrite(crop_path, cropped_image)
#                     print(f"已保存裁剪图: {crop_path}")
#
#         # 保存检测结果图
#         result.save(save_dir)
#         print(f"已保存检测结果到: {save_dir}")
#
#     return detected
#
#
# def model1(weights_path, img_path, save_dir="database"):
#     """
#     1. 加载 YOLO 模型
#     2. 调用 process_image 处理图片
#     """
#     if not os.path.exists(weights_path):
#         print("权重文件不存在:", weights_path)
#         return
#
#     model = YOLO(weights_path)
#     detected = process_image(img_path, model, save_dir)
#
#     if not detected:
#         print("输入图片非法")



# """
# 读取检测结果并排序，开始刻度线坐标即为如下所示：# 先按第一列排序，再按第二列排序
# sorted_rows = sorted(rows_as_lists, key=lambda x: (float(x[0]), float(x[1])))
# # print(sorted_rows)
# # 起始刻度线坐标
# x_s1 = width * float(sorted_rows[0][1])
# y_s1 = width * float(sorted_rows[0][2])
# x_s2 = width * float(sorted_rows[0][3])
# y_s2 = width * float(sorted_rows[0][4])
# x_s3 = width * float(sorted_rows[0][5])
# y_s3 = width * float(sorted_rows[0][6])
# x_s4 = width * float(sorted_rows[0][7])
# y_s4 = width * float(sorted_rows[0][8])
# # 拟合起始刻度线坐标
# x_sf = (x_s1 + x_s2 + x_s3 + x_s4) / 4
# y_sf = (y_s1 + y_s2 + y_s3 + y_s4) / 4
#
# # 终止刻度线坐标
# x_e1 = width * float(sorted_rows[1][1])
# y_e1 = width * float(sorted_rows[1][2])
# x_e2 = width * float(sorted_rows[1][3])
# y_e2 = width * float(sorted_rows[1][4])
# x_e3 = width * float(sorted_rows[1][5])
# y_e3 = width * float(sorted_rows[1][6])
# x_e4 = width * float(sorted_rows[1][7])
# y_e4 = width * float(sorted_rows[1][8])
# # 拟合终止刻度线坐标
# x_ef = (x_e1 + x_e2 + x_e3 + x_e4) / 4
# y_ef = (y_e1 + y_e2 + y_e3 + y_e4) / 4
#
# # 指针端点坐标
# x_p1 = width * float(sorted_rows[3][1])
# y_p1 = width * float(sorted_rows[3][2])
# x_p2 = width * float(sorted_rows[3][3])
# y_p2 = width * float(sorted_rows[3][4])
# x_p3 = width * float(sorted_rows[3][5])
# y_p3 = width * float(sorted_rows[3][6])
# x_p4 = width * float(sorted_rows[3][7])
# y_p4 = width * float(sorted_rows[3][8])
# # 拟合指针端点坐标
# x_pf = (x_p1 + x_p2 + x_p3 + x_p4) / 4
# y_pf = (y_p1 + y_p2 + y_p3 + y_p4) / 4
#
# # 中间刻度线坐标
# x_z1 = width * float(sorted_rows[2][1])
# y_z1 = width * float(sorted_rows[2][2])
# x_z2 = width * float(sorted_rows[2][3])
# y_z2 = width * float(sorted_rows[2][4])
# x_z3 = width * float(sorted_rows[2][5])
# y_z3 = width * float(sorted_rows[2][6])
# x_z4 = width * float(sorted_rows[2][7])
# y_z4 = width * float(sorted_rows[2][8])
# # 拟合中间刻度线坐标
# x_zf = (x_z1 + x_z2 + x_z3 + x_z4) / 4
# y_zf = (y_z1 + y_z2 + y_z3 + y_z4) / 4相应的，获得结束刻度线坐标以及指针中心坐标，再计算开始刻度线与结束刻度线交点坐标p1、开始刻度线与指针交点坐标p2、结束刻度线与指针交点坐标p3，计算p1、p2、p3之间距离，取距离最小的两个交点的平均值即为圆心坐标c1，由角度法计算指针与开始刻度线所成角度与结束刻度线与开始刻度线所成角度之比，即为仪表读数。
# （注：类别名称为Pointer，标签索引为2的目标为指针检测框；类别名称为Scale，标签索引为0的目标为开始与结束刻度检测框。其中，对于开始与结束刻度检测框而言，所有坐标的横坐标即全部x的值更小的检测框为开始刻度检测框，所有坐标的横坐标即全部x的值更大的检测框为结束刻度检测框）
#     """