
#include<string>
using namespace std;
//ͨѶ¼�������1000����Ϣ
#define max 1000
//��ϵ��
struct Person {
	string name;
	int age;
	char sex;
	string phonenumber;
	string address;

};
//ͨѶ¼�ṹ��
struct Addressbooks
{
	struct Person personArray[max];

	//ͨѶ¼��ǰ��¼��ϵ������
	int m_size;
};
