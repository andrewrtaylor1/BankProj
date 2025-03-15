#include "pch.h"
#include "../Src/header/BankDB.h"


namespace DB {
	//initialization test
	TEST(SavingsTest, SavingsInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		Saving s(t.get(), "s0001"); //initialize
		EXPECT_EQ(s.Transactions.get(1).get()->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(SavingsTest, SavingsPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Savings"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(SavingsTest, SavingsTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed thet transaction in
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(SavingsTest, SavingsDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(SavingsTest, SavingsPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(SavingsTest, SavingsTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(SavingsTest, SavingsTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t.get(), "s0001")); //init 1
		std::shared_ptr<Account> a1(new Saving(t1.get(), "s0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01),a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(CheckingTest, CheckingInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		Checking c(t.get(), "0001"); //initialize
		EXPECT_EQ(c.Transactions.get(1).get()->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(CheckingTest, CheckingPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Checking"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(CheckingTest, CheckingTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed thet transaction in
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(CheckingTest, CheckingDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(CheckingTest, CheckingPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(CheckingTest, CheckingTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(CheckingTest, CheckingTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t.get(), "c0001")); //init 1
		std::shared_ptr<Account> a1(new Checking(t1.get(), "c0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(CoDTest, CoDInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		CertOfDep cd(t.get(), "cd0001"); //initialize
		EXPECT_EQ(cd.Transactions.get(1).get()->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(CoDTest, CoDPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Certificate of Deposit"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(CoDTest, CoDTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed thet transaction in
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(CoDTest, CoDDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(CoDTest, CoDPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(CoDTest, CoDTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(CoDTest, CoDTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t.get(), "cd0001")); //init 1
		std::shared_ptr<Account> a1(new CertOfDep(t1.get(), "cd0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(MMTest, MMInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		MoneyMarket mm(t.get(), "mm0001"); //initialize
		EXPECT_EQ(mm.Transactions.get(1).get()->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(MMTest, MMPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Money Market"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(MMTest, MMTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed thet transaction in
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(MMTest, MMDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(MMTest, MMPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(MMTest, MMTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(MMTest, MMTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t.get(), "mm0001")); //init 1
		std::shared_ptr<Account> a1(new MoneyMarket(t1.get(), "mm0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(2).get()->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(2).get()->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}
}