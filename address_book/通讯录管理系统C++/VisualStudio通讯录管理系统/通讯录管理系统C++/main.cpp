#include<iostream>
#include"contacts.h"
using namespace std;
void showManu(); // ��ʾ�˵�
void addperson(Addressbooks* abs); // �����ϵ�˵�ͨѶ¼
void showcontact(Addressbooks* abs); // ��ӡͨѶ¼��������ϵ��
int isExist(Addressbooks* abs, string name); // ���ͨѶ¼���Ƿ��и�name����ϵ��
void deleteperson(Addressbooks* abs, string name); // ɾ��ָ����ϵ��
void findperson(Addressbooks* abs); // ����ָ����ϵ��
void fixperson(Addressbooks* abs); // �޸�ָ����ϵ��
void cleancontacts(Addressbooks* abs); // ���ͨѶ¼

//��ʾ�˵�
void showManu() {
	cout << "********************************" << endl;
	cout << " *****  1.�����ϵ��   ********" << endl;
	cout << " *****  2.��ʾ��ϵ��   ********" << endl;
	cout << " *****  3.ɾ����ϵ��   ********" << endl;
	cout << " *****  4.������ϵ��   ********" << endl;
	cout << " *****  5.�޸���ϵ��   ********" << endl;
	cout << " *****  6.�����ϵ��   ********" << endl;
	cout << " *****  0.�˳�ͨѶ¼   ********" << endl;
	cout << "********************************" << endl;
	cout << endl;
}

//�����ϵ�˵�ͨѶ¼
void addperson(Addressbooks* abs) {
	//�ж�ͨѶ¼�Ƿ�Ϊ��
	if (abs->m_size == max) {
		cout << "ͨѶ¼����" << endl;
		return;
	}
	
	else {
		//�����ϵ����Ϣ

		//����
		string name;
		cout << "����������" << endl;
		cin >> name;
		if (isExist(abs, name)>-1) {
			cout << "����ϵ���Ѵ���!" << endl;
			
		}
		else{
			abs->personArray[abs->m_size].name = name;
			//����
			cout << "����������" << endl;
			unsigned int age = 0;
			cin >> age;
			abs->personArray[abs->m_size].age = age;

			//�Ա�
			cout << "�������Ա�" << endl;
			cout << "1--��" << endl;
			cout << "0--Ů" << endl;
			char sex;
			while (1) {
				cin >> sex;
				if (sex == '1' || sex == '0') {
					abs->personArray[abs->m_size].sex = sex;
					break;
				}
				else
					cout << "������������������" << endl;
			}
			//�绰
			cout << "������绰" << endl;
			string phonenumber = "0";
			cin >> phonenumber;
			abs->personArray[abs->m_size].phonenumber = phonenumber;
			// ��ַ
			cout << "�������ַ" << endl;
			string address = "0";
			cin >> address;
			abs->personArray[abs->m_size].address = address;
			//ͨѶ¼����+1
			abs->m_size++;
			cout << endl;
			cout << "��ϵ�ˡ�" << name << "����Ϣ��ӳɹ���" << endl;
			cout << endl;
		}
		system("pause");//���������
		//system("cls");//����

	}
}

//��ӡͨѶ¼��������ϵ��
void showcontact(Addressbooks* abs) {
	if (abs->m_size == 0) {//�ж���ϵ�˼�¼�Ƿ�Ϊ��
		cout << "��ǰͨѶ¼����ϵ�ˣ�" << endl;
	}
	else {
		for (int i = 0; i < abs->m_size; i++) {
			cout << "��" << i + 1 << "λ��ϵ����Ϣ��" << endl;
			cout << "������" << abs->personArray[i].name << "\t";
			cout << "�Ա�" << (abs->personArray[i].sex == '1' ? "��" : "Ů") << "\t";
			cout << "���䣺" << abs->personArray[i].age << "\t";
			cout << "�绰��" << abs->personArray[i].phonenumber << "\t";
			cout << "��ַ��" << abs->personArray[i].address << endl;
			cout << endl;
		}
		system("pause");
	}
}

//���ͨѶ¼���Ƿ��и�name����ϵ�ˣ�����У����ظ���ϵ��λ�ã����û�У�����-1
int isExist(Addressbooks* abs, string name) {
	for (int i = 0; i < abs->m_size; i++) {
		if (abs->personArray[i].name == name) {
			return i;
		}
	}
	return -1;
}

//���ͨѶ¼���Ƿ��и�name����ϵ�ˣ�����У�ɾ������ϵ����Ϣ����������ϵ��ǰ�ƶ���m_size--�����û�У����ز�����
void deleteperson(Addressbooks* abs, string name) {
	int ret = isExist(abs, name);
	if (ret == -1) {
		cout << "���޴��ˣ�" << endl;

	}
	else {
		for (int i = ret; i < abs->m_size; i++) {
			//������ϵ����Ϣ�ռ��ͷŲ���������ϵ��ǰ�ƶ�
			abs->personArray[i] = abs->personArray[i + 1];
		}
		abs->m_size--;
		cout << "�ҵ���ϵ��:" << name << endl;
		cout << "��ɾ����ϵ�ˣ�" << name << endl;
	}

}

//����ָ����ϵ�ˣ�����У����ظ���ϵ��������Ϣ�����û�У�����-1
void findperson(Addressbooks* abs) {
	cout << "������Ҫ������ϵ�˵�������" << endl;
	string name;
	cin >> name;
	int ret = isExist(abs, name);
	if (ret == -1) {
		cout << "���޴��ˣ�" << endl;

	}
	else {
		cout << "������" << abs->personArray[ret].name << "\t";
		cout << "�Ա�" << (abs->personArray[ret].sex == '1' ? "��" : "Ů") << "\t";
		cout << "���䣺" << abs->personArray[ret].age << "\t";
		cout << "�绰��" << abs->personArray[ret].phonenumber << "\t";
		cout << "��ַ��" << abs->personArray[ret].address << endl;
		cout << endl;
	}
}

//�޸�ָ����ϵ�ˣ�����У������޸ĸ���ϵ��������Ϣ�����û�У����ز��޴���
void fixperson(Addressbooks* abs) {
	cout << "������Ҫ�޸���ϵ�˵�������" << endl;
	string name;
	cin >> name;
	int ret = isExist(abs, name);
	if (ret == -1) {
		cout << "���޴��ˣ�" << endl;
	}

	else {
		//�����ϵ����Ϣ
		//����
		string name;
		cout << "����������" << endl;
		cin >> name;
		abs->personArray[ret].name = name;
		//����
		cout << "����������" << endl;
		unsigned int age = 0;
		cin >> age;
		abs->personArray[ret].age = age;

		//�Ա�
		cout << "�������Ա�" << endl;
		cout << "1--��" << endl;
		cout << "0--Ů" << endl;
		char sex = 0;
		while (1) {
			cin >> sex;
			if (sex == '1' || sex == '0') {
				abs->personArray[ret].sex = sex;
				break;
			}
			else
				cout << "������������������" << endl;
		}
		//�绰
		cout << "������绰" << endl;
		string phonenumber = "0";
		cin >> phonenumber;
		abs->personArray[ret].phonenumber = phonenumber;
		// ��ַ
		cout << "�������ַ" << endl;
		string address = "0";
		cin >> address;
		abs->personArray[ret].address = address;
		cout << endl;
		cout << "��ϵ�ˡ�" << name << "����Ϣ�޸ĳɹ���" << endl;
		cout << endl;

		cout << "������" << abs->personArray[ret].name << "\t";
		cout << "�Ա�" << (abs->personArray[ret].sex == 1 ? "��" : "Ů") << "\t";
		cout << "���䣺" << abs->personArray[ret].age << "\t";
		cout << "�绰��" << abs->personArray[ret].phonenumber << "\t";
		cout << "��ַ��" << abs->personArray[ret].address << endl;
		cout << endl;
		system("pause");
	}
}

//���ͨѶ¼
void cleancontacts(Addressbooks* abs) {
	abs->m_size = 0;
	cout << "�����ͨѶ¼��" << endl;
}

int main() {

	Addressbooks abs;
	abs.m_size = 0;
	int chioce = 0;
	showManu();

	while (1) {
		cin >> chioce;
		switch (chioce) {
		case 1://�����ϵ��
			addperson(&abs);
			showManu();
			break;
		case 2://��ʾ��ϵ��
			showcontact(&abs);
			break;
		case 3: {//ɾ����ϵ��
			string name;
			cout << "������Ҫɾ����ϵ�˵�������" << endl;
			cin >> name;
			deleteperson(&abs, name);
		}
			  break;
		case 4: {//������ϵ��
			findperson(&abs);
			break;
		}
		case 5: {//�޸���ϵ��
			fixperson(&abs);
		}
			  break;
		case 6: {//���ͨѶ¼
			cout << "�Ƿ����������ϵ�ˣ�" << endl;
			cout << "�ǣ�1��" << "\t";
			cout << "��0��" << "\t";
			char X;
			cout;
			cin >> X;
			if (X == '1')
				cleancontacts(&abs);
			else
				cout << "�������������ϵ�ˣ�" << endl;
			break;
		}
		case 0://�˳�ͨѶ¼
			cout << "��ӭ�´�ʹ��" << endl;
			system("pause");
			return 0;
			break;
		default:
			break;
		}
	}
	return 0;
}