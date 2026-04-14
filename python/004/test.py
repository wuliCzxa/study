def factorial(n):
    """
    递归函数计算整数n的阶乘
    """
    if n == 0 or n == 1:
        return 1
    else:
        return n * factorial(n - 1)

def main():
    # 从键盘输入一个整数
    n = int(input())

    # 计算输入整数的阶乘并输出结果
    result = factorial(n)
    print(f"{result}")

if __name__ == "__main__":
    main()


