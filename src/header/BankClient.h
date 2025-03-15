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
			std::string TextInput(int len);

	};

	//specific menu classes
	class MnuStart : public Menu
	{
		public:
			MnuStart() {}
			~MnuStart() {
			
			}
			void logic();

	};
	class MnuCustomerLogin : public Menu
	{
		public:
			MnuCustomerLogin() {}
			~MnuCustomerLogin() {
	
			}
			void logic();
	};
	class MnuEmployeeLogin : public Menu
	{
		public:
			MnuEmployeeLogin() {}
			~MnuEmployeeLogin() {
	
			}
			void logic();
			std::string login;
	};
	class MnuCustomerStart : public Menu
	{
		public:
			MnuCustomerStart() {}
			~MnuCustomerStart() {
	
			}
			void logic();
			std::string user;
	};
	class MnuEmployeeStart : public Menu
	{
		public:
			MnuEmployeeStart() {}
			~MnuEmployeeStart() {
	
			}
			void logic();
			std::string user;
	};
	class MnuEmployeeCreation : public Menu
	{
		public:
			MnuEmployeeCreation() {}
			~MnuEmployeeCreation() {
	
			}
			void logic();
	};
	class MnuCustomerCreation : public Menu
	{
		public:
			MnuCustomerCreation() {}
			~MnuCustomerCreation() {
	
			}
			void logic();
	};
	class MnuTransferBetweenAccounts : public Menu
	{
		public:
			MnuTransferBetweenAccounts() {} 
			~MnuTransferBetweenAccounts() {
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string account1;
			std::string account2;
	};
	class MnuDeposit : public Menu
	{
		public:
			MnuDeposit() {
			
			}
			~MnuDeposit() {
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string account;
	};
	class MnuGetAccountHistory : public Menu
	{
		public:
			MnuGetAccountHistory(std::string us, std::string acc)
			{
				user = us;
				account = acc;
			}
			~MnuGetAccountHistory() {
	
			}
			void logic();
			//strings for logic
			std::string user;
			std::string account;
	};
}
