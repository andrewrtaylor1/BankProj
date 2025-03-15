
#include "BankDB.h"
#include "BankServer.h"

using namespace Serv;

DB::Database db;

bool Server::userValidation(std::string user, std::string pass)
{
	bool b = false;

	return b;
}
bool Server::userCreation(std::string user, std::string pass, std::string acc, double deposit)
{
	bool b = false;

	return b;
}
bool Server::accountCreation(std::string user, std::string acc, double deposit)
{
	bool b = false;

	return b;
}
bool Server::accountsTransfer(std::string user, std::string acc, std::string acc2)
{
	bool b = false;

	return b;
}

LinkedList<std::string> Server::accountsCount(std::string user)
{
	int i = db.Customers.find(user);
	if (i != -1)
	{
		return db.Customers.get(i)->AccountIDs;
	}
	else
	{
		int j = db.Employees.find(user);
		if (j != -1)
		{
			LinkedList <std::string> ll;
			for (int i = 1; i <= db.Accounts.getCount(); i++)
			{
				ll.put(std::shared_ptr<std::string>(new std::string(db.Accounts.get(i)->ID)));
			}

			return ll;
		}
	}
	return LinkedList<std::string>();
}
std::string Server::accountDisplay(std::string user, std::string acc)
{
	int i = db.Customers.find(user); //get user 
	if (i > 0)
	{
		std::shared_ptr<DB::Customer> c = db.Customers.get(i);
		if (c && c->AccountIDs.find(acc) >= 0)
		{
			int z = db.Accounts.find(acc);
			std::shared_ptr<DB::Account> a = db.Accounts.get(z);
			if (a) return a->preview();
		}
	}
	else
	{
		int j = db.Employees.find(user);
		if (j > 0)
		{
			int k = db.Accounts.find(acc);
			if (k > 0) return db.Accounts.get(k)->preview();
		}
	}

	return "";
}
std::string Server::accountTransactions(std::string user, std::string acc)
{
	int i = db.Customers.find(user); //get user 
	if (i > 0)
	{
		std::shared_ptr<DB::Customer> c = db.Customers.get(i);
		if (c && c->AccountIDs.find(acc) >= 0)
		{
			int z = db.Accounts.find(acc);
			std::shared_ptr<DB::Account> a = db.Accounts.get(z);
			if (a) return a->transactionHistory();
		}
	}
	else
	{
		int j = db.Employees.find(user);
		if (j > 0)
		{
			int k = db.Accounts.find(acc);
			if (k > 0) return db.Accounts.get(k)->transactionHistory();
		}
	}

	return "";
}