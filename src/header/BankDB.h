#pragma once
#include "List.h"
#include <chrono>
#include <string>

namespace DB
{
	
	//currency base class
	class Currency
	{
		public:
			Currency(int i)
			{
				value = i;
			} 
			virtual ~Currency(){}
			virtual const std::string getName() = 0; //currency Name
			virtual const std::string getSymbol() = 0;//currency symbol, like $
			virtual const std::string formattedValue() = 0; //formatted currency, like $0.00

		protected:
			int value; //value is stored as int to innately handle 

	};
	//individual transactions
	class Transaction
	{
		public:
			Transaction(Currency* c)
			{
				value = c;
			}
			virtual ~Transaction(){}
			const std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now(); //time, to resolve conflicts + sorting
			Currency* value; //the actual value of the transaction
			std::string name = "Transaction"; //default transaction name is Transaction; should be changed 

	};
	//Bank Accounts (savings, checking) base class
	class Account
	{
		public:
			Account(Transaction t){}
			virtual ~Account(){}
			LinkedList transactions;

			virtual void SpecialFunctions() = 0; //will run special functions in derived classes
	};

	//base class, takes a string
	class User
	{
	public:
		User(std::string s, std::string pass)
		{
			name = s;
			password = pass;
		}
		virtual ~User() {}
		std::string name;
		std::string password;
	};

	class SpecialFunction
	{
		public:
			SpecialFunction() {}
			~SpecialFunction() {}
	};

	class Database
	{
		public:
			Database() {}
			~Database() {}
			LinkedList users;
			LinkedList accounts;
			LinkedList encryptKeys;
	};

	//savings account
	class Saving : public Account
	{
		public:
			Saving(Transaction t) : Account(t) {}
			~Saving() {}
			void SpecialFunctions();
	};
	//Checking account
	class Checking : public Account
	{
		public:
			Checking(Transaction t) : Account(t) {}
			~Checking() {}
			void SpecialFunctions();
	};
	//purchases (like debit)
	class Purchase : public Transaction
	{
		Purchase(Currency* c) : Transaction(c) {}
		~Purchase() {}
	};
	//transfer between accounts transaction
	class Transfer : public Transaction
	{
		Transfer(Currency* c) : Transaction(c) {}
		~Transfer() {}
	};
	//transaction for deposits
	class Deposit : public Transaction
	{
		Deposit(Currency* c) : Transaction(c) {}
		~Deposit() {}
	};
	//transaction for overdraft/interest
	class BankFunction : public Transaction
	{
		BankFunction(Currency* c) : Transaction(c) {}
		~BankFunction() {}
	};
	//customer; has accounts associated via IDs in Linked List (primary checking as first id)
	class Customer : public User
	{
		public:
			Customer(std::string s, std::string pass, int i) : User(s,pass) {}
			~Customer() {
				
			}
			LinkedList accountIDs;
	};
	class Employee : public User
	{
		public:
			Employee(std::string s, std::string pass) : User(s,pass) {}
			~Employee() {}
	};

	class Overdraft : public SpecialFunction
	{
		public:
			Overdraft() {}
			~Overdraft() {}
	};
	class Interest : public SpecialFunction
	{
		public:
			Interest() {}
			~Interest() {}
	};
	class Suspicion : public SpecialFunction
	{
		public:
			Suspicion() {}
			~Suspicion() {}
	};

	class USDollar : public Currency
	{
		USDollar(int i) :Currency(i) {}
		~USDollar() {}
		//returns the name of the currency. Could be used to ID currency types
		const std::string getName()
		{
			return "US Dollar";
		}
		//gets the symbol; might not need this but could be useful later
		const std::string getSymbol()
		{
			return "$";
		}
		//returns the value with the proper decimal position & dollar sign, as a string
		const std::string formattedValue()
		{
			std::string s = std::to_string(value);
			if (s.front() == '-')
			{
				s.erase(0, 1);
				if (s.length() < 3) s = "00" + s;
				s = "$" + s;
				s.insert(s.length() - 2, ".");
				s = "-" + s;
			}
			else
			{
				if (s.length() < 3) s = "00" + s;
				s = "$" + s;
				s.insert(s.length() - 2, ".");
			}
			return s;
		}
		//allows addition; subtraction not needed, just use negative ints instead
		USDollar operator+(const USDollar&)
		{
		}
		//multiplication; will be used to get interest (IE: Dollar*0.01)
		USDollar operator*(const USDollar&)
		{
		}
		//comparison operators
		USDollar operator==(const USDollar&)
		{
		}
		USDollar operator>(const USDollar&)
		{
		}
		USDollar operator<(const USDollar&)
		{
			
		}
	};
}