#pragma once
#include "List.h"
#include <chrono>
#include <string>

namespace DB
{
	//Forward declarations
	class Database;

	/// <summary>
	/// Currency base class
	/// </summary>
	class Currency
	{
		public:
			Currency(int i = 0) //default/int constructor; this isn't human facing so we just need to pass the raw value
			{
				value = i;
			} 
			virtual ~Currency(){} //destructor
			virtual const std::string getName() = 0; //currency Name
			virtual const std::string getSymbol() = 0;//currency symbol, like $
			virtual const std::string formattedValue() = 0; //formatted currency, like $0.00

			//allows string comparison function
			int compare(std::string s)
			{
				return formattedValue().compare(s); //compares formatted value string
			}

		protected:
			int value; //value is stored as int to innately handle 

	};

	/// <summary>
	/// Currency used by the bank, United States Dollars
	/// </summary>
	class USDollar : public Currency
	{
		public:
			USDollar(int i = 1) : Currency(i) {} //default/int constructor; this isn't human facing so we just need to pass the raw value
			USDollar(double d) : Currency((int)(d * 100)) {} //double constructor; human-facing systems & interest calcs will make doubles
			~USDollar() {} //destructor
			//returns the name of the currency. Could be used to ID currency types
			const std::string getName()
			{
				return "US Dollar";
			}
			//gets the symbol; might not need this but could be useful later
			const std::string getSymbol()
			{
				return "$";
			}
			//returns the value with the proper decimal position & dollar sign, as a string
			const std::string formattedValue()
			{
				std::string s = std::to_string(value); //converts value to a string
				//is it negative
				if (s.front() == '-')
				{
					s.erase(0, 1); //get rid of negative
					if (s.length() < 3) s = "00" + s; //adds 2 0's to keep formatting right
					s = "$" + s; //add dollar sign
					s.insert(s.length() - 2, "."); //add decimal point
					s = "-" + s; //add negative back
				}
				else
				{
					if (s.length() < 3) s = "00" + s; //adds 2 0's to keep formatting right
					s = "$" + s; //add dollar sign
					s.insert(s.length() - 2, "."); //add decimal point
				}
				return s; //return string
			}

			//operators & certain other functions do need to be implemented in the derived class

			//Gets Percentage; as the stored int is real value * 100 already, we do have to cast to int
			USDollar GetPercentage(double d)
			{
				return USDollar(int(value * (d / 100)));
			}

			//positive operator
			USDollar operator+() const
			{
				return USDollar(value);
			}

			//negative operator
			USDollar operator-() const
			{
				return USDollar(-value);
			}

			//allows addition
			USDollar operator+(const USDollar& add) const
			{
				return USDollar(value + add.value);
			}

			//allows subtraction
			USDollar operator-(const USDollar& sub) const
			{
				return USDollar(value - sub.value);
			}

			//comparison operators
			
			//equals operator overload
			bool operator==(const USDollar& comp) const
			{
				return value == comp.value;
			}
			//not equal operator overload
			bool operator!=(const USDollar& comp) const
			{
				return value != comp.value;
			}
			//greater than operator overload
			bool operator>(const USDollar& comp) const
			{
				return value > comp.value;
			}
			//less than operator overload
			bool operator<(const USDollar& comp) const
			{
				return value < comp.value;
			}
			//greater than or equal overload
			bool operator>=(const USDollar& comp) const
			{
				return value >= comp.value;
			}
			//less than or equal overload
			bool operator<=(const USDollar& comp) const
			{
				return value <= comp.value;
			}
	};

	/// <summary>
	/// individual transactions
	/// </summary>
	class Transaction
	{
		public:
			Transaction(USDollar c) //constructor; don't want a default constructor cause we always want a dollar amount
			{
				Val = c;
			};
			virtual ~Transaction(){} //destructor
			const std::chrono::system_clock::time_point Timestamp = std::chrono::system_clock::now(); //time, to resolve conflicts + sorting
			USDollar Val; //the actual value of the transaction
			std::string Name = "Transaction"; //default transaction name is Transaction; should be changed 
			std::string Origin = "Bank"; //default Origin is Bank; will need to be changed
			bool Pending = false; //transaction isn't finalized; initalized as false
			bool Suspicious = false; //marks transaction as suspicious

			virtual std::string TransactionType() = 0;

			int compare(std::string s) //lets Compare work on this class; uses all strings available
			{
				return (Name + Origin + TransactionType() + Val.formattedValue()).compare(s);
			}
	};

	/// <summary>
	/// purchases (like debit)
	/// </summary>
	class Purchase : public Transaction
	{
		public:
			Purchase(USDollar c, std::string n, std::string o) : Transaction(c) {
				Name = n;
				Origin = o;
			}
			~Purchase() {}

			std::string TransactionType() { return "Purchase"; }

	};
	/// <summary>
	/// transfer between accounts transaction
	/// </summary>
	class Transfer : public Transaction
	{
		public:
			Transfer(USDollar c, std::string n) : Transaction(c) {
				Name = n;
			}
			~Transfer() {}

			std::string TransactionType() { return "Transfer"; }

	};
	/// <summary>
	/// transaction for deposits
	/// </summary>
	class Deposit : public Transaction
	{
		public:
			Deposit(USDollar c, std::string o = "Bank") : Transaction(c) {
				Name = "Deposit";
				Origin = o;

			}
		~Deposit() {}

		std::string TransactionType() { return "Deposit"; }

	};
	/// <summary>
	/// transaction for overdraft/interest/other
	/// </summary>
	class BankFunction : public Transaction
	{
		public:
			BankFunction(USDollar c, std::string n = "Bank Function") : Transaction(c) {
				Name = n;
			}
			~BankFunction() {}

			std::string TransactionType() { return "Bank Function"; }

	};

	/// <summary>
	/// Bank Accounts base class
	/// </summary>
	class Account
	{
		friend class Interest; //forward declaration of friendship
		public:
			Account(Transaction* t, std::string id) {
				Transactions = LinkedList<Transaction>(std::shared_ptr<Transaction>(t)); //construct Transaction list
				updateBalance(); //get the first balance
				ID = id; //gets the name; we always want a unique name, 0000 would be an error/placeholder
			}
			virtual ~Account(){}
			LinkedList<Transaction> Transactions; //transaction history!
			std::string ID = "0000"; //identifier
			USDollar balance; //total balance; updated when transactions gets changed
			USDollar available; //total available; in theory, it is total balance - account minimum & certain charges
			double APY = 0; //interest rate (can always be expressed as APY, it's just that simple doesn't compound each year)
			int interestType = 0; //0: None, 1: Simple, 2: Compound Yearly, 3: Compound Monthly, 4: Compound Daily
			int payoutRate = 0; //0: Yearly/None, 1: Every 6 months, 2: monthly, 3: daily
			USDollar interestSoFar; //interest accrued so far. This is needed both for compounding & also compound that doesn't pay out at the compound rate
			
			//time members; will just go unused when interest is disabled
			//last time paid out; compared against for current payout. default is now(), whenever it is initialized.
			std::chrono::system_clock::time_point LastPayout = std::chrono::system_clock::now();
			//last time interest came in; compared against for interest. default is now(), whenever it is initialized.
			std::chrono::system_clock::time_point LastInterest = std::chrono::system_clock::now();

			void updateBalance() //updates balance & available 
			{
				USDollar b(0); //balance
				USDollar a(0); //available
				//for each member of the transactions list
				for (int i = 1; i <= Transactions.getCount(); i++)
				{
					//get the transaction as a pointer
					Transaction* t = Transactions.get(i).get();
					//check if transaction isn't pending, add to available
					if (!(t->Pending)) a = a + t->Val;
					b = b + t->Val; //add to balance
					//fill the values
					balance = b;
					available = a;
				}
			}

			int compare(std::string s) const //lets Compare work on this class; gets the ID
			{
				return ID.compare(s);
			}

			//function for displaying the account at a glance
			std::string preview()
			{
				std::string s = "";
				s.append(ID + " : " + this->getType()+ "\n");
				s.append(available.formattedValue() + "  :  " + balance.formattedValue() + "\n\n");
				return s;
			}

			//displays transaction history
			std::string transactionHistory()
			{
				std::string s = "Transaction History:\n";
				for (int i = Transactions.getCount(); i > 0; i--)
				{
					//grabs transaction as pointer (shared ptr lets me make & destroy as many as I want)
					std::shared_ptr<Transaction> t = Transactions.get(i);
					s.append(t->TransactionType() + ": " + t->Name + " - " + t->Origin + "\n"); //type, name, and origin
					s.append(t->Val.formattedValue()+"\n\n"); //value display (money gained/lost)
				}
				return s;
			}

			virtual bool deposit(double d) = 0; //deposit dollar amount
			virtual USDollar sendTransfer(double d) = 0; //creates the transfer dollar amount
			virtual bool receiveTransfer(USDollar d, std::string id) = 0; //receive transfer amount
			virtual bool purchase(double d, std::string name, std::string origin) = 0; //handles purchases
			virtual int processTransaction(std::shared_ptr<Transaction> t) = 0; //receives a new transaction
			virtual void specialFunctions() = 0; //will run certain special functions in derived classes

			virtual std::string getType() //returns account type
			{
				return "Account";
			}

	};

	/// <summary>
	/// User base class, takes a name & password
	/// </summary>
	class User
	{
	public:
		User(std::string s, std::string pass)
		{
			name = s;
			password = pass;
		}
		virtual ~User() {}
		std::string name;
		std::string password;

		int compare(std::string s) //comparison function
		{
			return name.compare(s);
		}

		virtual int transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v) = 0; //Transfer by accounts for user; int for return code
		virtual int deposit(std::shared_ptr<Database> d, std::string acc, double v) = 0; //Deposit into account; int for return code

	};

	/// <summary>
	/// Savings account
	/// </summary>
	class Saving : public Account
	{
		public:
			Saving(Transaction* t, std::string id) : Account(t, id) {}
			~Saving() {}
			void specialFunctions() //handles special functions ()
			{
			}
			bool deposit(double d) //deposits money
			{
				//pretty verbose line here, let's work backwards
				//I make a new dollar amount with the double, pass that to the new Transaction, which gets put into a smart pointer, which is then processed (success is 0)
				return processTransaction(std::shared_ptr<Transaction>(new Deposit(USDollar(d))))==0;
			}
			USDollar sendTransfer(double d) //transfers money
			{
				//simple for right now, logic for special functionality related to transfers will be here
				USDollar dollar = USDollar(d); //makes dollar amount
				processTransaction(std::shared_ptr<Transaction>(new Transfer(-dollar, ID))); //create the transfer
				return dollar;
			}
			bool receiveTransfer(USDollar d, std::string id) //recieves transfered money
			{
				//transfer recieve, success is 0
				return processTransaction(std::shared_ptr<Transaction>(new Transfer(USDollar(d), id)))==0;
			}

			bool purchase(double d, std::string name, std::string origin) //handles purchase
			{
				//purchase success is 0
				return processTransaction(std::shared_ptr<Transaction>(new Purchase(USDollar(d), name, origin)))==0;
			}

			int processTransaction(std::shared_ptr<Transaction> t) //underlying method for processing transactions (int return code for what happened to the transaction)
			{
				//very simple for right now
				int i = 1; //failure code is 1
				if(Transactions.put(t))
				{
					i = 0; //success code is 0
					updateBalance(); //update the balance/available
				}

				return i; //return code
			}

			std::string getType() //returns account type
			{
				return "Savings";
			}
	};

	/// <summary>
	/// Checking account
	/// </summary>
	class Checking : public Account
	{
		public:
			Checking(Transaction* t, std::string id) : Account(t, id) {}
			~Checking() {}
			void specialFunctions() //handles special functions ()
			{
			}
			bool deposit(double d) //deposits money
			{
				//pretty verbose line here, let's work backwards
				//I make a new dollar amount with the double, pass that to the new Transaction, which gets put into a smart pointer, which is then processed (success is 0)
				return processTransaction(std::shared_ptr<Transaction>(new Deposit(USDollar(d)))) == 0;
			}
			USDollar sendTransfer(double d) //transfers money
			{
				//simple for right now, logic for special functionality related to transfers will be here
				USDollar dollar = USDollar(d); //makes dollar amount
				processTransaction(std::shared_ptr<Transaction>(new Transfer(-dollar, ID))); //create the transfer
				return dollar;
			}
			bool receiveTransfer(USDollar d, std::string id) //recieves transfered money
			{
				//transfer recieve, success is 0
				return processTransaction(std::shared_ptr<Transaction>(new Transfer(USDollar(d), id))) == 0;
			}

			bool purchase(double d, std::string name, std::string origin) //handles purchase
			{
				//purchase success is 0
				return processTransaction(std::shared_ptr<Transaction>(new Purchase(USDollar(d), name, origin))) == 0;
			}

			int processTransaction(std::shared_ptr<Transaction> t) //underlying method for processing transactions (int return code for what happened to the transaction)
			{
				//very simple for right now
				int i = 1; //failure code is 1
				if (Transactions.put(t))
				{
					i = 0; //success code is 0
					updateBalance(); //update the balance/available
				}

				return i; //return code
			}

			std::string getType() //returns account type
			{
				return "Checking";
			}

	};

	/// <summary>
	/// Certificates of Deposit account
	/// </summary>
	class CertOfDep : public Account
	{
	public:
		CertOfDep(Transaction* t, std::string id) : Account(t, id) {}
		~CertOfDep() {}

		std::chrono::system_clock::time_point EndOfTerm;

		void specialFunctions() //handles special functions (Savings + ensuring term)
		{
		}
		bool deposit(double d) //deposits money
		{
			//pretty verbose line here, let's work backwards
			//I make a new dollar amount with the double, pass that to the new Transaction, which gets put into a smart pointer, which is then processed (success is 0)
			return processTransaction(std::shared_ptr<Transaction>(new Deposit(USDollar(d)))) == 0;
		}
		USDollar sendTransfer(double d) //transfers money
		{
			//simple for right now, logic for special functionality related to transfers will be here
			USDollar dollar = USDollar(d); //makes dollar amount
			processTransaction(std::shared_ptr<Transaction>(new Transfer(-dollar, ID))); //create the transfer
			return dollar;
		}
		bool receiveTransfer(USDollar d, std::string id) //recieves transfered money
		{
			//transfer recieve, success is 0
			return processTransaction(std::shared_ptr<Transaction>(new Transfer(USDollar(d), id))) == 0;
		}

		bool purchase(double d, std::string name, std::string origin) //handles purchase
		{
			//purchase success is 0
			return processTransaction(std::shared_ptr<Transaction>(new Purchase(USDollar(d), name, origin))) == 0;
		}

		int processTransaction(std::shared_ptr<Transaction> t) //underlying method for processing transactions (int return code for what happened to the transaction)
		{
			//very simple for right now
			int i = 1; //failure code is 1
			if (Transactions.put(t))
			{
				i = 0; //success code is 0
				updateBalance(); //update the balance/available
			}

			return i; //return code
		}

		std::string getType() //returns account type
		{
			return "Certificate of Deposit";
		}
	};

	/// <summary>
	/// Money Market account
	/// </summary>
	class MoneyMarket : public Account
	{
	public:
		MoneyMarket(Transaction* t, std::string id) : Account(t, id) {}
		~MoneyMarket() {}
		void specialFunctions() //handles special functions (has needs of both checkings & savings, and its own needs)
		{
		}
		bool deposit(double d) //deposits money
		{
			//pretty verbose line here, let's work backwards
			//I make a new dollar amount with the double, pass that to the new Transaction, which gets put into a smart pointer, which is then processed (success is 0)
			return processTransaction(std::shared_ptr<Transaction>(new Deposit(USDollar(d)))) == 0;
		}
		USDollar sendTransfer(double d) //transfers money
		{
			//simple for right now, logic for special functionality related to transfers will be here
			USDollar dollar = USDollar(d); //makes dollar amount
			processTransaction(std::shared_ptr<Transaction>(new Transfer(-dollar, ID))); //create the transfer
			return dollar;
		}
		bool receiveTransfer(USDollar d, std::string id) //recieves transfered money
		{
			//transfer recieve, success is 0
			return processTransaction(std::shared_ptr<Transaction>(new Transfer(USDollar(d), id))) == 0;
		}

		bool purchase(double d, std::string name, std::string origin) //handles purchase
		{
			//purchase success is 0
			return processTransaction(std::shared_ptr<Transaction>(new Purchase(USDollar(d), name, origin))) == 0;
		}

		int processTransaction(std::shared_ptr<Transaction> t) //underlying method for processing transactions (int return code for what happened to the transaction)
		{
			//very simple for right now
			int i = 1; //failure code is 1
			if (Transactions.put(t))
			{
				i = 0; //success code is 0
				updateBalance(); //update the balance/available
			}

			return i; //return code
		}

		std::string getType() //returns account type
		{
			return "Money Market";
		}
	};

	/// <summary>
	/// Customer User; has accounts associated via IDs in Linked List (primary as first id)
	/// </summary>
	class Customer : public User
	{
		public:
			Customer(std::string s, std::string pass) : User(s,pass) {}
			~Customer() {}
			LinkedList<std::string> AccountIDs; //accounts owned/accessible by user

			//Transfer between accounts; int for return code. Customers need to own/have access to account
			int transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v);

			//Customer deposit logic
			int deposit(std::shared_ptr<Database> d, std::string acc, double v);

	};

	/// <summary>
	/// Employee User 
	/// </summary>
	class Employee : public User
	{
		public:
			Employee(std::string s, std::string pass) : User(s,pass) {}
			~Employee() {}

			//Transfer between accounts; int for return code. Employees don't care about account ownership
			int transfer(std::shared_ptr<Database> d, std::string acc1, std::string acc2, double v);

			int deposit(std::shared_ptr<Database> d, std::string acc, double v);
	};

	/// <summary>
	/// contains static functions for overdraft
	/// </summary>
	class Overdraft
	{
		public:
			/// <summary>
			/// Handles overdraft for a specific user
			/// </summary>
			/// <param name="c">Customer shared pointer</param>
			/// <returns>was successful?, bool</returns>
			static bool OnPurchase(std::shared_ptr<Customer> cust, std::shared_ptr<Database> d);
	};

	/// <summary>
	/// contains static functions for interest
	/// </summary>
	class Interest
	{
		public:
			
			/// <summary>
			/// payout function, used to simplify code 
			/// </summary>
			/// <param name="acc">account to do interest on</param>
			static void payout(std::shared_ptr<Account> acc, double rate, double ratio)
			{
				USDollar pay(rate * ratio); //get pay from rate * ratio
				if (pay < 1) return; //if pay is 0, just stop
				std::shared_ptr<BankFunction> trans(new BankFunction(pay, "Interest payout")); //create new transaction
				acc->processTransaction(trans); //send new transaction to account
				acc->LastPayout = std::chrono::system_clock::now(); //reset last payout to now
			}
			
			
			/// <summary>
			/// handles interest per account
			/// </summary>
			/// <param name="acc">account to have interest updated</param>
			static void IndividualAccount(std::shared_ptr<Account> acc)
			{
				//if account exists & is not a null pointer
				if (acc)
				{
					//get values for ease of use from here on
					int t = acc->interestType;
					int p = acc->payoutRate;

					//get time values in hours
					int interestTime = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now() - acc->LastInterest).count();
					int payoutTime = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now() - acc->LastPayout).count();
					//hours comparison value; payoutRate will change that
					int payoutComparison = 8760;
					if (p == 1) payoutComparison = 4320;
					if (p == 2) payoutComparison = 720;
					if (p == 3) payoutComparison = 24;

					double rate = acc->APY;
					double adjustedRate = rate;
					double ratio = 1;

					//what type of interest?
					switch (t)
					{
						case 1 : case 2:
							//compound interest, yearly
							ratio = payoutComparison / 8760.00;
							if (interestTime > 8760)
							{
								acc->interestSoFar = acc->interestSoFar + acc->balance.GetPercentage(adjustedRate);
								acc->LastInterest = std::chrono::system_clock::now();
							}
							break;
						case 3:
							//compound interest, monthly
							adjustedRate = rate / 12;
							ratio = payoutComparison / 720.00;
							if (interestTime > 720)
							{
								acc->interestSoFar = acc->interestSoFar + acc->balance.GetPercentage(adjustedRate);
								acc->LastInterest = std::chrono::system_clock::now();
							}
							break;
						case 4:
							//compound interest, daily
							adjustedRate = rate / 365;
							ratio = payoutComparison / 24.00;
							if (interestTime > 24)
							{
								acc->interestSoFar = acc->interestSoFar + acc->balance.GetPercentage(adjustedRate);
								acc->LastInterest = std::chrono::system_clock::now();
							}
							break;
						default:
							//do nothing by default, or no interest
							break;
					}

					//if payout is greater than comparison value & not a certificate of deposit, payout
					if (payoutTime > payoutComparison && acc->getType() != "Certificate of Deposit")
					{
						//use payout static function
						payout(acc, adjustedRate, ratio);
					}
				}
			}

			/// <summary>
			/// Goes through & handles interest for all accounts
			/// </summary>
			/// <param name="accs">list of accounts to go through</param>
			static void AllAccounts(LinkedList<Account> accs)
			{
				for (int i = 0; i < accs.getCount(); i++)
				{
					IndividualAccount(accs.get(i));
				}
			}
	};

	/// <summary>
	/// Database class
	/// </summary>
	class Database
	{
	public:
		Database() {
			//default employee
			std::shared_ptr<Employee> e(new Employee("TotallyNotAnAdmin", "ao2j4ona5rorn2"));

			Customers = LinkedList<Customer>();
			Employees = LinkedList<Employee>(e); //put default employee into employees
			Accounts = LinkedList<Account>();
			EncryptionKeys = LinkedList<std::string>();
		}
		~Database() {}
		LinkedList<Customer> Customers; //customers, main users
		LinkedList<Employee> Employees; //administrators, essentially
		LinkedList<Account> Accounts; //all accounts, split from customers
		LinkedList<std::string> EncryptionKeys; //encryption keys (not yet used)

		/// <summary>
		/// purchase request
		/// </summary>
		/// <param name="acc">account identifier string</param>
		/// <param name="user">user identifer string</param>
		/// <returns>was successful, bool</returns>
		bool purchase(std::string acc, std::string user, std::string name = "Purchase", std::string origin = "Unknown")
		{
			std::shared_ptr<Customer> cust = Customers.get(user);
			Overdraft::OnPurchase(cust, std::shared_ptr<Database>(this));
		}

		/// <summary>
		/// bank processes done at a regular interval
		/// </summary>
		void bankProcesses()
		{
			Interest::AllAccounts(Accounts);
		}
	};

}