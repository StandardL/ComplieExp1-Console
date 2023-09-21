#include "Alphabet.h"

using namespace std;

int main()
{
	Alphabet alphabet;
	string input;

	while (getline(cin, input)) // 读取一行算数表达式
	{
		if (input == "ENDINPUT") break;
		alphabet.GetToken(input); // 读取所有单词
	}

	alphabet.PrintToken();
	return 0;
}