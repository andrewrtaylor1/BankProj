#pragma once

#include <string>
#include <memory>

namespace Client
{

	//base Menu class, will display and handle logic for each action
	class Menu
	{
		public:
			Menu(){}
			virtual ~Menu(){}

			virtual void logic() = 0;
			//function declarations (generic input functions)
			bool YesNo(bool def = false);
			int DynamicOptions(std::string s);
			std::string TextInput(std::string s = "Input here: ");

	};

	//specific menu classes
	class MnuStart : public Menu
	{
		public:
			MnuStart() { logic(); }
			~MnuStart() {
			
			}
			void logic();

	};
	class MnuCustomerLogin : public Menu
	{
		public:
			MnuCustomerLogin() { logic(); }
			~MnuCustomerLogin() {
	
			}
			void logic();
	};
	class MnuEmployeeLogin : public Menu
	{
		public:
			MnuEmployeeLogin() { logic(); }
			~MnuEmployeeLogin() {
	
			}
			void logic();
			std::string login;
	};
	class MnuCustomerStart : public Menu
	{
		public:
			MnuCustomerStart(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuCustomerStart() {
	
			}
			void logic();
			std::string user;
			std::string pass;
	};
	class MnuEmployeeStart : public Menu
	{
		public:
			MnuEmployeeStart(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuEmployeeStart() {
	
			}
			void logic();
			std::string user;
			std::string pass;
	};
	class MnuEmployeeCreation : public Menu
	{
		public:
			MnuEmployeeCreation(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuEmployeeCreation() {
	
			}
			void logic();
			std::string user;
			std::string pass;
	};
	class MnuCustomerCreation : public Menu
	{
		public:
			MnuCustomerCreation(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuCustomerCreation() {
	
			}
			void logic();
			std::string user;
			std::string pass;
	};

	class MnuChangePassword : public Menu
	{
	public:
		MnuChangePassword(std::string us, std::string pss)
		{
			user = us;
			pass = pss;
			logic();
		}
		~MnuChangePassword() {

		}
		void logic();
		std::string user;
		std::string pass;
	};

	class MnuAccountCreation : public Menu
	{
	public:
		MnuAccountCreation(std::string us, std::string pss)
		{
			user = us;
			pass = pss;
			logic();
		}
		~MnuAccountCreation() {

		}
		void logic();
		std::string user;
		std::string pass;
	};

	class MnuTransferBetweenAccounts : public Menu
	{
		public:
			MnuTransferBetweenAccounts(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuTransferBetweenAccounts()
			{
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string pass;
	};
	class MnuDeposit : public Menu
	{
		public:
			MnuDeposit(std::string us, std::string pss)
			{
				user = us;
				pass = pss;
				logic();
			}
			~MnuDeposit() {
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string pass;
	};

	class MnuGetAccounts : public Menu
	{
	public:
		MnuGetAccounts(std::string us, std::string pss)
		{
			user = us;
			pass = pss;
			logic();
		}
		~MnuGetAccounts() {

		}
		void logic();
		//strings for logic
		std::string user;
		std::string pass;
	};

	class MnuGetAccountHistory : public Menu
	{
		public:
			MnuGetAccountHistory(std::string us, std::string pss, std::string acc)
			{
				user = us;
				pass = pss;
				account = acc;
				logic();
			}
			~MnuGetAccountHistory() {
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string pass;
			std::string account;
	};


	class MnuManualBankOperations : public Menu
	{
	public:
		MnuManualBankOperations(std::string us, std::string pss)
		{
			user = us;
			pass = pss;
			logic();
		}
		~MnuManualBankOperations() {

		}
		void logic();
		//strings for logic
		std::string user;
		std::string pass;
	};
}
