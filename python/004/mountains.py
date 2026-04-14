import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re

# 读取Mountains数据集，尝试使用不同的编码
try:
    mountains = pd.read_csv('experiment004/mountains.csv', encoding='utf-8')
except UnicodeDecodeError:
    mountains = pd.read_csv('experiment004/mountains.csv', encoding='latin1')

# 打印列名以确认其正确性
print("列名：", mountains.columns)

# 调整pandas显示设置，确保显示所有列
pd.set_option('display.max_columns', None)
pd.set_option('display.width', 1000)
pd.set_option('display.max_colwidth', None)

# 设置图片显示的主题样式
sns.set_theme(style="whitegrid")

# 设置中文字体为SimHei
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


# 清洗数据集中的非数字值
def clean_numeric_values(series):
    # 移除所有非数字字符
    series = series.apply(lambda x: re.sub(r'\D', '', str(x)))
    # 将清理后的值转换为整数
    series = series.apply(lambda x: int(x) if x.isdigit() else 0)
    return series


# 重构和清洗数据集
def clean_and_restructure_data(df):
    df = df.rename(columns={
        'Height (m)': 'Height',
        'Ascents bef. 2004': 'Success',
        'Failed attempts bef. 2004': 'Failed'
    })
    df['Success'] = clean_numeric_values(df['Success'])
    df['Failed'] = clean_numeric_values(df['Failed'])
    return df


# 检查重命名是否成功
mountains = clean_and_restructure_data(mountains)
print("重命名后的列名：", mountains.columns)


# 可视化功能
def visualize_data(df):
    plt.figure(figsize=(14, 10))

    # 图1: 山峰的数量和海拔
    plt.subplot(2, 2, 1)
    sns.histplot(df['Height'], bins=30, kde=True)
    plt.title('山峰的数量和海拔')
    plt.xlabel('海拔 (m)')
    plt.ylabel('数量')

    # 图2: 山峰的数量和年份（假设有Year列）
    if 'Year' in df.columns:
        plt.subplot(2, 2, 2)
        sns.histplot(df['Year'], bins=30, kde=True)
        plt.title('山峰的数量和年份')
        plt.xlabel('年份')
        plt.ylabel('数量')

    # 图3: 海拔与登顶
    plt.subplot(2, 2, 3)
    sns.scatterplot(x='Height', y='Success', data=df)
    plt.title('海拔与登顶')
    plt.xlabel('海拔 (m)')
    plt.ylabel('成功次数')

    # 图4: 登顶与排名（假设有Rank列）
    if 'Rank' in df.columns:
        plt.subplot(2, 2, 4)
        sns.scatterplot(x='Success', y='Rank', data=df)
        plt.title('登顶与排名')
        plt.xlabel('成功次数')
        plt.ylabel('排名')

    plt.tight_layout(pad=4.0, w_pad=5.0, h_pad=5.0)
    plt.show()


visualize_data(mountains)
