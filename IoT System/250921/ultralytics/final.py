# -*- coding: utf-8 -*-
import os
import re
import numpy as np
from datetime import datetime

def read_scale_latest_file(file_path):
    """解析txt获取框数据"""
    with open(file_path, 'r', encoding='utf-8') as file:
        content = file.read()

    pattern = r"Label: (.+?), Num: (\d+)\s+Center: \((.+?), (.+?)\)\s+Corner points:\nCorner_Lower_Left \(x1,y1\):\((.+?), (.+?)\)\nCorner_Lower_Right \(x2,y2\):\((.+?), (.+?)\)\nCorner_Upper_Right \(x3,y3\):\((.+?), (.+?)\)\nCorner_Upper_Left \(x4,y4\):\((.+?), (.+?)\)"
    matches = re.findall(pattern, content)

    boxes = []
    for match in matches:
        label = match[0]
        label_num = int(match[1])
        cx, cy = float(match[2]), float(match[3])
        corners = [(float(match[i]), float(match[i + 1])) for i in range(4, 12, 2)]
        boxes.append({
            'label': label,
            'label_num': label_num,
            'center': (cx, cy),
            'corners': corners
        })

    return boxes

def find_box_by_label(boxes, label_num):
    """根据Num索引框"""
    for box in boxes:
        if box['label_num'] == label_num:
            return box
    return None

def perpendicular_bisector(p1, p2):
    """返回两点垂直平分线上的两个点"""
    mid_x, mid_y = (p1[0]+p2[0])/2, (p1[1]+p2[1])/2
    if p1[0] == p2[0]:
        return (mid_x, 0), (mid_x, 1)
    if p1[1] == p2[1]:
        return (0, mid_y), (1, mid_y)
    slope = (p2[1]-p1[1])/(p2[0]-p1[0])
    perp_slope = -1/slope
    return (mid_x-1, mid_y-perp_slope), (mid_x+1, mid_y+perp_slope)
# def perpendicular_bisector(p1, p2):
#     """返回两点垂直平分线上的两个点"""
#     mid_x, mid_y = (p1[0] + p2[0]) / 2, (p1[1] + p2[1]) / 2
#     if p1[0] == p2[0]:
#         return (mid_x, 0), (mid_x, 1)
#     elif p1[1] == p2[1]:
#         return (0, mid_y), (1, mid_y)
#     else:
#         slope = (p2[1] - p1[1]) / (p2[0] - p1[0])
#         perp_slope = -1 / slope
#         return (mid_x - 1, mid_y - perp_slope), (mid_x + 1, mid_y + perp_slope)

def intersection(p1, p2, p3, p4):
    """求两条直线交点"""
    A1, B1 = p2[1] - p1[1], p1[0] - p2[0]
    C1 = A1 * p1[0] + B1 * p1[1]
    A2, B2 = p4[1] - p3[1], p3[0] - p4[0]
    C2 = A2 * p3[0] + B2 * p3[1]
    det = A1 * B2 - A2 * B1
    if det == 0:
        raise ValueError("线段垂直平分线无交点")
    x = (C1 * B2 - C2 * B1) / det
    y = (A1 * C2 - A2 * C1) / det
    return x, y

def angle_between(v1, v2):
    """计算向量夹角(0~180)"""
    unit_v1 = v1 / np.linalg.norm(v1)
    unit_v2 = v2 / np.linalg.norm(v2)
    dot_product = np.dot(unit_v1, unit_v2)
    angle = np.arccos(np.clip(dot_product, -1.0, 1.0))
    return np.degrees(angle)

def compute_circle_center_x(s1, s2, s3):
    """用三个刻度的x中心求圆心1"""
    l1 = perpendicular_bisector(s1, s3)
    l2 = perpendicular_bisector(s3, s2)
    return intersection(*l1, *l2)

def compute_circle_center_y(s1, s2, s3):
    """用三个刻度的y中心求圆心2"""
    l1 = perpendicular_bisector(s1, s3)
    l2 = perpendicular_bisector(s3, s2)
    return intersection(*l1, *l2)

def compute_angle(center, p):
    """以水平向量为基准计算完整角度(0~360)"""
    base = np.array([1, 0])
    v = np.array([p[0]-center[0], p[1]-center[1]])
    ang = angle_between(base, v)
    if v[1] < 0:
        ang = 360 - ang
    return ang

def compute_gauge_new(boxes, log_file=None):
    """新读数计算"""
    scale1 = find_box_by_label(boxes, 0)
    scale2 = find_box_by_label(boxes, 1)
    scale3 = find_box_by_label(boxes, 2)
    pointer = find_box_by_label(boxes, 3)

    if not (scale1 and scale2 and scale3 and pointer):
        raise ValueError("缺少刻度或指针框数据")

    # x方向中心点
    sx1 = ((scale1['corners'][0][0] + scale1['corners'][1][0]) / 2,
           (scale1['corners'][0][1] + scale1['corners'][1][1]) / 2)
    sx2 = ((scale2['corners'][0][0] + scale2['corners'][1][0]) / 2,
           (scale2['corners'][0][1] + scale2['corners'][1][1]) / 2)
    sx3 = ((scale3['corners'][0][0] + scale3['corners'][1][0]) / 2,
           (scale3['corners'][0][1] + scale3['corners'][1][1]) / 2)

    # y方向中心点
    sy1 = ((scale1['corners'][3][0] + scale1['corners'][2][0]) / 2,
           (scale1['corners'][3][1] + scale1['corners'][2][1]) / 2)
    sy2 = ((scale2['corners'][3][0] + scale2['corners'][2][0]) / 2,
           (scale2['corners'][3][1] + scale2['corners'][2][1]) / 2)
    sy3 = ((scale3['corners'][3][0] + scale3['corners'][2][0]) / 2,
           (scale3['corners'][3][1] + scale3['corners'][2][1]) / 2)

    # 分别求圆心
    c1 = compute_circle_center_x(sx1, sx2, sx3)
    c2 = compute_circle_center_y(sy1, sy2, sy3)

    # 取平均圆心
    center = ((c1[0]+c2[0])/2, (c1[1]+c2[1])/2)

    # s1、s2、指针中心
    s1_vec = sx1
    s2_vec = sx2
    p_center = ((pointer['corners'][0][0] + pointer['corners'][1][0]) / 2,
                (pointer['corners'][0][1] + pointer['corners'][1][1]) / 2)

    # 计算总角度α1
    v_s1 = np.array([s1_vec[0]-center[0], s1_vec[1]-center[1]])
    v_s2 = np.array([s2_vec[0]-center[0], s2_vec[1]-center[1]])
    α1 = angle_between(v_s1, v_s2)
    if not (180 < α1 < 360):
        α1 = 360 - α1
    if not (180 < α1 < 360):
        raise ValueError("α1 不满足 (180°,360°)")

    # 计算指针角度
    α2 = compute_angle(center, p_center)  # Pointer x方向角
    α3 = compute_angle(center, (p_center[0], s1_vec[1]))  # Pointer y方向角

    # 归一化映射
    r1 = α2/α1
    r2 = α3/α1
    result = (r1 + r2)/2
    final_reading = result - 0.1

    # 限制范围(-0.1,0.9)且边界不可取
    if not (-0.1 < final_reading < 0.9):
        raise ValueError(f"读数 {final_reading:.4f} 超出有效范围(-0.1,0.9)，已拒绝")

    # 日志
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log = (
        f"==== 运行时间: {timestamp} ====\n"
        f"最终圆心: {center}\n"
        f"α1: {α1:.4f}°\n"
        f"α2: {α2:.4f}°\n"
        f"α3: {α3:.4f}°\n"
        f"result: {result:.4f}\n"
        f"最终读数: {final_reading:.4f}\n"
        "----------------------------------------\n"
    )
    print(log)
    if log_file:
        os.makedirs(os.path.dirname(log_file), exist_ok=True)
        with open(log_file, "a", encoding="utf-8") as f:
            f.write(log)

    return final_reading
# def compute_gauge(boxes, log_file=None):
#     min_box = min(boxes, key=lambda x: x['center'][0])
#     max_box = max(boxes, key=lambda x: x['center'][0])
#     scale2_box = find_box_by_label(boxes, 1)
#     pointer_box = find_box_by_label(boxes, 2)
#
#     if not scale2_box or not pointer_box:
#         raise ValueError("缺少中间刻度或指针信息")
#
#     (x_sf, y_sf) = min_box['center']
#     (x_ef, y_ef) = max_box['center']
#     (x_zf, y_zf) = scale2_box['center']
#     (x_pf, y_pf) = pointer_box['center']
#
#     l1 = perpendicular_bisector((x_sf, y_sf), (x_zf, y_zf))
#     l2 = perpendicular_bisector((x_zf, y_zf), (x_ef, y_ef))
#     l3 = perpendicular_bisector((x_sf, y_sf), (x_ef, y_ef))
#
#     c_x, c_y = intersection(*l1, *l2)
#     c_x2, c_y2 = intersection(*l2, *l3)
#     c_x3, c_y3 = intersection(*l1, *l3)
#
#     c_x = (c_x + c_x2 + c_x3) / 3
#     c_y = (c_y + c_y2 + c_y3) / 3
#
#     if abs(c_x - c_y) > 100:
#         c_y = c_x
#
#     vec_rs = np.array([x_sf - c_x, y_sf - c_y])
#     vec_rp = np.array([x_pf - c_x, y_pf - c_y])
#     vec_re = np.array([x_ef - c_x, y_ef - c_y])
#
#     a1 = angle_between(vec_rs, vec_re)
#     a2 = angle_between(vec_rs, vec_rp)
#
#     if a1 < 180:
#         a1 = 360 - a1
#     elif a1 > 360 or a1 < 0:
#         raise ValueError("a1 值非法")
#
#     ans = 1.0000 * a2 / a1 - 0.1000
#
#     timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
#     log_content = (
#         f"==== 运行时间: {timestamp} ====\n"
#         f"起始刻度线中心: ({x_sf}, {y_sf})\n"
#         f"终止刻度线中心: ({x_ef}, {y_ef})\n"
#         f"中间刻度中心: ({x_zf}, {y_zf})\n"
#         f"指针端点: ({x_pf}, {y_pf})\n"
#         f"计算得出的圆心: ({c_x}, {c_y})\n"
#         f"夹角 a1: {a1:.4f}°\n"
#         f"夹角 a2: {a2:.4f}°\n"
#         f"最终仪表读数: {ans:.4f}\n"
#         "----------------------------------------\n"
#     )
#
#     print(log_content)
#
#     if log_file:
#         os.makedirs(os.path.dirname(log_file), exist_ok=True)
#         with open(log_file, "a", encoding="utf-8") as f:
#             f.write(log_content)
#
#     return ans

# ------------------ 封装 final 函数 ------------------

def final(file_path=None):
    """
    入口函数
    对指定文件路径计算仪表读数，并返回日志文件路径。
    如果 file_path 为 None，则使用默认路径 "../output/scale_latest.txt"
    """
    if file_path is None:
        file_path = os.path.join(os.path.dirname(__file__), "output/scale_latest.txt")
    log_file = os.path.join(os.path.dirname(__file__), "output/output_dial_logs.txt")

    boxes = read_scale_latest_file(file_path)
    # ans = compute_gauge(boxes, log_file=log_file)
    # return log_file  # 返回日志文件路径，可用于 GUI 调用
    boxes = read_scale_latest_file(file_path)
    return compute_gauge_new(boxes, log_file=log_file)

# ------------------ 可直接运行 ------------------

# if __name__ == "__main__":
#     log_path = final()
#     print("日志文件路径:", log_path)



    # # 将 box 转换为 NumPy 数组
    # box_np = np.asarray(box, dtype=int)
    #
    # # 计算旋转矩形的中心点坐标
    # center_x = int(np.mean(box_np[:, 0]))  # 所有顶点的 x 坐标平均值
    # center_y = int(np.mean(box_np[:, 1]))  # 所有顶点的 y 坐标平均值
    #
    # # 在预测图中绘制一个中心坐标红点
    # cv2.circle(self.im, (center_x, center_y), self.lw, (0, 0, 255), self.lw)
    #
    # # 创建中心点坐标变量
    # Center = (center_x, center_y)
    #
    # # 在图像上添加中心点坐标文本
    # cv2.putText(self.im, str(Center), (center_x, center_y), 0, self.lw / 3, (0, 0, 255), thickness=4,
    #             lineType=cv2.LINE_AA)
    #
    # # 添加四个顶点的坐标文本
    # for i, (x, y) in enumerate(box_np):
    #     # 调整文本位置，避免重叠
    #     if i == 0:  # 顶点1
    #         text_pos = (x, y - 10)
    #     elif i == 1:  # 顶点2
    #         text_pos = (x, y - 10)
    #     elif i == 2:  # 顶点3
    #         text_pos = (x, y + 20)
    #     elif i == 3:  # 顶点4
    #         text_pos = (x, y + 20)
    #
    #     # 添加顶点坐标文本
    #     cv2.putText(self.im, f"({x}, {y})", text_pos, 0, self.lw / 3, (0, 0, 255), thickness=2, lineType=cv2.LINE_AA)


    # # 在预测图中绘制一个中心坐标红点
    # # center_x = (p1[0] + p2[0]) // 2
    # # center_y = (p1[1] + p2[1]) // 2
    # # cv2.circle(self.im, (center_x, center_y), self.lw, (0, 0, 225), self.lw)
    #
    # # 创建中心点坐标变量
    # # Center = (center_x, center_y)
    #
    # # 用于在图像上添加文本
    # # cv2.putText(self.im, str(Center), (center_x, center_y), 0, self.lw / 3, (0, 0, 225), thickness=4, lineType=cv2.LINE_AA)
    #
    # # 添加左上角、右上角、左下角和右下角的坐标
    # # 左上角
    # cv2.putText(self.im, f"({p1[0]}, {p1[1]})", (p1[0], p1[1] + 20), 0, self.lw / 3, (0, 0, 225), thickness=2,
    #             lineType=cv2.LINE_AA)
    # # 右上角
    # cv2.putText(self.im, f"({p2[0]}, {p1[1]})", (p2[0], p1[1] + 20), 0, self.lw / 3, (0, 0, 225), thickness=2,
    #             lineType=cv2.LINE_AA)
    # # 左下角
    # cv2.putText(self.im, f"({p1[0]}, {p2[1]})", (p1[0] - 20, p2[1] + 20), 0, self.lw / 3, (0, 0, 225), thickness=2,
    #             lineType=cv2.LINE_AA)
    # # 右下角
    # cv2.putText(self.im, f"({p2[0]}, {p2[1]})", (p2[0] - 20, p2[1] + 20), 0, self.lw / 3, (0, 0, 225), thickness=2,
    #             lineType=cv2.LINE_AA)
