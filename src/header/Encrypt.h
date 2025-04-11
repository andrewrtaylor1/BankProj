#pragma once

namespace Encrypt
{

	//meant to hash passwords & compare hashes (SHA-256)
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
