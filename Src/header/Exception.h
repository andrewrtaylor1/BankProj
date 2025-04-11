#pragma once

#include <string>
#include <iostream>

/// <summary>
/// Exception base class, with a default printError function
/// </summary>
class Exception
{
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="s">throwing function</param>
		Exception(std::string s)
		{
			throwingFunc = s;
		}
		std::string throwingFunc; //function that threw the exception

		/// <summary>
		/// print the error to cout
		/// </summary>
		virtual void printError()
		{
			std::cout << "An error has occurred while running function: " << throwingFunc;
		}
};