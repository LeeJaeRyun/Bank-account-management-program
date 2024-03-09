#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

const int NAME_LEN = 20;
enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT };//프로그램사용자의 선택 메뉴
enum {LEVEL_A=7, LEVEL_B=4, LEVEL_C=2};//신용등급
enum {NORMAL=1, CREDIT=2};//계좌의종류

class Account//Entity 클래스
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
		cout << "계좌ID: " << accID << endl;
		cout << "이름: " << cusName << endl;
		cout << "잔액: " << balance << endl;
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
		Account::Deposit(money);//원금추가
		Account::Deposit(money * (izayulper / 100.0));//이자추가
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
		NormalAccount::Deposit(money);//원금과 이자추가
		Account::Deposit(money * (specialrate / 100.0));//특별이자추가
	}
};

class AccountHandler//컨트롤 클래스 
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
		cout << "1. 계좌개설" << endl;
		cout << "2. 입금" << endl;
		cout << "3. 출금" << endl;
		cout << "4. 계좌정보 전체 출력" << endl;
		cout << "5. 프로그램 종료" << endl;
	}

	void MakeAccount()
	{
		int a;

		cout << "[계좌종류선택]" << endl;
		cout << "1.보통예금계좌 2.신용신뢰계좌" << endl;
		cout << "선택: "; cin >> a;
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
			cout << "잘못된 값을 입력했습니다." << endl;
			return;
		}
	}

	void MakeNormalAccount()
	{
		int id;
		char name[NAME_LEN];
		int balance;
		int izayul;

		cout << "[보통예금계좌 개설]" << endl;
		cout << "계좌ID: "; cin >> id;
		cout << "이 름: "; cin >> name;
		cout << "입금액: "; cin >> balance;
		cout << "이자율: "; cin >> izayul;
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

		cout << "[신용신뢰계좌 개설]" << endl;
		cout << "계좌ID: "; cin >> id;
		cout << "이 름: "; cin >> name;
		cout << "입금액: "; cin >> balance;
		cout << "이자율: "; cin >> izayul;
		cout << "신용등급(1toA, 2toB, 3toC): "; cin >> rate;
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
		cout << "[입금]" << endl;
		cout << "계좌ID: "; cin >> id;
		cout << "입금액: "; cin >> money;

		for (int i = 0; i < accNum; i++)
		{
			if (accArr[i]->GetAccID() == id)
			{
				accArr[i]->Deposit(money);
				cout << "입금완료" << endl << endl;
				return;
			}
			cout << "유효하지 않은 ID입니다." << endl << endl;
		}
	}

	void withdrawMoney()
	{
		int money;
		int id;
		cout << "[출금]" << endl;
		cout << "계좌ID: "; cin >> id;
		cout << "출금액: "; cin >> money;

		for (int i = 0; i < accNum; i++)
		{
			if (accArr[i]->GetAccID() == id)
			{
				if (accArr[i]->withdraw(money) == 0)
				{
					cout << "잔액부족" << endl << endl;
					return;
				}
				cout << "출금완료" << endl << endl;
				return;
			}
		}
		cout << "유효하지 않은 ID입니다." << endl << endl;
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
		cout << "선택: ";
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