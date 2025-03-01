#pragma once

namespace Encrypt
{
	//Forward Declarations
	
	//Password Hashing
	class Hasher;
	//Base
	class Cryptography;
	//Derived
	class SingleKeyHandler;

	//meant to hash passwords & compare hashes
	class Hasher
	{
	};

	//base Cryptography class; virtual decrypt & encrypt functions, in case different encryption is needed in the future
	class Cryptography
	{
	};

	//Single key encryption, encrypts in and out on one key
	class SingleKeyHandler : public Cryptography
	{
	};

}
