#include<iostream>
using namespace std;

//定义单链表节点结构体
struct ListNode
{
	int value;
	ListNode* next;
	ListNode(int x) : value(x), next(nullptr) {}
};

//在链表末尾添加节点
void appendNode(ListNode*& head, ListNode*& tail, int value) {
	ListNode* newNode = new ListNode(value); // 声明 newNode
	if (!head)
	{
		head = newNode;
	}
	else
	{
		tail->next = newNode;
	}
	tail = newNode;
}

// 打印链表
void printList(ListNode* head)
{
	while (head)
	{
		cout << head->value << " ";
		head = head->next;
	}
	cout << endl;
}

//合并两个有序链表
ListNode* mergeSortedLists(ListNode* list1, ListNode* list2)
{
	if (!list1) return list2;
	if (!list2) return list1;

	ListNode* head = nullptr, * tail = nullptr;
	if (list1->value < list2->value) {
		head = tail = list1;
		list1 = list1->next;
	}
	else {
		head = tail = list2;
		list2 = list2->next;
	}

	while (list1 && list2) {
		if (list1->value < list2->value) {
			tail->next = list1;
			list1 = list1->next;
		}
		else {
			tail->next = list2;
			list2 = list2->next;
		}
		tail = tail->next;
	}

	if (list1) tail->next = list1;
	if (list2) tail->next = list2;

	return head;
}

int main()
{
	ListNode* list1 = nullptr, * list1Tail = nullptr;
	ListNode* list2 = nullptr, * list2Tail = nullptr;

	cout << "Enter the first sorted list(end with -1): ";
	int value;
	while (cin >> value, value != -1)
	{
		appendNode(list1, list1Tail, value);
	}

	cout << "Enter the second sorted list(end with -1): ";
	while (cin >> value, value != -1)
	{
		appendNode(list2, list2Tail, value);
	}

	// 打印原始链表
	cout << "List 1: ";
	printList(list1);
	cout << "List 2: ";
	printList(list2);

	// 合并链表
	ListNode* mergedList = mergeSortedLists(list1, list2);

	// 打印合并后的链表
	cout << "Merged List: ";
	printList(mergedList);

	// 释放链表内存
	ListNode* current = mergedList;
	while (current) {
		ListNode* toDelete = current;
		current = current->next;
		delete toDelete;
	}

	return 0;
}