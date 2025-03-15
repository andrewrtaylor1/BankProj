
#include "BankServer.h"
#include "BankClient.h"
#include <iostream>

using namespace Client;

Serv::Server server;

//function implementations 

/// <summary>
/// boolean inputs
/// </summary>
/// <param name="def">boolean for default option, defaults to no(default=false)</param>
/// <returns>Yes or no (true, false)</returns>
bool Menu::YesNo(bool def) 
{
	bool b = def;
	return b;
}

/// <summary>
/// dynamic inputs; takes input & then iterates through a string to find return code. -1 for no valid input
/// </summary>
/// <param name="s">string of characters</param>
/// <returns>integer of picked option, -1 if none</returns>
int Menu::DynamicOptions(std::string s)
{
	int r = -1;
	return r;
}

/// <summary>
/// string input, returns empty string if nothing is recieved
/// </summary>
/// <param name="len">input length</param>
/// <returns>string from user</returns>
std::string Menu::TextInput(int len)
{
	std::string s = "";
	return s;
}

/// <summary>
/// landing page
/// </summary>
void MnuStart::logic()
{
	
}

/// <summary>
/// login for customers
/// </summary>
void MnuCustomerLogin::logic()
{
	
}

/// <summary>
/// login for employees
/// </summary>
void MnuEmployeeLogin::logic()
{
	
}

/// <summary>
/// customer overview
/// </summary>
void MnuCustomerStart::logic()
{
	
}

/// <summary>
/// employee overview
/// </summary>
void MnuEmployeeStart::logic()
{
	
}

/// <summary>
/// page to make a new employee login
/// </summary>
void MnuEmployeeCreation::logic()
{
	
}
/// <summary>
/// page to make a new customer login & their first account
/// </summary>
void MnuCustomerCreation::logic()
{
	
}

/// <summary>
/// move money between accounts
/// </summary>
void MnuTransferBetweenAccounts::logic()
{
	
}

/// <summary>
/// employee despositing cash; no self service/ATM yet
/// </summary>
void MnuDeposit::logic()
{
	
}

/// <summary>
/// view the transaction history
/// </summary>
void MnuGetAccountHistory::logic()
{
	std::cout << server.accountDisplay(user, account);
	std::cout << server.accountTransactions(user, account);
}