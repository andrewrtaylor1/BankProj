#include "BankDB.h"
#include "BankServer.h"
#include "BankClient.h"

int main()
{
	Client::Client c;
	c.currentMenu->logic(); //currently just showing that the virtual functions are working
	return 0;
}