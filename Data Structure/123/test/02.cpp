#include<iostream>
using namespace std;

class Stack
{
private:
	//储存栈元素的数组
	int* array;
	//栈的容量
	size_t capacity;
	//栈顶位置
	size_t top;

public:
	//构造函数
	Stack(size_t size = 10) : capacity(size), top(0)
	{
		array = new int[capacity];
	}

	//析构函数
	~Stack() 
	{
		delete[] array;
	}

	//入栈
	void push(int value)
	{
		if (top == capacity)
		{
			//栈满 需扩容
			int* newArray = new int[capacity * 2];
			for (size_t i = 0 ; i < capacity ; ++i) 
			{
				newArray[i] = array[i];
			}
			delete[] array;
			array = newArray;
			capacity *= 2;
		}
		array[top++] = value;
	}

	//出栈
	void pop() {
		if (isEmpty()) 
		{
			cerr << "栈为空，无法执行出栈操作" << endl;
			return;
		}
		--top;
		// 如果栈利用率低于25%，则缩容
		if (top > 0 && top == capacity / 4) 
		{
			int* newArray = new int[capacity / 2];
			for (std::size_t i = 0; i < top; ++i) 
			{
				newArray[i] = array[i];
			}
			delete[] array;
			array = newArray;
			capacity /= 2;
		}
	}

	// 获取栈顶元素
	int& peek() {
		if (isEmpty()) {
			std::cerr << "栈为空" << std::endl;
			throw std::runtime_error("栈为空");
		}
		return array[top - 1];
	}

	// 检查栈是否为空
	bool isEmpty() const {
		return top == 0;
	}

	// 获取栈的大小
	std::size_t size() const {
		return top;
	}
};

int main()
{
	size_t capacity;
	cout << "Enter the initial capacity of the stack: ";
	cin >> capacity;

	Stack myStack(capacity);

	int value;
	cout << "Enter an integer to fill the stack and a non-numeric ending input: ";
	while (cin >> value)
	{
		myStack.push(value);
	}

	cout << "Elements in the stack (output in reverse order): ";
	while (!myStack.isEmpty())
	{
		cout << myStack.peek() << " ";
		myStack.pop();
	}
	cout << endl;

	return 0;
}