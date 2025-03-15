
#include "BankDB.h"

using namespace DB;

int Customer::transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v)
//Transfer between accounts; int for return code. Customers need to own/have access to account
{
	if (AccountIDs.find(acc1) >= 0 && AccountIDs.find(acc2) >= 0) //if the customer has access to both accounts (find returns -1 if it can't find anything)
	{
		std::shared_ptr<Account> Account1 = d->Accounts.get(acc1); //grab account 1
		std::shared_ptr<Account> Account2 = d->Accounts.get(acc2); //grab account 2

		//bool can be converted to int, so i can return it. SendTransfer creates the dollar amount & puts a negative transaction in Account 1,
		// passing dollar amount to Account 2 which will also confirm the transaction completed successfully when done
		return Account2->receiveTransfer(Account1->sendTransfer(v), acc1);
	}
	else
	{
		return 0;
	}
}

int Customer::deposit(std::shared_ptr<Database> d, std::string acc, double v)
{
	if (AccountIDs.find(acc))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Employee::transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v)
//Transfer between accounts; int for return code. Employees don't care about account ownership
{
	if (d->Accounts.find(acc1) >= 0 && d->Accounts.find(acc2) >= 0)
		//check if both accounts exist (find returns -1 if it can't find anything)
	{
		std::shared_ptr<Account> Account1 = d->Accounts.get(acc1); //grab account 1
		std::shared_ptr<Account> Account2 = d->Accounts.get(acc2); //grab account 2

		//bool can be converted to int, so i can return it. SendTransfer creates the dollar amount & puts a negative transaction in Account 1,
		// passing dollar amount to Account 2 which will also confirm the transaction completed successfully when done
		return Account2->receiveTransfer(Account1->sendTransfer(v), acc1);
	}
	else
	{
		return 0;
	}
}

int Employee::deposit(std::shared_ptr<Database> d, std::string acc, double v)
{
	if (d->Accounts.find(acc))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}