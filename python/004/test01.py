def calculate_power(n, m):
    """
    计算 i 的 m 次方，其中 i 从 1 到 n
    """
    return [i ** m for i in range(1, n + 1)]

def main():
    # 定义参数
    n1 = 100
    n2 = 50
    n3 = 10

    # 计算累加和
    sum1 = sum(k1 for k1 in range(1, n1 + 1))
    sum2 = sum(k2 ** 2 for k2 in range(1, n2 + 1))
    sum3 = sum(1/k3 for k3 in range(1, n3 + 1))

    # 计算总和
    total_sum = sum1 + sum2 + sum3

    # 打印结果
    print(f"{total_sum}")

if __name__ == "__main__":
    main()
