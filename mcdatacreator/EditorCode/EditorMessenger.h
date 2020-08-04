#pragma once

//https://www.geeksforgeeks.org/how-to-implement-our-own-vector-class-in-c/

#include <stdio.h>

#include <string>
#include <vector>
#include <tuple>

typedef std::string Value;
typedef std::string Sender;

// Value, Sender
typedef std::pair<Value, Sender> Message;

#define MESSENGER_VERBOSITY 0

//namespace MyMessenger
//{
	class Messenger
	{
	private:
		Messenger()
		{
#if MESSENGER_VERBOSITY == 1
			printf("Creating new Messenger!");
#endif
		}
		static std::vector<Message> messages;

	public:
		~Messenger()
		{

		}

		static void push(std::string value, std::string sender)
		{
			messages.push_back(Message(value, sender));
		}

		static Message get(size_t index)
		{
			return messages[index];
		}

		// function to delete last element 
		static void pop()
		{
			messages.pop_back();
		}

		static size_t size()
		{
			return messages.size();
		}

		static Message back()
		{
			if (messages.size() > 0)
				return messages.back();
			return Message("", "");
		}

		static bool eat(std::string value)
		{
			if (back().first == value)
			{
				pop();
				return true;
			}
			return false;
		}

		static void leftovers()
		{
#if MESSENGER_VERBOSITY == 1
			if (size() > 1)
			{
				throw "did not eat all messages!\n"; // because this is pretty harmful for the program's (already attrocious) memory usage
			}
#endif
		}

	};

	std::vector<Message> Messenger::messages = std::vector<Message>();

//}
