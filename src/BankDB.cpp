
#include "BankDB.h"

using namespace DB;

bool Customer::transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v)
//Transfer between accounts; int for return code. Customers need to own/have access to account
{
	if (AccountIDs.find(acc1) >= 0 && AccountIDs.find(acc2) >= 0) //if the customer has access to both accounts (find returns -1 if it can't find anything)
	{
		std::shared_ptr<Account> Account1 = d->Accounts.get(acc1); //grab account 1
		std::shared_ptr<Account> Account2 = d->Accounts.get(acc2); //grab account 2

		//bool can be converted to int, so i can return it. SendTransfer creates the dollar amount & puts a negative transaction in Account 1,
		// passing dollar amount to Account 2 which will also confirm the transaction completed successfully when done
		if (Account1 && Account2)
		{
			bool b = Account2->receiveTransfer(Account1->sendTransfer(v), acc1);
			return b;
		}
		
	}
	else
	{
		return false;
	}
}

bool Customer::deposit(std::shared_ptr<Database> d, std::string acc, double v)
{
	if (AccountIDs.find(acc) >= 0)
	{
		std::shared_ptr<Account> accountToDep = d->Accounts.get(acc); //grab account
		if (accountToDep)
		{
			bool b = accountToDep->deposit(v); //get deposit
			return b; //return success/fail
		}
	}
	else
	{
		return false;
	}
}

bool Employee::transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v)
//Transfer between accounts; int for return code. Employees don't care about account ownership
{
	if (d->Accounts.find(acc1) >= 0 && d->Accounts.find(acc2) >= 0)
		//check if both accounts exist (find returns -1 if it can't find anything)
	{
		std::shared_ptr<Account> Account1 = d->Accounts.get(acc1); //grab account 1
		std::shared_ptr<Account> Account2 = d->Accounts.get(acc2); //grab account 2

		//bool can be converted to int, so i can return it. SendTransfer creates the dollar amount & puts a negative transaction in Account 1,
		// passing dollar amount to Account 2 which will also confirm the transaction completed successfully when done
		if (Account1 && Account2)
		{
			bool b = Account2->receiveTransfer(Account1->sendTransfer(v), acc1);
			return b;
		}
	}
	else
	{
		return false;
	}
}

bool Employee::deposit(std::shared_ptr<Database> d, std::string acc, double v)
{
	if (d->Accounts.find(acc) >= 0)
	{
		std::shared_ptr<Account> accountToDep = d->Accounts.get(acc); //grab account
		return accountToDep->deposit(v);
	}
	else
	{
		return false;
	}
}

/// <summary>
/// Handles overdraft for a specific user
/// </summary>
/// <param name="c">Customer shared pointer</param>
/// <returns>was successful?, bool</returns>
bool Overdraft::OnPurchase(std::string user, std::shared_ptr<Database> d)
{
	bool success = false;
	std::shared_ptr<Customer> cust = d->Customers.get(user);
	if (cust)
	{
		for (int i = 0; i < cust->AccountIDs.getCount(); i++)
		{
			//pointer to account, from account get -> dereferenced pointer to AccountID string
			std::shared_ptr<Account> a;
			std::string accID = "";
			if (cust->AccountIDs.get(i))
			{
				accID = *cust->AccountIDs.get(i);
			}
			a = d->Accounts.get(accID);
			//check if we actually got the account
			if (a)
			{
				//if so, continue & see if account's balance is less than 0
				if (a->balance < 0)
				{
					//check accounts again
					for (int j = 0; j < cust->AccountIDs.getCount(); j++)
					{
						std::shared_ptr<Account> b;
						std::string accID2 = "";
						if (cust->AccountIDs.get(j))
						{
							accID2 = *cust->AccountIDs.get(j);
						}
						b = d->Accounts.get(accID2);
						//make sure b exists
						if (b)
						{
							//extra if to skip trying to pull money from itself, Just In Case; preemptive bug fix
							if (!(accID2 == accID))
							{
								//go until account balance is fixed
								while (a->balance < 0 && b->available > 0)
								{
									//make amount to sub, starting at 5 bucks
									double amtToSub = 5.00;
									//scale amount to sub based on amount currently missing
									if (a->balance < USDollar(20.00))
									{
										amtToSub = 50.00;
									}
									else if (a->balance < USDollar(10.00))
									{
										amtToSub = 20.00;
									}
									else if (a->balance < USDollar(5.00))
									{
										amtToSub = 10.00;
									}

									//send from account b to account A
									success = cust->transfer(d, accID2, accID, amtToSub) == 0;
								}
							}
						}
					}
				}
			}
		}
	}
	return success;
}