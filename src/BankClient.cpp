
#include "BankServer.h"
#include "BankClient.h"
#include <iostream>

using namespace Client;

//next 2 functions are for terminal-related security. These should be unneeded when switching to a better UI in the future

/// <summary>
/// Clears output in modern terminals
/// </summary>
void clearScreenANSI() {
	std::cout << "\033[2J\033[1;1H"; //uses ANSI Escape codes to clear the console, when supported
}
/// <summary>
/// Clear terminal on specific systems. This is not ideal (system shell calls aren't great), but it'll work in our test CMD
/// </summary>
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

//pointer to new server
std::shared_ptr<Serv::Server> server = std::shared_ptr<Serv::Server>(new Serv::Server);

//function implementations 

/// <summary>
/// boolean inputs
/// </summary>
/// <param name="def">boolean for default option, defaults to no(default=false)</param>
/// <returns>Yes or no (true, false)</returns>
bool Menu::YesNo(bool def) 
{
	bool b = def; //grab the default
	std::string s = ""; //make empty string
	std::cout << "(Y/y for yes, N/n for no; default: " << (def ? "yes" : "no") << ")"; //display the request
	std::getline(std::cin, s); //get the input
	if (s.compare("Y") == 0 || s.compare("y") == 0) b = true; //if yes, true
	if (s.compare("N") == 0 || s.compare("n") == 0) b = false; //if no, false
	//if anything else, just have the default
	return b; //return value
}

/// <summary>
/// dynamic inputs; takes input & then iterates through a string to find return code. -1 for no valid input
/// </summary>
/// <param name="s">string of characters</param>
/// <returns>integer of picked option, -1 if none</returns>
int Menu::DynamicOptions(std::string s)
{
	int r = -1; //default value
	//number inputs give first 10, probably all we need
	if (s.compare("0") == 0) r = 0;
	if (s.compare("1") == 0) r = 1;
	if (s.compare("2") == 0) r = 2;
	if (s.compare("3") == 0) r = 3;
	if (s.compare("4") == 0) r = 4;
	if (s.compare("5") == 0) r = 5;
	if (s.compare("6") == 0) r = 6;
	if (s.compare("7") == 0) r = 7;
	if (s.compare("8") == 0) r = 8;
	if (s.compare("9") == 0) r = 9;
	return r;
}

/// <summary>
/// string input, returns empty string if nothing is recieved
/// </summary>
/// <param name="s">The input ask text</param>
/// <returns>string from user</returns>
std::string Menu::TextInput(std::string s)
{
	std::string sin = ""; //empty string
	std::cout << s; //ask for input
	std::getline(std::cin, sin); //just grab the line
	return sin; //return whatever we've got
}

/// <summary>
/// landing page
/// </summary>
void MnuStart::logic()
{
	std::cout << "Welcome to the Comprehensive Banking System. How may we help you today?\n 0) Customer Login\n 1) Employee Login\n 2) Quit\n"; //give quick menu landing
	bool running = true; //we're starting to grab input
	int i = -1; //default is negative 1; declared here so it isn't redeclared every loop
	while (running) //go until asked to stop
	{
		i = DynamicOptions(TextInput()); //ask for input, convert to int with option function
		if (i == 0)
		{
			MnuCustomerLogin cl; //start the customer login
			//fake a full reset, we just need to continue the loop
			std::cout << "Reset in progress.\nWelcome to the Comprehensive Banking System. How may we help you today?\n 0) Customer Login\n 1) Employee Login\n 2) Quit\n";
		}
		else if (i == 1)
		{
			MnuEmployeeLogin el; //start the employee login
			//fake a full reset, we just need to continue the loop
			std::cout << "Reset in progress.\nWelcome to the Comprehensive Banking System. How may we help you today?\n 0) Customer Login\n 1) Employee Login\n 2) Quit\n";
		}
		else if (i == 2)
		{
			running = false; //close the loop, shutting the program down
		}
		else
		{
			std::cout << "I'm sorry, that's not one of the options, try again.\nYour options are: \n 0) Customer Login\n 1) Employee Login\n"; //redisplay options
		}
	}
}

/// <summary>
/// login for customers
/// </summary>
void MnuCustomerLogin::logic()
{
	std::cout << "You are trying to login as a Customer.";
	bool login = true; //we are running the login functionality
	while (login) //run until we get success
	{
		std::cout << "Please supply your";
		std::string user     = TextInput(" Username: "); //get the username
		std::string password = TextInput(" Password: "); //get the password
		if (server->userValidation(user, password) == 0)
		{
			clearScreen();
			clearScreenANSI();
			MnuCustomerStart cs(user, password);
			login = false;
		}
		else
		{
			std::cout << "Username or password incorrect. Please try again. ";
		}
	}
}

/// <summary>
/// login for employees
/// </summary>
void MnuEmployeeLogin::logic()
{
	std::cout << "You are trying to login as an Employee.";
	bool login = true; //we are running the login functionality
	while (login) //run until we get success
	{
		std::cout << "Please supply your";
		std::string user     = TextInput(" Username: "); //get the username
		std::string password = TextInput(" Password: "); //get the password
		if (server->userValidation(user, password) == 1)
		{
			clearScreen();
			clearScreenANSI();
			MnuEmployeeStart es(user, password);
			login = false;
		}
		else
		{
			std::cout << "Username or password incorrect. Please try again. ";
		}
	}
}

/// <summary>
/// customer overview
/// </summary>
void MnuCustomerStart::logic()
{
	if (server->userValidation(user, pass) == 0)
	{
		std::cout << "Welcome, valued customer\n"; //give quick menu landing
		bool running = true; //we're starting to grab input
		int i = -1; //default is negative 1; declared here so it isn't redeclared every loop
		while (running) //go until asked to stop
		{
			std::cout << "Your options are 0) Account Summaries 1) Transfer Between Accounts 2) Get Account History 3) Password Change 4) Go Back\n";
			i = DynamicOptions(TextInput()); //ask for input, convert to int with option function
			if (i == 0)
			{
				MnuGetAccounts ga(user, pass); //get accounts summary
				
			}
			else if (i == 1)
			{
				MnuTransferBetweenAccounts trnsfr(user, pass); //transfer between accounts
			}
			else if (i == 2)
			{
				std::string id = TextInput("Account ID: "); //grab account ID
				MnuGetAccountHistory ah(user, pass, id); //show account history
			}
			else if (i == 3)
			{
				MnuChangePassword ch(user, pass); //change password
			}
			else if (i == 4)
			{
				running = false; //end the loop
			}
			else
			{
				std::cout << "I'm sorry, that's not one of the options, try again.\n"; //redisplay options
			}
		}
	}
}

/// <summary>
/// employee overview
/// </summary>
void MnuEmployeeStart::logic()
{
	if (server->userValidation(user, pass) == 1)
	{
		std::cout << "Welcome, valued employee.\n"; //give quick menu landing
		bool running = true; //we're starting to grab input
		int i = -1; //default is negative 1; declared here so it isn't redeclared every loop
		while (running) //go until asked to stop
		{
			std::cout << "Your options are: 0) Transfer Between Accounts 1) Desposit To Account\n2) Customer Creation 3) Account Creation 4) Employee Creation 5) Get Accounts\n 6) Get Account History 7) Manual Bank Operations 8) Change Passwords 9) Go Back\n";
			i = DynamicOptions(TextInput()); //ask for input, convert to int with option function
			if (i == 0)
			{
				MnuTransferBetweenAccounts trnsfr(user, pass); //start the transfer menu
			}
			else if (i == 1)
			{
				MnuDeposit dep(user, pass); //start the deposit menu
			}
			else if (i == 2)
			{
				MnuCustomerCreation cc(user, pass); //start customer creation
				clearScreen();
				clearScreenANSI();
			}
			else if (i == 3)
			{
				MnuAccountCreation ac(user, pass); //start making a new account
			}
			else if (i == 4)
			{
				MnuEmployeeCreation ec(user, pass); //start making anew employee entry
				clearScreen();
				clearScreenANSI();
			}
			else if (i == 5)
			{
				MnuGetAccounts ga(user, pass); //get accounts summaries
			}
			else if (i == 6)
			{
				std::string id = TextInput("Account ID: "); //grab account ID
				MnuGetAccountHistory ah(user, pass, id); //show account history
			}

			else if (i == 7)
			{
				MnuManualBankOperations bo(user,pass); //manual operations
			}
			else if (i == 8)
			{
				MnuChangePassword ch(user, pass); //change password
			}
			else if (i == 9)
			{
				running = false; //end the loop
			}
			
			else
			{
				std::cout << "I'm sorry, that's not one of the options, try again.\n"; //redisplay options
			}
		}
	}
}

/// <summary>
/// page to make a new employee login
/// </summary>
void MnuEmployeeCreation::logic()
{
	if (server->userValidation(user, pass) == 1)
	{
		std::cout << "You are trying to create an Employee.";
		bool create = true; //we are running the login functionality
		while (create) //run until we get success
		{
			std::cout << "Please supply the";
			std::string userC     = TextInput(" Username: "); //get the username
			std::string passwordC = TextInput(" Password: "); //get the password
			if (server->employeeCreation(userC,passwordC)) //make employee
			{
				create = false;
			}
			else
			{
				std::cout << "Error Occured, try again\n";
			}
		}
	}
}
/// <summary>
/// page to make a new customer login & their first account
/// </summary>
void MnuCustomerCreation::logic()
{
	if (server->userValidation(user, pass) == 1)
	{
		std::cout << "You are trying to create a Customer.";
		bool create = true; //we are running the login functionality
		while (create) //run until we get success
		{
			std::cout << "Please supply the";
			std::string userC = TextInput(" Username: "); //get the username
			std::string passwordC = TextInput(" Password: "); //get the password
			std::string accC = TextInput(" Account ID: "); //get the account ID
			//get the deposit amount, convert to double
			double d = 1;
			try
			{
				d = stod(TextInput(" Deposit Amount (0.00): "));
			}
			catch (...)
			{
				std::cout << "Not a number or an error occured.";
				return;
			}
			if (server->userCreation(userC, passwordC, accC, d)) //make user
			{
				create = false;
			}
			else
			{
				std::cout << "Error Occured, try again\n";
			}
		}
	}
}

/// <summary>
/// allows changing passwords
/// </summary>
void MnuChangePassword::logic()
{
	if (server->userValidation(user, pass) == 1) //employee
	{
		std::cout << "You are changing the password as an Employee\nPlease supply the";
		int type = DynamicOptions(TextInput(" Account Type (O, Customer; 1, Current Employee): "));
		if (type == 0)
		{
			std::cout << "You are changing a customer's password. Please supply";
			std::string userC = TextInput(" Username: "); //get the username
			std::string passwordC = TextInput(" New Password: "); //get the password
			if (server->userPassword(userC, passwordC, 1))
			{
				clearScreen();
				clearScreenANSI();
				std::cout << "You have successfully changed a password.\n";
			}
			else
			{
				clearScreen();
				clearScreenANSI();
				std::cout << "The change was not successful.\n";
			}
		}
		else if (type == 1)
		{
			std::cout << "You are changing your own password. Please supply";
			std::string passwordOld = TextInput(" Old Password: "); //get the password
			std::string passwordNew = TextInput(" New Password: "); //get the new password
			if (passwordOld == pass)
			{
				if (server->userPassword(user, passwordNew, 2))
				{
					clearScreen();
					clearScreenANSI();
					std::cout << "You have successfully changed your password.\n";
				}
				else
				{
					clearScreen();
					clearScreenANSI();
					std::cout << "The change was not successful\n";
				}
			}
			else
			{
				std::cout << "Credentials problem, exiting.";
			}
		}
		else
		{
			std::cout << "Did not pick a valid option, exiting.";
		}
		
	}
	else if (server->userValidation(user, pass) == 0) //user
	{
		std::cout << "You are changing the password as a Customer\nPlease supply the";
		std::string passwordOld = TextInput(" Old Password: "); //get the password
		std::string passwordNew = TextInput(" New Password: "); //get the new password
		if (passwordOld == pass)
		{
			if (server->userPassword(user, passwordNew, 0))
			{
				clearScreen();
				clearScreenANSI();
				std::cout << "You have successfully changed your password.\n";
			}
			else
			{
				clearScreen();
				clearScreenANSI();
				std::cout << "The change was not successful\n";
			}
		}
		else
		{
			std::cout << "Credentials problem, exiting.\n";
		}
	}
}

/// <summary>
/// making a new account
/// </summary>
void MnuAccountCreation::logic()
{
	if (server->userValidation(user, pass) == 1)
	{
		bool create = true; //we are running the login functionality
		if (server->accountsCount(user) < 1) create = false; //fail if we don't have any accounts
		while (create) //run until we get success
		{
			std::cout << "Please supply the";
			std::string userC = TextInput(" Username: "); //get the username
			std::string accC = TextInput(" Account ID: "); //get the account ID
			int i = DynamicOptions(TextInput(" Account Type\n(0 - Savings, 1 - Checking, 2 - Certificate of Deposit, 3 - Money Market): "));
			//get the deposit amount, convert to double
			double d = 1;
			try
			{
				d = stod(TextInput(" Deposit Amount (0.00): "));
			}
			catch (...)
			{
				std::cout << "Not a number or an error occured.";
				return;
			}
			if (server->accountCreation(userC, accC, i, d)) //make account
			{
				create = false;
			}
			else
			{
				std::cout << "Error Occured, try again\n";
			}
		}
	}
}

/// <summary>
/// move money between accounts
/// </summary>
void MnuTransferBetweenAccounts::logic()
{
	int i = server->userValidation(user, pass);
	if (i >= 0)
	{
		bool create = true; //we are running the login functionality
		std::string userC = user;
		while (create) //run until we get success
		{
			std::cout << "Please supply the";
			if (i == 1)
			{
				userC = TextInput(" Username: "); //get the username
				if (userC == "")
				{
					userC = user;
					std::cout << "No username supplied, assuming transfer between users.\n";
				}
			}
			std::string accC = TextInput(" Account ID: "); //get the account ID
			std::string acc2C = TextInput(" 2nd Account ID: "); //get the account ID again
			//get the transfer amount, convert to double
			double d = 1;
			try
			{
				d = stod(TextInput(" Transfer Amount (0.00): "));
			}
			catch (...)
			{
				std::cout << "Not a number or an error occured.";
				return;
			}
			if (d < 0) d = -d; //make sure it's positive
			if (server->accountsTransfer(userC, accC, acc2C, d)) //make transfers
			{
				create = false;
			}
			else
			{
				std::cout << "Error Occured, try again\n";
			}
		}
	}
}

/// <summary>
/// employee despositing cash; no self service/ATM yet
/// </summary>
void MnuDeposit::logic()
{
	if (server->userValidation(user, pass) == 1)
	{
		bool create = true; //we are running the login functionality
		while (create) //run until we get success
		{
			std::cout << "Please supply the";
			std::string userC = TextInput(" Username: "); //get the username
			std::string accC = TextInput(" Account ID: "); //get the account ID
			//get the deposit amount, convert to double
			double d = 1;
			try
			{
				d = stod(TextInput(" Deposit Amount (0.00): "));
			}
			catch (...)
			{
				std::cout << "Not a number or an error occured.";
				return;
			}
			if (d < 0) d = -d; //make sure it's positive
			if (server->accountDeposit(userC, accC, d)) //make deposit
			{
				create = false;
			}
			else
			{
				std::cout << "Error Occured, try again\n";
			}
		}
	}
}

/// <summary>
/// Get the summary of every account for a user
/// </summary>
void MnuGetAccounts::logic()
{
	int access = server->userValidation(user, pass); //validate & get privlege
	if (access >= 0)
	{
		std::cout << server->accountsDisplay(user); //display the accounts
		std::cout << "Amount of accounts: " << server->accountsCount(user) << "\n"; //also get count of accounts
	}
}

/// <summary>
/// view the transaction history
/// </summary>
void MnuGetAccountHistory::logic()
{
	if (server->userValidation(user, pass) >= 0)
	{
		std::cout << server->accountDisplay(user, account);
		std::cout << server->accountTransactions(user, account);
	}
}

/// <summary>
/// While task scheduling & online processing is not implemented, handle purchases & ongoing bank functions here
/// </summary>
void MnuManualBankOperations::logic()
{
	if (server->userValidation(user, pass) == 1) //make sure you're an employee
	{
		std::cout << "Manual bank functions are available.\n";
		bool running = true;
		int i = -1; //default is negative 1; declared here so it isn't redeclared every loop
		while (running)
		{
			std::cout << "Your options are: 0) Purchases 1) Recurring Tasks (Interest) 2) Exit\n";
			i = DynamicOptions(TextInput()); //ask for input, convert to int with option function
			if (i == 0)
			{
				std::string us = TextInput(" Username: "); //username
				std::string acc = TextInput(" Account ID: "); //account id
				//get the purchase amount, convert to double
				double d = 1;
				try
				{
					d = stod(TextInput(" Purchase Amount (0.00): "));
				}
				catch (...)
				{
					std::cout << "Not a number or an error occured.";
					return;
				}
				if (d < 0) d = -d; //make sure it's positive
				std::string originAndName = "Manual Purchase";
				server->purchase(us,acc,d,originAndName,originAndName); //purchase
			}
			else if (i == 1)
			{
				std::cout << "Running banking tasks.";
				server->runBankProccesses(); //run said banking tasks
			}
			else if (i == 2)
			{
				running = false; //end the loop
			}
			else
			{
				std::cout << "I'm sorry, that's not one of the options, try again.\n"; //redisplay options
			}
		}
	}
}