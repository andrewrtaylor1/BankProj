#include "pch.h"
#include "../Src/header/List.h"
#include "../Src/header/BankDB.h"

//LinkedList initialization
TEST(LinkedList, ListInit)
{
	LinkedList < std::string> ll; //make an empty list
	EXPECT_EQ(ll.getCount(),0); //check if count is 0
	LinkedList<std::string> ll2(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_EQ(ll2.getCount(), 1); //check if count is 1
}

//LinkedList put function
TEST(LinkedList, ListPut)
{
	LinkedList < std::string> ll; //make an empty list
	EXPECT_EQ(ll.getCount(), 0); //check if count is 0
	EXPECT_TRUE(ll.put(std::shared_ptr<std::string>(new std::string("string")))); //put a new string in, check if it worked
	EXPECT_EQ(ll.getCount(), 1); //check if count is 1
}

//LinkedList basic get function
TEST(LinkedList, ListGet)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_EQ(*ll.get(0), "string"); //dereference pointer to get string back, check if string is equivalent
}

//LinkedList find function
TEST(LinkedList, ListFind)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_EQ(ll.find("string"), 0); //try to find entry for string, should equal 0 
}

//LinkedList exist function
TEST(LinkedList, ListExist)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_TRUE(ll.exists(0)); //try to find if entry exists
}


//LinkedList drop function
TEST(LinkedList, ListDrop)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_TRUE(ll.drop(0)); //drop should work
	EXPECT_EQ(ll.getCount(), 0); //drop should lead to 0 count
}


//LinkedList update function
TEST(LinkedList, ListUpdate)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	EXPECT_TRUE(ll.update(0, std::shared_ptr<std::string>(new std::string("string2")))); //update function, should be true
	EXPECT_EQ(*ll.get(0), "string2"); //dereference pointer to get string back, see if theupdate worked
}

//functions making use of other functions, like get with string input (find leads to get)
TEST(LinkedList, ListCompoundFunctions)
{
	LinkedList<std::string> ll(std::shared_ptr<std::string>(new std::string("string"))); //make a list with 1 entry
	//next functions will seem a bit silly Usefulness-wise, but we do also use LinkedLists for more complex classes
	EXPECT_EQ(*ll.get("string"), "string"); //check get works
	EXPECT_TRUE(ll.update("string", std::shared_ptr<std::string>(new std::string("string2")))); //check update works
	EXPECT_TRUE(ll.drop("string2")); //check that drop works

}

//testing for proper exception handling
TEST(LinkedList, LLExceptions)
{
	LinkedList < std::string> ll; //make an empty list
	EXPECT_EQ(ll.getCount(), 0); //check if count is 0
	testing::internal::CaptureStdout(); //capture output until we grab output
	EXPECT_FALSE(ll.get(0)); //null = false
	std::string emptyOut = testing::internal::GetCapturedStdout(); //grab output
	EXPECT_TRUE(ll.put(std::shared_ptr<std::string>(new std::string("string")))); //put a new string in, check if it worked
	EXPECT_EQ(ll.getCount(), 1); //check if count is 1
	testing::internal::CaptureStdout();  //capture output until we grab output
	EXPECT_FALSE(ll.get(1)); //null = false
	EXPECT_TRUE(ll.get(0)); //real = true
	std::string oneOut = testing::internal::GetCapturedStdout(); //grab output
	EXPECT_TRUE(ll.put(std::shared_ptr<std::string>(new std::string("string2")))); //put a new string in, check if it worked
	EXPECT_EQ(ll.getCount(), 2); //check if count is 2
	testing::internal::CaptureStdout(); //capture output until we grab output
	EXPECT_FALSE(ll.get(2)); //null = false
	EXPECT_TRUE(ll.get(1)); //real = true
	EXPECT_TRUE(ll.get(0)); //real = true
	std::string twoOut = testing::internal::GetCapturedStdout(); //grab output
	testing::internal::CaptureStdout();  //capture output until we grab output
	EXPECT_FALSE(ll.get(-1)); //null = false
	std::string negatOut = testing::internal::GetCapturedStdout(); //grab output
	testing::internal::CaptureStdout();  //capture output until we grab output
	EXPECT_FALSE(ll.drop(-1)); //should fail and call exception
	std::string dropOut = testing::internal::GetCapturedStdout(); //grab output
	testing::internal::CaptureStdout();  //capture output until we grab output
	EXPECT_FALSE(ll.update(-1, std::shared_ptr<std::string>())); //should fail and call exception
	std::string updateOut = testing::internal::GetCapturedStdout(); //grab output
	//check outputs all did out of bounds index
	EXPECT_EQ(emptyOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::get\n");
	EXPECT_EQ(oneOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::get\n");
	EXPECT_EQ(twoOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::get\n");
	EXPECT_EQ(emptyOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::get\n");
	EXPECT_EQ(dropOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::drop\n");
	EXPECT_EQ(updateOut, "Attempted to access out of bounds index in LinkedList, while executing function: LinkedList::update\n");
}

namespace DB {
	//initialization test
	TEST(SavingsTest, SavingsInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		Saving s(t, "s0001"); //initialize
		EXPECT_EQ(s.Transactions.get(0)->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(SavingsTest, SavingsPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Savings"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(SavingsTest, SavingsTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed the transaction in
		EXPECT_EQ(a->Transactions.get(1)->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(SavingsTest, SavingsDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(SavingsTest, SavingsPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(SavingsTest, SavingsTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(SavingsTest, SavingsTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Saving(t, "s0001")); //init 1
		std::shared_ptr<Account> a1(new Saving(t1, "s0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01),a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(CheckingTest, CheckingInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		Checking c(t, "0001"); //initialize
		EXPECT_EQ(c.Transactions.get(0)->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(CheckingTest, CheckingPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Checking"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(CheckingTest, CheckingTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed that transaction in
		EXPECT_EQ(a->Transactions.get(1)->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(CheckingTest, CheckingDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(CheckingTest, CheckingPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(CheckingTest, CheckingTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(CheckingTest, CheckingTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new Checking(t, "c0001")); //init 1
		std::shared_ptr<Account> a1(new Checking(t1, "c0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(CoDTest, CoDInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		CertOfDep cd(t, "cd0001"); //initialize
		EXPECT_EQ(cd.Transactions.get(0)->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(CoDTest, CoDPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Certificate of Deposit"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(CoDTest, CoDTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed thet transaction in
		EXPECT_EQ(a->Transactions.get(1)->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(CoDTest, CoDDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(CoDTest, CoDPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(CoDTest, CoDTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(CoDTest, CoDTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new CertOfDep(t, "cd0001")); //init 1
		std::shared_ptr<Account> a1(new CertOfDep(t1, "cd0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}

	//initialization test
	TEST(MMTest, MMInit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		MoneyMarket mm(t, "mm0001"); //initialize
		EXPECT_EQ(mm.Transactions.get(0).get()->Val, 1); //test that transaction list is working; 1 cent in initialization, 1 cent in outcome
	}

	//test just polymorphic call
	TEST(MMTest, MMPolymorph)
	{
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //initialize;pointer of type "account" to test polymorphism
		EXPECT_EQ(a->getType(), "Money Market"); //type information is accessed polymorphically, should return a string "Savings"
	}

	//transaction
	TEST(MMTest, MMTransaction) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //initialize
		std::shared_ptr<Transaction> t1(new BankFunction(USDollar(10))); //new transaction
		a->processTransaction(t1); //feed the transaction in
		EXPECT_EQ(a->Transactions.get(1)->Val, 10); //new entry in transaction list, on top of the first one; should give 10 cents
		EXPECT_EQ(a->balance, 11); //balance should successfully add the entries together
		EXPECT_EQ(a->available, 11); //ditto with above, available should match balance
	}

	//deposit
	TEST(MMTest, MMDeposit) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1)));
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //initialize
		EXPECT_TRUE(a->deposit(10.01)); //deposit 10 bucks & 1 cent, should return true
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //check for 2nd transaction as above
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + 10.01, 10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + 10.01, 10.02
	}

	//purchase
	TEST(MMTest, MMPurchase) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //initialize
		EXPECT_TRUE(a->purchase(-10.01, "Test Purchase", "Test Company")); //buy something that's 10 bucks & 1 cent
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending transfer
	TEST(MMTest, MMTransferFrom) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //initialize
		EXPECT_EQ(a->sendTransfer(10.01), USDollar(1001)); //Transfer 10.01 dollars
		EXPECT_EQ(a->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a->available, 98999); //$989.99 remaining in available
	}

	//sending & recieving transfer
	TEST(MMTest, MMTransferTo) {
		std::shared_ptr<Transaction> t(new Deposit(USDollar(1))); //1 cent
		std::shared_ptr<Transaction> t1(new Deposit(USDollar(100000))); //1000 dollars
		std::shared_ptr<Account> a(new MoneyMarket(t, "mm0001")); //init 1
		std::shared_ptr<Account> a1(new MoneyMarket(t1, "mm0002")); //init 2
		EXPECT_TRUE(a->receiveTransfer(a1->sendTransfer(10.01), a1->ID)); //send & recieve transfer
		EXPECT_EQ(a->Transactions.get(1)->Val, 1001); //get the additive transaction, $10.01
		EXPECT_EQ(a1->Transactions.get(1)->Val, -1001); //get the subtractive transaction, -$10.01
		EXPECT_EQ(a->balance, 1002); //balance should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a->available, 1002); //available should be 1 cent + $10.01, $10.02
		EXPECT_EQ(a1->balance, 98999); //$989.99 remaining in balance
		EXPECT_EQ(a1->available, 98999); //$989.99 remaining in available

	}
}