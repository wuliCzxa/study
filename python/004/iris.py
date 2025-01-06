import pandas as pd
import numpy as np

# 读取Iris数据集
iris = pd.read_csv('experiment004/iris.csv')

# 选择萼片(sepal)和花瓣(petal)的长度
sepal_length = iris['sepal_length']
petal_length = iris['petal_length']

# 计算所需的统计信息
def calculate_statistics(data):
    stats = {
        'sum': np.sum(data),
        'cumulative_sum': np.cumsum(data).tolist(),  # 累计和返回列表
        'mean': np.mean(data),
        'std': np.std(data),
        'variance': np.var(data),
        'max': np.max(data),
        'min': np.min(data)
    }
    return stats

# 计算萼片长度的统计信息
sepal_length_stats = calculate_statistics(sepal_length)
# 计算花瓣长度的统计信息
petal_length_stats = calculate_statistics(petal_length)

# 输出统计信息
def print_statistics(name, stats):
    print(f"{name} Statistics:")
    print(f"Sum: {stats['sum']}")
    print(f"Cumulative Sum: {stats['cumulative_sum']}")
    print(f"Mean: {stats['mean']}")
    print(f"Standard Deviation: {stats['std']}")
    print(f"Variance: {stats['variance']}")
    print(f"Max: {stats['max']}")
    print(f"Min: {stats['min']}")
    print()

# 输出萼片长度的统计信息
print_statistics("Sepal Length", sepal_length_stats)
# 输出花瓣长度的统计信息
print_statistics("Petal Length", petal_length_stats)
