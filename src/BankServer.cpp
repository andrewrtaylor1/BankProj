
#include "BankDB.h"
#include "BankServer.h"

using namespace Serv;

//pointer to a new Database
std::shared_ptr<DB::Database> db = std::shared_ptr<DB::Database>(new DB::Database);

/// <summary>
/// validates user & gives their access level
/// </summary>
/// <param name="user">username</param>
/// <param name="pass">password</param>
/// <returns>int code for validation; -1 is default, 0-1 are access levels (user,employee)</returns>
int Server::userValidation(std::string user, std::string pass)
{
	int result = -1;
	int i = db->Customers.find(user);
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i); //grab customer
		if (c)
		{
			if(c->password == pass) result = 0; //will be replaced with password hash comparison later
		}
	}
	else
	{
		int j = db->Employees.find(user);
		if (j != -1)
		{
			std::shared_ptr<DB::Employee> e = db->Employees.get(j);
			if (e)
			{
				if(e->password == pass) result = 1; //will be replaced with password hash comparison later
			}
		}
	}
	return result;
}

/// <summary>
/// creating a user
/// </summary>
/// <param name="user">username</param>
/// <param name="pass">pass</param>
/// <param name="acc">first account name</param>
/// <param name="deposit">initial deposit</param>
/// <returns>was it successful, bool</returns>
bool Server::userCreation(std::string user, std::string pass, std::string acc, double deposit)
{
	bool b = false;
	if (db->Customers.find(user) == -1 && db->Accounts.find(acc) == -1) //make sure user & acc don't already exist
	{
		std::shared_ptr<DB::Customer> u = std::shared_ptr<DB::Customer>(new DB::Customer(user, pass));
		std::shared_ptr<DB::Account> a = std::shared_ptr<DB::Account>(new DB::Saving(std::shared_ptr<DB::Transaction>(new DB::Deposit(deposit)), acc));
		std::shared_ptr<std::string> s = std::shared_ptr<std::string>(new std::string(acc));
		b = (u->AccountIDs.put(s) && db->Accounts.put(a) && db->Customers.put(u));
	}
	return b;
}

/// <summary>
/// Creates an employee account. just needs user & pass
/// </summary>
/// <param name="user">username</param>
/// <param name="pass">password</param>
/// <returns>were we successful? bool </returns>
bool Server::employeeCreation(std::string user, std::string pass)
{
	bool b = false;
	if (db->Customers.find(user) == -1)
	{
		std::shared_ptr<DB::Employee> u = std::shared_ptr<DB::Employee>(new DB::Employee(user, pass));
		b = db->Employees.put(u);
	}
	return b;
}

/// <summary>
/// creating a new account for an existing user
/// </summary>
/// <param name="user">existing user</param>
/// <param name="acc">account name to make</param>
/// <param name="t">type code</param>
/// <param name="deposit">deposit amount</param>
/// <returns>were we successful, bool</returns>
bool Server::accountCreation(std::string user, std::string acc, int t, double deposit)
{
	bool b = false;
	int i = db->Customers.find(user);
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c)
		{
			std::shared_ptr<DB::Transaction> tr(new DB::Deposit(deposit));
			if (c->AccountIDs.find(acc) == -1 && db->Accounts.find(acc) == -1)
			{
				std::shared_ptr<DB::Account> a; //make empty pointer
				switch (t) //switch based on #
				{
					case 0:
						a = std::shared_ptr<DB::Account>(new DB::Saving(tr, acc));
						break;
					case 1:
						a = std::shared_ptr<DB::Account>(new DB::Checking(tr, acc));
						break;
					case 2:
						a = std::shared_ptr<DB::Account>(new DB::CertOfDep(tr, acc));
						break;
					case 3:
						a = std::shared_ptr<DB::Account>(new DB::MoneyMarket(tr, acc));
						break;
					default:
						break;
				}
				if (a) //make sure our pointer isn't empty
				{
					std::shared_ptr<std::string> id(new std::string(a->ID)); //make ID
					b = c->AccountIDs.put(id) && db->Accounts.put(a); //put ID & account
				}
			}
		}
	}
	return b;
}

/// <summary>
/// transfer between accounts
/// </summary>
/// <param name="user">user to transfer for</param>
/// <param name="acc">account 1</param>
/// <param name="acc2">account 2</param>
/// <param name="amnt">amount to transfer</param>
/// <returns>were we successful, bool</returns>
bool Server::accountsTransfer(std::string user, std::string acc, std::string acc2, double amnt)
{
	bool b = false;
	int i = db->Customers.find(user);
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c)
		{
			b = c->transfer(db, acc, acc2, amnt);
		}
	}
	return b;
}

/// <summary>
/// Depositing into account
/// </summary>
/// <param name="user">user to deposit for</param>
/// <param name="acc">account to deposit in</param>
/// <param name="deposit">amount to deposit</param>
/// <returns>were we successful? bool</returns>
bool Server::accountDeposit(std::string user, std::string acc, double deposit)
{
	bool b = false;
	int i = db->Customers.find(user);
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c)
		{
			b = c->deposit(db, acc, deposit);
		}
	}
	return b;
}

/// <summary>
/// get the count of accounts. For customers, their accounts. for Employees, all accounts.
/// </summary>
/// <param name="user">user to get</param>
/// <returns>count of accounts, int</returns>
int Server::accountsCount(std::string user)
{
	int i = db->Customers.find(user);
	if (i != -1)
	{
		return db->Customers.get(i)->AccountIDs.getCount();
	}
	else
	{
		int j = db->Employees.find(user);
		if (j != -1)
		{
			return db->Accounts.getCount();
		}
	}
	return 0;
}

/// <summary>
/// account display, displays a summary of 1 account
/// </summary>
/// <param name="user">user to check account against</param>
/// <param name="acc">account to find</param>
/// <returns>text for account display, string</returns>
std::string Server::accountDisplay(std::string user, std::string acc)
{
	int i = db->Customers.find(user); //get user 
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c && c->AccountIDs.find(acc) >= 0)
		{
			int z = db->Accounts.find(acc);
			std::shared_ptr<DB::Account> a = db->Accounts.get(z);
			if (a) return a->preview();
		}
	}
	else
	{
		int j = db->Employees.find(user);
		if (j != -1)
		{
			int k = db->Accounts.find(acc);
			if (k > -1) return db->Accounts.get(k)->preview();
		}
	}

	return "";
}

/// <summary>
/// displays all accounts; all of customers, or all of them for employees
/// </summary>
/// <param name="user">user to check against</param>
/// <returns>a combined string of all the accounts</returns>
std::string Server::accountsDisplay(std::string user)
{
	std::string s = "";
	int i = db->Customers.find(user); //get user 
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c)
		{
			for (int j = 0; j < c->AccountIDs.getCount(); j++)
			{
				s += accountDisplay(user, *c->AccountIDs.get(j));
			}
		}
	}
	else
	{
		int j = db->Employees.find(user);
		if (j != -1)
		{
			for (int j = 0; j < db->Accounts.getCount(); j++)
			{
				s += accountDisplay(user, db->Accounts.get(j)->ID);
			}
		}
	}

	return s;
}

/// <summary>
/// list of account transactions
/// </summary>
/// <param name="user">user to check against</param>
/// <param name="acc">account to get</param>
/// <returns>list of all transactions, string</returns>
std::string Server::accountTransactions(std::string user, std::string acc)
{
	int i = db->Customers.find(user); //get user 
	if (i != -1)
	{
		std::shared_ptr<DB::Customer> c = db->Customers.get(i);
		if (c && c->AccountIDs.find(acc) >= 0)
		{
			int z = db->Accounts.find(acc);
			std::shared_ptr<DB::Account> a = db->Accounts.get(z);
			if (a) return a->transactionHistory();
		}
	}
	else
	{
		int j = db->Employees.find(user);
		if (j != -1)
		{
			int k = db->Accounts.find(acc);
			if (k != -1) return db->Accounts.get(k)->transactionHistory();
		}
	}

	return "";
}