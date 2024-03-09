#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

const int NAME_LEN = 20;
enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT };//���α׷�������� ���� �޴�
enum {LEVEL_A=7, LEVEL_B=4, LEVEL_C=2};//�ſ���
enum {NORMAL=1, CREDIT=2};//����������

class Account//Entity Ŭ����
{
private:
	int accID;
	int balance;
	char* cusName;
public:
	Account(int ID, int money, const char* name)
		:accID(ID), balance(money)
	{
		cusName = new char[strlen(name) + 1];
		strcpy(cusName, name);
	}

	Account(const Account& ref)
		: accID(ref.accID), balance(ref.balance)
	{
		cusName = new char[strlen(ref.cusName) + 1];
		strcpy(cusName, ref.cusName);
	}

	int GetAccID() const
	{
		return accID;
	}

	virtual void Deposit(int money)
	{
		balance += money;
	}

	int withdraw(int money)
	{
		if (balance < money)
			return 0;
		balance -= money;
		return money;
	}

	void ShowAccInfo() const
	{
		cout << "����ID: " << accID << endl;
		cout << "�̸�: " << cusName << endl;
		cout << "�ܾ�: " << balance << endl;
	}

	~Account()
	{
		delete[]cusName;
	}
};

class NormalAccount:public Account
{
private:
	int izayulper;
public:
	NormalAccount(int ID, int money, const char* name,int izayul)
		:Account(ID, money, name), izayulper(izayul)
	{ }

	virtual void Deposit(int money)
	{
		Account::Deposit(money);//�����߰�
		Account::Deposit(money * (izayulper / 100.0));//�����߰�
	}
};

class HighCreditAccount :public NormalAccount
{
private:
	int specialrate;
public:
	HighCreditAccount(int ID, int money, const char* name, int izayul, int rate)
		:NormalAccount(ID, money, name, izayul), specialrate(rate)
	{ }
	void Deposit(int money)
	{
		NormalAccount::Deposit(money);//���ݰ� �����߰�
		Account::Deposit(money * (specialrate / 100.0));//Ư�������߰�
	}
};

class AccountHandler//��Ʈ�� Ŭ���� 
{
private:
	Account* accArr[100];
	int accNum;
public:
	AccountHandler() :accNum(0)
	{ }

	void ShowMenu() const
	{
		cout << "-----Menu-----" << endl;
		cout << "1. ���°���" << endl;
		cout << "2. �Ա�" << endl;
		cout << "3. ���" << endl;
		cout << "4. �������� ��ü ���" << endl;
		cout << "5. ���α׷� ����" << endl;
	}

	void MakeAccount()
	{
		int a;

		cout << "[������������]" << endl;
		cout << "1.���뿹�ݰ��� 2.�ſ�ŷڰ���" << endl;
		cout << "����: "; cin >> a;
		if (a == NORMAL)
		{
			MakeNormalAccount();
		}
		else if (a == CREDIT)
		{
			MakeCreditAccount();
		}
		else
		{
			cout << "�߸��� ���� �Է��߽��ϴ�." << endl;
			return;
		}
	}

	void MakeNormalAccount()
	{
		int id;
		char name[NAME_LEN];
		int balance;
		int izayul;

		cout << "[���뿹�ݰ��� ����]" << endl;
		cout << "����ID: "; cin >> id;
		cout << "�� ��: "; cin >> name;
		cout << "�Աݾ�: "; cin >> balance;
		cout << "������: "; cin >> izayul;
		cout << endl;

		accArr[accNum++] = new NormalAccount(id, balance, name, izayul);
	}

	void MakeCreditAccount()
	{
		int id;
		char name[NAME_LEN];
		int balance;
		int izayul;
		int rate;

		cout << "[�ſ�ŷڰ��� ����]" << endl;
		cout << "����ID: "; cin >> id;
		cout << "�� ��: "; cin >> name;
		cout << "�Աݾ�: "; cin >> balance;
		cout << "������: "; cin >> izayul;
		cout << "�ſ���(1toA, 2toB, 3toC): "; cin >> rate;
		cout << endl;

		switch (rate)
		{
		case 1:
			accArr[accNum++] = new HighCreditAccount(id, balance, name, izayul, LEVEL_A);
			break;
		case 2:
			accArr[accNum++] = new HighCreditAccount(id, balance, name, izayul, LEVEL_B);
			break;
		case 3:
			accArr[accNum++] = new HighCreditAccount(id, balance, name, izayul, LEVEL_C);
		}
		
	}

	void DepositMoney()
	{
		int money;
		int id;
		cout << "[�Ա�]" << endl;
		cout << "����ID: "; cin >> id;
		cout << "�Աݾ�: "; cin >> money;

		for (int i = 0; i < accNum; i++)
		{
			if (accArr[i]->GetAccID() == id)
			{
				accArr[i]->Deposit(money);
				cout << "�ԱݿϷ�" << endl << endl;
				return;
			}
			cout << "��ȿ���� ���� ID�Դϴ�." << endl << endl;
		}
	}

	void withdrawMoney()
	{
		int money;
		int id;
		cout << "[���]" << endl;
		cout << "����ID: "; cin >> id;
		cout << "��ݾ�: "; cin >> money;

		for (int i = 0; i < accNum; i++)
		{
			if (accArr[i]->GetAccID() == id)
			{
				if (accArr[i]->withdraw(money) == 0)
				{
					cout << "�ܾ׺���" << endl << endl;
					return;
				}
				cout << "��ݿϷ�" << endl << endl;
				return;
			}
		}
		cout << "��ȿ���� ���� ID�Դϴ�." << endl << endl;
	}

	void ShowAllAccInfo() const
	{
		for (int i = 0; i < accNum; i++)
		{
			accArr[i]->ShowAccInfo();
			cout << endl;
		}
	}

	~AccountHandler()
	{
		for (int i = 0; i < accNum; i++)
		{
			delete accArr[i];
		}
	}
};

int main(void)
{
	AccountHandler manager;
	int choice;

	while (1)
	{
		manager.ShowMenu();
		cout << "����: ";
		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case MAKE:
			manager.MakeAccount();
			break;
		case DEPOSIT:
			manager.DepositMoney();
			break;
		case WITHDRAW:
			manager.withdrawMoney();
			break;
		case INQUIRE:
			manager.ShowAllAccInfo();
			break;
		case EXIT:
			return 0;
		default:
			cout << "Illegal selection.." << endl;
		}
	}
	return 0;
}