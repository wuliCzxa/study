#include <iostream>

class Stack {
private:
    int* array; // 存储栈元素的数组
    std::size_t capacity; // 栈的容量
    std::size_t top; // 栈顶位置

public:
    // 构造函数
    Stack(std::size_t size = 10) : capacity(size), top(0) {
        array = new int[capacity];
    }

    // 析构函数
    ~Stack() {
        delete[] array;
    }

    // 入栈操作
    void push(int value) {
        if (top == capacity) {
            // 栈满，需要扩容
            int* newArray = new int[capacity * 2];
            for (std::size_t i = 0; i < capacity; ++i) {
                newArray[i] = array[i];
            }
            delete[] array;
            array = newArray;
            capacity *= 2;
            std::cout << "栈扩容后容积: " << capacity << std::endl;
        }
        array[top++] = value;
    }

    // 出栈操作
    void pop() {
        if (isEmpty()) {
            std::cerr << "栈为空，无法执行出栈操作" << std::endl;
            return;
        }
        --top;
        // 如果栈利用率低于25%，则缩容
        if (top > 0 && top == capacity / 4) {
            int* newArray = new int[capacity / 2];
            for (std::size_t i = 0; i < top; ++i) {
                newArray[i] = array[i];
            }
            delete[] array;
            array = newArray;
            capacity /= 2;
            std::cout << "栈缩容后容积: " << capacity << std::endl;
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

int main() {
    Stack myStack;

    int value;
    std::cout << "请输入整数以填充栈，输入非数字结束输入:" << std::endl;
    while (std::cin >> value) {
        myStack.push(value);
    }

    std::cout << "栈中的元素（逆序输出）:" << std::endl;
    while (!myStack.isEmpty()) {
        std::cout << myStack.peek() << " ";
        myStack.pop();
    }
    std::cout << std::endl;

    return 0;
}