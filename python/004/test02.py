import math


# 定义求圆面积的函数
def area(r):
    return round(math.pi * r ** 2, 2)


# 计算并打印半径为3.5的圆的面积
print(f"r=3.5  area={area(3.5)}")

# 计算并打印半径为2.9的圆的面积
print(f"r=2.9  area={area(2.9)}")

# 计算并打印外圆半径为6.2、内圆半径为3.3的圆环的面积
# 圆环面积 = 外圆面积 - 内圆面积
ring_area = area(6.2) - area(3.3)
ring_area = round(ring_area, 2)
print(f"r1=6.2  r2=3.3  area={ring_area}")
