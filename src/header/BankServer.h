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

			bool userValidation(std::string user, std::string pass);
			bool userCreation(std::string user, std::string pass, std::string acc, double deposit = 0);
			bool accountCreation(std::string user, std::string acc, double deposit=10.00);
			bool accountsTransfer(std::string user, std::string acc, std::string acc2);
			LinkedList<std::string> accountsCount(std::string user);
			std::string accountDisplay(std::string user, std::string acc);
			std::string accountTransactions(std::string user, std::string acc);
	};
}