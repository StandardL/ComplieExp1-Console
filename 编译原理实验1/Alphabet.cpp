#include "Alphabet.h"
using namespace std;

Alphabet::Alphabet()
{
	//set<string> initset{"auto", "bool", "break", "case"}
}

Alphabet::~Alphabet()
{
}

bool Alphabet::GetToken(std::string input)
{
	double num = 0;
	int pos = 0, nowpos = 0;
	int len = input.length();
	while (pos < len)
	{
		Token token;  // 临时存储token
		while (input[pos] == 32 || input[pos] == 9)  // 跳过空格和制表符
			pos++;

		nowpos = pos;
		if ((input[nowpos] >= '0') && (input[nowpos] <= '9'))  // 数字
		{
			nowpos = pos;
			string numstr = "";
			while ((input[nowpos] >= '0') && (input[nowpos] <= '9'))  // 整数
			{
				numstr += input[nowpos];
				nowpos++;
			}
			if (input[nowpos] == '.')  // 小数
			{
				numstr += input[nowpos];
				nowpos++;
				while ((input[nowpos] >= '0') && (input[nowpos] <= '9'))
				{
					numstr += input[nowpos];
					nowpos++;
				}
			}
			token.ID = TokenType::Number;
			token.value = numstr;
			pos = nowpos;
			tokens.push_back(token);
		}
		else if (isalpha(input[pos]) || input[pos] == '_')  // 判断变量
		{
			string str = "";
			while (isalpha(input[pos]) || isdigit(input[pos]) || input[pos] == '_')
			{
				str += input[pos];
				pos++;
			}
			token.ID = TokenType::Var;
			token.value = str;
			tokens.push_back(token);
		}
		else if (input[pos] != '\n')  // 判断符号
		{
			switch (input[pos])
			{
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
			case '=':
				if (judgeComplex(input, pos))
				{
					if (input[pos] == '=')  // 如果是等号，两个等号出现说明是关系运算符
						token.ID = TokenType::Relation;
					else if ((input[pos] == '+' || input[pos] == '-') && input[pos + 1] == '=')
						token.ID = TokenType::Assignment;
					else if (input[pos] == '*' || input[pos] == '/' || input[pos] == '%')
						token.ID = TokenType::Arithmetic;
					else
						token.ID = TokenType::Arithmetic;
					token.value = input.substr(pos, 2);
					tokens.push_back(token);
					pos++;
					break;
				}
				token.ID = TokenType::Arithmetic;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			case '&':
			case '|':
			case '!':
				if (judgeComplex(input, pos))
				{
					if (input[pos] == '!')
						token.ID = TokenType::Relation;
					else if ((input[pos] == '&' || input[pos] == '|') && input[pos + 1] == '=')
						token.ID = TokenType::Assignment;
					else
						token.ID = TokenType::Logic;
					token.value = input.substr(pos, 2);
					tokens.push_back(token);
					pos++;
					break;
				}
			case '<':
			case '>':
				if (judgeComplex(input, pos))
				{
					if ((input[pos] == '<' || input[pos] == '>') && input[pos + 1] == '=')
						token.ID = TokenType::Relation;
					else
						token.ID = TokenType::Bitwise;
					token.value = input.substr(pos, 2);
					tokens.push_back(token);
					pos++;
					break;
				}
				token.ID = TokenType::Relation;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			case '^':
			case '~':
				token.ID = TokenType::Bitwise;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			case '{':
			case '}':
			case '(':
			case ')':
			case '[':
			case ']':
			case ',':
			case ';':
				token.ID = TokenType::Division;
				token.value = input[pos];
				tokens.push_back(token);
				break;

			default:
				break;
			}

			pos++;
		}
		else token.ID = TokenType::Endinput;
	}

	return true;
}

void Alphabet::PrintToken()
{
	for (auto& token : tokens)
	{
		if (token.ID == TokenType::Keyword) cout << token.value << ": " << "关键字" << endl;
		else if (token.ID == TokenType::Var) cout << token.value << ": " << "变量" << endl;
		else if (token.ID == TokenType::String) cout << token.value << ": " << "字符串" << endl;
		else if (token.ID == TokenType::Number) cout << token.value << ": " << "数字" << endl;
		else if (token.ID == TokenType::Arithmetic) cout << token.value << ": " << "算术运算符" << endl;
		else if (token.ID == TokenType::Logic) cout << token.value << ": " << "逻辑运算符" << endl;
		else if (token.ID == TokenType::Relation) cout << token.value << ": " << "关系运算符" << endl;
		else if (token.ID == TokenType::Bitwise) cout << token.value << ": " << "位运算符" << endl;
		else if (token.ID == TokenType::Assignment) cout << token.value << ": " << "赋值运算符" << endl;
		else if (token.ID == TokenType::Others) cout << token.value << ": " << "杂项运算符" << endl;
		else if (token.ID == TokenType::Comment) cout << token.value << ": " << "注释" << endl;
		else if (token.ID == TokenType::Division) cout << token.value << ": " << "分割符" << endl;
		else if (token.ID == TokenType::Special) cout << token.value << ": " << "特殊符号" << endl;
		else if (token.ID == TokenType::Endinput) return;
	}
}

bool Alphabet::judgeComplex(std::string input, int pos)
{
	int nextpos = pos + 1;
	if (nextpos >= input.length()) return false;
	if (input[pos] == '+')
	{
		switch (input[nextpos])
		{
		case '=':
		case '+':
			return true;
		default:
			return false;
		}
	}
	if (input[pos] == '-')
	{
		switch (input[nextpos])
		{
		case '=':
		case '-':
		case '>':
			return true;
		default:
			return false;
		}
	}
	if (input[pos] == '*' || input[pos] == '/' || input[pos] == '%' || input[pos] == '!' || input[pos] == '=')
	{
		if (input[nextpos] == '=')
			return true;
		return false;
	}
	if (input[pos] == '<')
	{
		switch (input[nextpos])
		{
		case '<':
		case '=':
			return true;
		default:
			return false;
		}
	}
	if (input[pos] == '>')
	{
		switch (input[nextpos])
		{
		case '>':
		case '=':
			return true;
		default:
			return false;
		}
	}
	if (input[pos] == '&' || input[pos] == '|')
	{
		if (input[nextpos] == input[pos])
			return true;
		return false;
	}
}
