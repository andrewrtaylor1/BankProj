#pragma once

#include <string>

namespace Client
{
	
	//base Menu class, will display and handle logic for each action
	class Menu
	{
		public:
			Menu(){}
			virtual ~Menu(){}
			virtual void logic() = 0;
	};

	//specific menu classes
	class MnuStart : public Menu
	{
		public:
			MnuStart() {}
			~MnuStart() {
			
			}
			void logic() override;

	};
	class MnuCustomerLogin : public Menu
	{
		public:
			MnuCustomerLogin() {}
			~MnuCustomerLogin() {
	
			}
			void logic() override;
			std::string login;
	};
	class MnuEmployeeLogin : public Menu
	{
		public:
			MnuEmployeeLogin() {}
			~MnuEmployeeLogin() {
	
			}
			void logic() override;
			std::string login;
	};
	class MnuCustomerStart : public Menu
	{
		public:
			MnuCustomerStart() {}
			~MnuCustomerStart() {
	
			}
			void logic() override;
			std::string user;
	};
	class MnuEmployeeStart : public Menu
	{
		public:
			MnuEmployeeStart() {}
			~MnuEmployeeStart() {
	
			}
			void logic() override;
			std::string user;
	};
	class MnuEmployeeCreation : public Menu
	{
		public:
			MnuEmployeeCreation() {}
			~MnuEmployeeCreation() {
	
			}
			void logic() override;
	};
	class MnuCustomerCreation : public Menu
	{
		public:
			MnuCustomerCreation() {}
			~MnuCustomerCreation() {
	
			}
			void logic() override;
	};
	class MnuTransferBetweenAccounts : public Menu
	{
		public:
			MnuTransferBetweenAccounts() {} 
			~MnuTransferBetweenAccounts() {
	
			}
			void logic() override;
			std::string account1;
			std::string account2;
	};
	class MnuDeposit : public Menu
	{
		public:
			MnuDeposit() {}
			~MnuDeposit() {
	
			}
			void logic() override;
			std::string account;
	};
	class MnuGetAccountHistory : public Menu
	{
		public:
			MnuGetAccountHistory() {}
			~MnuGetAccountHistory() {
	
			}
			void logic() override;
			std::string account;
	};

	//governing Menu class
	class Client
	{
	public:
		Client() {
			currentMenu = new MnuStart;
		}
		~Client() {}
		Menu* currentMenu;
	};
}
