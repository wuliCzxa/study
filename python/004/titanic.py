import pandas as pd
import matplotlib.pyplot as plt

# 读取Titanic数据集
titanic = pd.read_csv('experiment004/titanic.csv')

# 调整pandas显示设置，确保显示所有列
pd.set_option('display.max_columns', None)  # 显示所有列
pd.set_option('display.width', 1000)  # 设置显示宽度
pd.set_option('display.max_colwidth', None)  # 设置每列的最大宽度

# 显示前50行数据
print("前50行的数据信息：")
print(titanic.head(50))

# 定义一个函数显示数据信息总览
def data_overview(df):
    print("\n数据信息总览：")
    print(df.info())
    print("\n数据的描述性统计：")
    print(df.describe(include='all'))

# 调用函数显示数据信息总览
data_overview(titanic)

# 用均值填充年龄的缺失值
titanic['age'].fillna(titanic['age'].mean(), inplace=True)

# 绘制生还者的饼图
survived_counts = titanic['survived'].value_counts()
plt.figure(figsize=(8, 6))
plt.pie(survived_counts, labels=['Not Survived', 'Survived'], autopct='%1.1f%%', startangle=140, colors=['lightcoral', 'lightskyblue'])
plt.title('Survival Rate of Titanic Passengers')
plt.axis('equal')  # 确保饼图为圆形
plt.show()
