// Interface class header file with declared variables and functions
#pragma once
#include <string>
#include <map>
#include <set>
using namespace std;

class Interface {
	public:
		Interface();
		void display();
		void encode();
		void decode();
	private:
		string userInput;
		string file;
};