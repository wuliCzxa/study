# # # from fileinput import filename
# # # from ultralytics import YOLO
# # # from datetime import datetime
# # # import os
# # #
# # #
# # # # 获取推理结果并写入到文件的逻辑
# # # def process_inference_results(results, model):
# # #     timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
# # #     content = f"Timestamp: {timestamp}\n\n"
# # #     all_boxes = []
# # #
# # #     # 遍历所有检测结果，筛选出符合条件的目标框
# # #     for result in results:
# # #         obb = result.obb
# # #         if obb is not None:
# # #             high_conf_indices = obb.conf >= 0.5
# # #             filtered_obb = obb[high_conf_indices]
# # #             result.obb = filtered_obb
# # #
# # #             xywhr = result.obb.xywhr
# # #             xyxyxyxy = result.obb.xyxyxyxy
# # #             labels = result.obb.cls
# # #
# # #             for box, corners, label in zip(xywhr, xyxyxyxy, labels):
# # #                 cx, cy = float(box[0]), float(box[1])
# # #                 label_name = model.names[int(label)]
# # #
# # #                 # 存储检测框数据及其横坐标
# # #                 all_boxes.append({
# # #                     'label': label_name,
# # #                     'label_num': int(label),
# # #                     'center_x': cx,
# # #                     'center_y': cy,
# # #                     'corners': corners
# # #                 })
# # #
# # #     # 按照横坐标 (center_x) 排序
# # #     sorted_boxes = sorted(all_boxes, key=lambda x: x['center_x'])
# # #
# # #     # 构建排序后的内容
# # #     for box in sorted_boxes:
# # #         content += f"Label: {box['label']}, Num: {box['label_num']}\n"
# # #         content += f"Center: ({box['center_x']:.6f}, {box['center_y']:.6f})\n"
# # #         content += "Corner points:\n"
# # #         content += f"Corner_Lower_Left (x1,y1):({float(box['corners'][0][0]):.6f}, {float(box['corners'][0][1]):.6f})\n"
# # #         content += f"Corner_Lower_Right (x2,y2):({float(box['corners'][1][0]):.6f}, {float(box['corners'][1][1]):.6f})\n"
# # #         content += f"Corner_Upper_Right (x3,y3):({float(box['corners'][2][0]):.6f}, {float(box['corners'][2][1]):.6f})\n"
# # #         content += f"Corner_Upper_Left (x4,y4):({float(box['corners'][3][0]):.6f}, {float(box['corners'][3][1]):.6f})\n"
# # #         content += "\n"
# # #
# # #     return content
# # #
# # #
# # # # 写入处理后的推理结果到文件
# # # def write_to_file(content):
# # #     with open("output/scale_latest.txt", "w") as f_now:
# # #         f_now.write(content)
# # #
# # #     prepend_to_file("output/scale_history.txt", content)
# # #
# # #
# # # # 将新内容插入到 scale_history.txt 文件的开头
# # # def prepend_to_file(file_path, content):
# # #     with open(file_path, "r") as f:
# # #         existing_content = f.read()
# # #
# # #     with open(file_path, "w") as f:
# # #         f.write(content + existing_content)
# # #
# # #
# # # # 主函数
# # # def model2():
# # #     # 加载训练好的 OBB 模型
# # #     model = YOLO("./runs/obb/test003/weights/best.pt")
# # #
# # #     # 对 new.jpg 进行推理
# # #     # results = model("database/new.jpg")  # 返回一个 Results 对象列表
# # #     results = model("database/dial_perspective_corrected.png")
# # #     # results = process_inference_results(results, model)
# # #
# # #     # 处理推理结果并构建写入内容
# # #     content = process_inference_results(results, model)
# # #
# # #     # 写入到文件
# # #     write_to_file(content)
# # #
# # #     # # 显示过滤后的结果
# # #     # results[0].show()
# # #
# # #     # 保存过滤后的结果
# # #     results[0].save(filename="database/dial_perspective_corrected_obb.png")  # 保存为 new_detected.jpg
# # #
# # #
# # # # 仅在直接运行时执行
# # # # if __name__ == "__main__":
# # # #     main()
# #
# # from ultralytics import YOLO
# # from datetime import datetime
# # import os
# # from dial_data import DialDatabase  # ⭐ 导入数据库模块
# #
# # def process_inference_results(results, model):
# #     timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
# #     content = f"==== 运行时间: {timestamp} ====\n"
# #     all_boxes = []
# #
# #     for result in results:
# #         obb = result.obb
# #         if obb is not None:
# #             high_conf_indices = obb.conf >= 0.5
# #             filtered_obb = obb[high_conf_indices]
# #             result.obb = filtered_obb
# #
# #             xywhr = result.obb.xywhr
# #             xyxyxyxy = result.obb.xyxyxyxy
# #             labels = result.obb.cls
# #
# #             for box, corners, label in zip(xywhr, xyxyxyxy, labels):
# #                 cx, cy = float(box[0]), float(box[1])
# #                 label_name = model.names[int(label)]
# #
# #                 all_boxes.append({
# #                     'label': label_name,
# #                     'label_num': int(label),
# #                     'center_x': cx,
# #                     'center_y': cy,
# #                     'corners': corners
# #                 })
# #
# #     # 按横坐标排序
# #     sorted_boxes = sorted(all_boxes, key=lambda x: x['center_x'])
# #
# #     # 构建输出内容
# #     for box in sorted_boxes:
# #         content += f"Label: {box['label']}, Num: {box['label_num']}\n"
# #         content += f"Center: ({box['center_x']:.6f}, {box['center_y']:.6f})\n"
# #         content += "Corner points:\n"
# #         content += f"Corner_Lower_Left (x1,y1):({float(box['corners'][0][0]):.6f}, {float(box['corners'][0][1]):.6f})\n"
# #         content += f"Corner_Lower_Right (x2,y2):({float(box['corners'][1][0]):.6f}, {float(box['corners'][1][1]):.6f})\n"
# #         content += f"Corner_Upper_Right (x3,y3):({float(box['corners'][2][0]):.6f}, {float(box['corners'][2][1]):.6f})\n"
# #         content += f"Corner_Upper_Left (x4,y4):({float(box['corners'][3][0]):.6f}, {float(box['corners'][3][1]):.6f})\n"
# #         content += "\n"
# #
# #     return content
# #
# #
# # def write_to_file(content, user_id=None):
# #     # 保存最新文件
# #     os.makedirs("output", exist_ok=True)
# #     with open("output/scale_latest.txt", "w", encoding="utf-8") as f_now:
# #         f_now.write(content)
# #
# #     # 写入历史文件，插入到开头
# #     prepend_to_file("output/scale_history.txt", content)
# #
# #     # ⭐ 如果提供 user_id，可直接写入数据库 output 表
# #     if user_id is not None:
# #         db = DialDatabase()
# #         # 这里 user_id 对应的 task_id 可通过 insert_task 或其他逻辑获取
# #         # 假设最新 task_id 对应 user_id
# #         task_id = db.get_last_task_id(user_id)
# #         if task_id:
# #             db.insert_output_from_file(task_id, content)
# #
# #
# # def prepend_to_file(file_path, content):
# #     if os.path.exists(file_path):
# #         with open(file_path, "r", encoding="utf-8") as f:
# #             existing_content = f.read()
# #     else:
# #         existing_content = ""
# #
# #     with open(file_path, "w", encoding="utf-8") as f:
# #         f.write(content + existing_content)
# #
# #
# # def model2(user_id=None):
# #     # 加载训练好的 OBB 模型
# #     model = YOLO("./runs/obb/test003/weights/best.pt")
# #
# #     # 推理 corrected dial 图像
# #     results = model("database/dial_perspective_corrected.png")
# #
# #     # 处理推理结果
# #     content = process_inference_results(results, model)
# #
# #     # 写入文件，并可写入数据库
# #     write_to_file(content, user_id=user_id)
# #
# #     # 保存检测结果图
# #     results[0].save(filename="database/dial_perspective_corrected_obb.png")
# from ultralytics import YOLO
# from datetime import datetime
# import os
# from dial_data import DialDatabase
#
#
# def process_inference_results(results, model):
#     timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
#     content = f"==== 运行时间: {timestamp} ====\n"
#     all_boxes = []
#
#     for result in results:
#         obb = result.obb
#         if obb is not None:
#             high_conf_indices = obb.conf >= 0.5
#             filtered_obb = obb[high_conf_indices]
#             result.obb = filtered_obb
#
#             xywhr = result.obb.xywhr
#             xyxyxyxy = result.obb.xyxyxyxy
#             labels = result.obb.cls
#
#             for box, corners, label in zip(xywhr, xyxyxyxy, labels):
#                 cx, cy = float(box[0]), float(box[1])
#                 label_name = model.names[int(label)]
#
#                 all_boxes.append({
#                     'label': label_name,
#                     'label_num': int(label),
#                     'center_x': cx,
#                     'center_y': cy,
#                     'corners': corners
#                 })
#
#     # 按横坐标排序
#     sorted_boxes = sorted(all_boxes, key=lambda x: x['center_x'])
#
#     # 构建输出内容
#     for box in sorted_boxes:
#         content += f"Label: {box['label']}, Num: {box['label_num']}\n"
#         content += f"Center: ({box['center_x']:.6f}, {box['center_y']:.6f})\n"
#         content += "Corner points:\n"
#         content += f"Corner_Lower_Left (x1,y1):({float(box['corners'][0][0]):.6f}, {float(box['corners'][0][1]):.6f})\n"
#         content += f"Corner_Lower_Right (x2,y2):({float(box['corners'][1][0]):.6f}, {float(box['corners'][1][1]):.6f})\n"
#         content += f"Corner_Upper_Right (x3,y3):({float(box['corners'][2][0]):.6f}, {float(box['corners'][2][1]):.6f})\n"
#         content += f"Corner_Upper_Left (x4,y4):({float(box['corners'][3][0]):.6f}, {float(box['corners'][3][1]):.6f})\n"
#         content += "\n"
#
#     return content, sorted_boxes  # 返回 boxes 以便插入数据库
#
#
# def write_to_file(content, sorted_boxes=None, user_id=None):
#     os.makedirs("output", exist_ok=True)
#     # 写最新文件
#     with open("output/scale_latest.txt", "w", encoding="utf-8") as f_now:
#         f_now.write(content)
#
#     # 写历史文件
#     prepend_to_file("output/scale_history.txt", content)
#
#     # 写入数据库
#     if user_id is not None and sorted_boxes is not None:
#         db = DialDatabase()
#         # 创建 task 记录
#         task_id = db.insert_task(user_id)
#         for box in sorted_boxes:
#             corners = box['corners']
#             db.insert_rotate(
#                 task_id=task_id,
#                 classes_id=box['label_num'],
#                 cx=box['center_x'],
#                 cy=box['center_y'],
#                 x1=corners[0][0], y1=corners[0][1],
#                 x2=corners[1][0], y2=corners[1][1],
#                 x3=corners[2][0], y3=corners[2][1],
#                 x4=corners[3][0], y4=corners[3][1]
#             )
#
#
# def prepend_to_file(file_path, content):
#     if os.path.exists(file_path):
#         with open(file_path, "r", encoding="utf-8") as f:
#             existing_content = f.read()
#     else:
#         existing_content = ""
#
#     with open(file_path, "w", encoding="utf-8") as f:
#         f.write(content + existing_content)
#
#
# def model2(user_id=None):
#     model = YOLO("./runs/obb/test003/weights/best.pt")
#     results = model("database/dial_perspective_corrected.png")
#
#     content, sorted_boxes = process_inference_results(results, model)
#
#     write_to_file(content, sorted_boxes=sorted_boxes, user_id=user_id)
#
#     # 保存可视化结果
#     results[0].save(filename="database/dial_perspective_corrected_obb.png")
from ultralytics import YOLO
from datetime import datetime
import os
from dial_data import DialDatabase
from final import final  # 你之前封装的 final 函数，用于计算仪表读数

def process_inference_results(results, model):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    content = f"==== 运行时间: {timestamp} ====\n"
    all_boxes = []

    for result in results:
        obb = result.obb
        if obb is not None:
            high_conf_indices = obb.conf >= 0.5
            filtered_obb = obb[high_conf_indices]
            result.obb = filtered_obb

            xywhr = result.obb.xywhr
            xyxyxyxy = result.obb.xyxyxyxy
            labels = result.obb.cls

            for box, corners, label in zip(xywhr, xyxyxyxy, labels):
                cx, cy = float(box[0]), float(box[1])
                label_name = model.names[int(label)]

                all_boxes.append({
                    'label': label_name,
                    'label_num': int(label),
                    'center_x': float(cx),
                    'center_y': float(cy),
                    'corners': [[float(p[0]), float(p[1])] for p in corners]
                })

    # 按横坐标排序
    sorted_boxes = sorted(all_boxes, key=lambda x: x['center_x'])

    # 构建输出内容
    for box in sorted_boxes:
        content += f"Label: {box['label']}, Num: {box['label_num']}\n"
        content += f"Center: ({box['center_x']:.6f}, {box['center_y']:.6f})\n"
        content += "Corner points:\n"
        corners = box['corners']
        content += f"Corner_Lower_Left (x1,y1):({corners[0][0]:.6f}, {corners[0][1]:.6f})\n"
        content += f"Corner_Lower_Right (x2,y2):({corners[1][0]:.6f}, {corners[1][1]:.6f})\n"
        content += f"Corner_Upper_Right (x3,y3):({corners[2][0]:.6f}, {corners[2][1]:.6f})\n"
        content += f"Corner_Upper_Left (x4,y4):({corners[3][0]:.6f}, {corners[3][1]:.6f})\n"
        content += "\n"

    return content, sorted_boxes


def prepend_to_file(file_path, content):
    if os.path.exists(file_path):
        with open(file_path, "r", encoding="utf-8") as f:
            existing_content = f.read()
    else:
        existing_content = ""
    with open(file_path, "w", encoding="utf-8") as f:
        f.write(content + existing_content)


def write_to_file(content, sorted_boxes=None, user_id=None):
    os.makedirs("output", exist_ok=True)
    # 写最新文件
    with open("output/scale_latest.txt", "w", encoding="utf-8") as f:
        f.write(content)
    # 写历史文件
    prepend_to_file("output/scale_history.txt", content)

    # 写入数据库 rotate / output
    if user_id is not None and sorted_boxes is not None:
        db = DialDatabase()
        # 创建新任务
        task_id = db.insert_task(user_id)
        # 插入 rotate 表
        for box in sorted_boxes:
            corners = box['corners']
            db.insert_rotate(
                task_id=task_id,
                classes_id=int(box['label_num']),
                cx=float(box['center_x']),
                cy=float(box['center_y']),
                x1=corners[0][0], y1=corners[0][1],
                x2=corners[1][0], y2=corners[1][1],
                x3=corners[2][0], y3=corners[2][1],
                x4=corners[3][0], y4=corners[3][1]
            )

        # 调用 final 计算仪表读数并写入 output 表
        log_file = final("output/scale_latest.txt")
        db.insert_output_from_file(task_id, log_file)


def model2(user_id=None):
    # 加载 OBB 模型
    model = YOLO("./runs/obb/test003/weights/best.pt")
    # 推理图像
    results = model("database/dial_perspective_corrected.png")
    # 处理结果
    content, sorted_boxes = process_inference_results(results, model)
    # 写入文件和数据库
    write_to_file(content, sorted_boxes=sorted_boxes, user_id=user_id)
    # 保存检测结果图
    results[0].save(filename="database/dial_perspective_corrected_obb.png")
