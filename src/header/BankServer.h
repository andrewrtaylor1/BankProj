#pragma once

#include "Encrypt.h"
#include "List.h"

namespace Serv
{

	/// <summary>
	///	Connection layer between Client & DB; will Encrypt & Decrypt everything once encryption is implemented.
	/// </summary>
	class Server
	{
		public:
			Server(){}
			~Server(){}

			//validates the user, customer or employee
			int userValidation(std::string user, std::string pass);
			//creates the user
			bool userCreation(std::string user, std::string pass, std::string acc, double deposit = 10.00);
			//creates the employee
			bool employeeCreation(std::string user, std::string pass);
			//adds account to user
			bool accountCreation(std::string user, std::string acc, int t, double deposit=10.00);
			//transfers between accounts
			bool accountsTransfer(std::string user, std::string acc, std::string acc2, double amnt);
			//deposits into account
			bool accountDeposit(std::string user, std::string acc, double deposit);
			//counts accounts available
			int accountsCount(std::string user);
			//displays summary of accounts
			std::string accountDisplay(std::string user, std::string acc);
			//displays summaries
			std::string accountsDisplay(std::string user);
			//gets transactions
			std::string accountTransactions(std::string user, std::string acc);
	};
}