#include "BankDB.h"
#include "BankServer.h"
#include "BankClient.h"
#include <string>
#include <iostream>

int main()
{
	Client::MnuGetAccountHistory("TestEmployee", "s0001").logic();
	Client::MnuGetAccountHistory("TestUser2", "c0001").logic();
	return 0;
}