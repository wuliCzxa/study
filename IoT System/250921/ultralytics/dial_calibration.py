# -*- coding: utf-8 -*-
import cv2
import numpy as np
import os


def enhance_image(img, alpha=1.2, beta=10, gamma=1.0, clahe_clip=2.0, clahe_grid=8):
    """
    图像增强：对比度、亮度、Gamma校正 + CLAHE
    参数:
        img: np.ndarray, 输入BGR图像
        alpha: 对比度增益 (1.0~3.0)
        beta: 亮度调节 (0~100)
        gamma: Gamma校正 (0.5~2.0)
        clahe_clip: CLAHE 对比度限制
        clahe_grid: CLAHE 网格大小
    返回:
        np.ndarray, 增强后的 BGR 图像
    """
    # ---------- 1. 线性亮度 & 对比度调整 ----------
    adjusted = cv2.convertScaleAbs(img, alpha=alpha, beta=beta)

    # ---------- 2. Gamma 校正 ----------
    inv_gamma = 1.0 / gamma
    table = np.array([(i / 255.0) ** inv_gamma * 255 for i in np.arange(256)]).astype("uint8")
    gamma_corrected = cv2.LUT(adjusted, table)

    # ---------- 3. CLAHE (应用到 LAB 空间的 L 通道) ----------
    lab = cv2.cvtColor(gamma_corrected, cv2.COLOR_BGR2LAB)
    l, a, b = cv2.split(lab)

    clahe = cv2.createCLAHE(clipLimit=clahe_clip, tileGridSize=(clahe_grid, clahe_grid))
    l = clahe.apply(l)

    enhanced = cv2.merge((l, a, b))
    enhanced = cv2.cvtColor(enhanced, cv2.COLOR_LAB2BGR)

    return enhanced


def dial_calibration(input_path="database/new.jpg", output_path="database/dial_perspective_corrected.png", size=400):
    """
    对指针型仪表图像进行表盘裁剪与透视校正 + 图像增强，生成透明背景 PNG。
    """
    # 确保路径存在
    if not os.path.exists(input_path):
        print(f"❌ 输入图片不存在: {input_path}")
        return False

    # 读取图片
    img = cv2.imread(input_path)
    if img is None:
        print(f"❌ 无法读取图片: {input_path}")
        return False

    # 转灰度并模糊，便于检测
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    gray = cv2.medianBlur(gray, 5)

    # ---------- 1. 检测表盘外圆 ----------
    circles = cv2.HoughCircles(
        gray, cv2.HOUGH_GRADIENT, dp=1.2, minDist=100,
        param1=100, param2=30, minRadius=100, maxRadius=0
    )

    if circles is not None:
        circles = np.uint16(np.around(circles))
        x, y, r = circles[0][0]

        # ---------- 2. 半径加大一点 ----------
        r = int(r * 1.08)

        # ---------- 3. 裁剪表盘区域 ----------
        crop = img[y - r:y + r, x - r:x + r]

        # ---------- 4. 缩放为目标正方形 ----------
        corrected = cv2.resize(crop, (size, size))

        # ---------- 5. 图像增强 ----------
        enhanced = enhance_image(corrected, alpha=1.3, beta=15, gamma=1.1)

        # ---------- 6. 创建透明背景 ----------
        result = np.zeros((size, size, 4), dtype="uint8")

        # ---------- 7. 生成圆形 alpha 掩膜 ----------
        mask = np.zeros((size, size), dtype="uint8")
        cv2.circle(mask, (size // 2, size // 2), size // 2 - 2, 255, -1)

        # 保留 RGB，添加 Alpha
        result[:, :, 0:3] = enhanced
        result[:, :, 3] = mask  # 透明通道

        # ---------- 8. 保存透明 PNG ----------
        cv2.imwrite(output_path, result)
        print(f"✅ 已保存增强后的图片: {output_path}")
        return True
    else:
        print("❌ 未检测到表盘圆，请调整 HoughCircles 参数")
        return False


# ---------- 测试 ----------
if __name__ == "__main__":
    dial_calibration("database/new.jpg", "database/dial_perspective_corrected.png")
