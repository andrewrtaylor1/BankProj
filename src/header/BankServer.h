#pragma once

namespace Serv
{

	//server converts strings into events, which communicate with the database
	class Event
	{
		public:
			Event() {}
			~Event() {}
	};

	//contains listener, uses that to make Events
	class Server
	{
		public:
			Server(){}
			~Server(){}
	};

	//recieves strings
	class Listener
	{
		public:
			Listener(){}
			~Listener(){}
	};

	class SrvLogon : public Event
	{
		public:
			SrvLogon() {}
			~SrvLogon() {}
	};
	class SrvChangePassword : public Event
	{
		public:
			SrvChangePassword() {}
			~SrvChangePassword() {}
	};
	class SrvAccountTransfer : public Event
	{
		public:
			SrvAccountTransfer() {}
			~SrvAccountTransfer() {}
	};
	class SrvAccountChange : public Event
	{
		public:
			SrvAccountChange() {}
			~SrvAccountChange() {}
	};
	class SrvAccountCreate : public Event
	{
		public:
			SrvAccountCreate() {}
			~SrvAccountCreate() {}
	};
	class SrvAccountHistory : public Event
	{
		public:
			SrvAccountHistory() {}
			~SrvAccountHistory() {}
	};
	class SrvUserCreate : public Event
	{
		public:
			SrvUserCreate() {}
			~SrvUserCreate() {}
	};
}