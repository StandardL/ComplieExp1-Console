#include "Alphabet.h"
using namespace std;

Alphabet::Alphabet()
{
	ReadKeywords();
}

Alphabet::~Alphabet()
{
}

bool Alphabet::GetToken(std::string input)
{
	double num = 0;
	int pos = 0, nowpos = 0;
	while (pos < input.length())
	{
		Token token;  // 临时存储token
		while (input[pos] == 32 || input[pos] == 9)  // 跳过空格和制表符
			pos++;

		nowpos = pos;
		if ((input[nowpos] >= '0') && (input[nowpos] <= '9'))  // 数字
		{
			auto res = dealwithSignNum(input, pos);
			token.ID = res.ID;
			token.value = res.value;
			pos = res.newpos;
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
			if (keywords.find(str) != keywords.end())  // 判断是否为关键字
				token.ID = TokenType::Keyword;
			else token.ID = TokenType::Var;
			token.value = str;
			tokens.push_back(token);
		}
		else if (!tokens.empty() && tokens.back().value == "include")  // 判断头文件
		{
			string hf;  // hf = headfile
			if (input[pos] == '<')  // <iostream>情况
				hf = input.substr(pos, input.find_first_of('>', pos) - pos + 1);
			else
				hf = input.substr(pos, input.find_first_of('\"', pos) - pos + 1);
			token.value = hf;
			token.ID = TokenType::Special;
			tokens.push_back(token);
			pos += hf.length();
		}
		else if (input[pos] != '\n')  // 判断符号
		{
			switch (input[pos])
			{
			case '+':
			{
				auto res = judgePlus(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '-':
			{
				auto res = judgeMinus(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '*':
			{
				auto res = judgeMultiply(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '/':
			{
				auto res = judgeDivide(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				// TODO 处理多行注释
				if (token.ID == TokenType::Comment && token.value == "/*")
				{
					pos = res.newpos + 1;
					while (true)
					{
						if (input[pos] == '*')  // 有机会结束
						{
							if (input[pos + 1] == '/')  // 真的可以结束
							{
								token.value += "*/";
								pos++;
								break;
							}
						}
						token.value += input[pos];
						pos++;
						// 如果已经到了这行的结尾，就需要再读一行新的
						if (pos >= input.length())
						{
							token.value += '\n';
							getline(cin, input);
							pos = 0;
						}
					}
				}
				else
				{
					token.value = input.substr(pos);
					pos = input.length();  // 表示这行读完了
				}
				tokens.push_back(token);

				break;
			}
			case '%':
			{
				auto res = judgeMod(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '=':
			{
				auto res = judgeEqual(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '&':
			{
				auto res = judgeAnd(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '|':
			{
				auto res = judgeOr(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '!':
			{
				auto res = judgeNot(input ,pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '<':
			{
				auto res = judgeLess(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '>':
			{
				auto res = judgeGreat(input, pos);
				token.ID = res.ID;
				token.value = res.value;
				tokens.push_back(token);
				pos = res.newpos;
				break;
			}
			case '~':
			{
				token.ID = TokenType::Bitwise;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			}
			case '^':
			{
				if (judgeComplex(input, pos))
				{
					token.ID = TokenType::Bitwise;
					token.value = input.substr(pos, 2);
					tokens.push_back(token);
					pos++;
					break;
				}
				token.ID = TokenType::Bitwise;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			}
			case '{':
			case '}':
			case '(':
			case ')':
			case ',':
			case ':':
			case ';':
			case '[':
			case ']':
				token.ID = TokenType::Division;
				token.value = input[pos];
				tokens.push_back(token);
				break;
			case '\'':
			{
				string str = input.substr(pos, 3);
				token.ID = TokenType::Char;
				token.value = str;
				tokens.push_back(token);
				pos += 2;
				break;
			}
			case '"':
			{
				auto endindex = input.find_first_of('\"', pos + 1);
				string str = input.substr(pos, endindex-pos+1);
				token.ID = TokenType::String;
				token.value = str;
				tokens.push_back(token);
				pos += str.length() - 1;
				break;
			}
			case '?':
			{
				auto endindex = input.find_first_of('?', pos+1);
				if (endindex != string::npos)
				{
					token.value = "?";
				}
				token.ID = TokenType::Others;
				token.value = "?:";
				tokens.push_back(token);
				break;
			}
			case ' ':
				break;
			default:
				token.ID = TokenType::Special;
				token.value = input[pos];
				tokens.push_back(token);
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
		if (token.ID == TokenType::Keyword) cout << "关键字" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::Var) cout << "标识符" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::String) cout << "字符串" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::Char) cout << "字符" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::Number) cout << "数字" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::Arithmetic) cout << "算术运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Logic) cout << "逻辑运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Relation) cout << "关系运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Bitwise) cout << "位运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Assignment) cout << "赋值运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Others) cout << "杂项运算符" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Comment) cout << "注释" << "\n" << token.value << endl;
		else if (token.ID == TokenType::Division) cout << "分割符" << "\t\t" << token.value << endl;
		else if (token.ID == TokenType::Special) cout << "特殊符号" << "\t" << token.value << endl;
		else if (token.ID == TokenType::Endinput) return;
	}
}

void Alphabet::ReadKeywords()
{
	ifstream csv_file("Keywords.csv", ios::in);
	if (!csv_file.is_open())
	{
		cout << "Error: Fail to open Keywords.csv." << endl;
		exit(1);
	}
	istringstream ss;
	string word, fields, line;

	while (getline(csv_file, line))
	{
		ss.clear();
		ss.str(line);
		word.clear();
		while (getline(ss, word, ','))
		{
			this->keywords.insert(word);
		}
	}	


	csv_file.close();
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
			if (input[nextpos] >= '0' && input[nextpos] <= '9')
				return true;
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
			if (input[nextpos] >= '0' && input[nextpos] <= '9')
				return true;
			return false;
		}
	}
	if (input[pos] == '*' || input[pos] == '%' || input[pos] == '!' || input[pos] == '=' || input[pos] == '^')
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
	if (input[pos] == '/')
	{
		if (input[nextpos] == '=' || input[nextpos] == '/' || input[nextpos] == '*')
			return true;
		return false;
	}
	return false;
}

JudgingComplex Alphabet::judgePlus(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		if (input[np] == '=')
			return JudgingComplex(TokenType::Assignment, "+=", np);
		if (input[np] == '+')
			return JudgingComplex(TokenType::Arithmetic, "++", np);
		if (input[np] >= '0' || input[np] <= '9')  // 处理正负号
		{
			auto res = dealwithSignNum(input, np);
			int nnp = res.newpos;
			string numstr = "+" + res.value;
			return JudgingComplex(TokenType::Number, numstr, nnp);
		}
	}
	return JudgingComplex(TokenType::Arithmetic, "+", pos);
}

JudgingComplex Alphabet::judgeMinus(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		if (input[np] == '=')
			return JudgingComplex(TokenType::Assignment, "-=", np);
		if (input[np] == '-')
			return JudgingComplex(TokenType::Arithmetic, "--", np);
		if (input[np] == '>')
			return JudgingComplex(TokenType::Others, "->", np);
		if (input[np] >= '0' || input[np] <= '9')  // 处理正负号
		{
			auto res = dealwithSignNum(input, np);
			int nnp = res.newpos;
			string numstr = "-" + res.value;
			return JudgingComplex(TokenType::Number, numstr, nnp);
		}
	}
	return JudgingComplex(TokenType::Arithmetic, "-", pos);
}

bool Alphabet::ResetToken()
{
	tokens.clear();
	return true;
}

JudgingComplex Alphabet::judgeMultiply(std::string input, int pos)
{
	if (judgeComplex(input, pos))
		return JudgingComplex(TokenType::Assignment, "*=", pos + 1);
	return JudgingComplex(TokenType::Arithmetic, "*", pos);
}

JudgingComplex Alphabet::judgeDivide(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		if (input[np] == '=')
			return JudgingComplex(TokenType::Assignment, "/=", np);
		else if (input[pos + 1] == '/')
			return JudgingComplex(TokenType::Comment, "//", np);
		else if (input[pos + 1] == '*')
			return JudgingComplex(TokenType::Comment, "/*", np);
	}
	return JudgingComplex(TokenType::Arithmetic, "/", pos);
}

JudgingComplex Alphabet::judgeMod(std::string input, int pos)
{
	if (judgeComplex(input, pos))
		return JudgingComplex(TokenType::Assignment, "%=", pos + 1);
	return JudgingComplex(TokenType::Arithmetic, "%", pos);
}

JudgingComplex Alphabet::judgeEqual(std::string input, int pos)
{
	if (judgeComplex(input, pos))
		return JudgingComplex(TokenType::Logic, "==", pos + 1);
	return JudgingComplex(TokenType::Assignment, "=", pos);
}

JudgingComplex Alphabet::judgeAnd(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		// 先判断是不是&&和&=的情况
		if (input[np] == '&')
			return JudgingComplex(TokenType::Logic, "&&", np);
		if (input[np] == '=')
			return JudgingComplex(TokenType::Assignment, "&=", np);
		// 再判断是否为引用符号, e.g.: &a
		while (input[np] == 32 || input[np] == 9)
			np++;
		if (isalpha(input[np]) || input[np] == '_')
		{
			string v;
			while(isalpha(input[np]) || input[np] == '_' || isdigit(input[np]))
				v += input[np];
			return JudgingComplex(TokenType::Others, v, np);
		}
	}
	return JudgingComplex(TokenType::Bitwise, "&", pos);
}

JudgingComplex Alphabet::judgeOr(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		// 判断是不是||和|=的情况
		if (input[np] == '|')
			return JudgingComplex(TokenType::Logic, "||", np);
		if (input[np] == '=')
			return JudgingComplex(TokenType::Assignment, "|=", np);
	}
	return JudgingComplex(TokenType::Bitwise, "|", pos);
}

JudgingComplex Alphabet::judgeNot(std::string input, int pos)
{
	if (judgeComplex(input, pos))
		return JudgingComplex(TokenType::Relation, "!=", pos + 1);
	return JudgingComplex(TokenType::Logic, "!", pos);
}

JudgingComplex Alphabet::judgeLess(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		if (input[np] == '=')
			return JudgingComplex(TokenType::Relation, "<=", pos);
		if (input[np] == '<')
		{
			// 判断是否是单纯的位移还是位移并赋值
			int nnp = np + 1;
			if (input[nnp] == '=')
				return JudgingComplex(TokenType::Assignment, "<<=", nnp);
			else
				return JudgingComplex(TokenType::Bitwise, "<<", np);
		}
	}
	return JudgingComplex(TokenType::Relation, "<", pos);
}

JudgingComplex Alphabet::judgeGreat(std::string input, int pos)
{
	int np = pos + 1;
	if (judgeComplex(input, pos))
	{
		if (input[np] == '=')
			return JudgingComplex(TokenType::Relation, ">=", pos);
		if (input[np] == '>')
		{
			// 判断是否是单纯的位移还是位移并赋值
			int nnp = np + 1;
			if (input[nnp] == '=')
				return JudgingComplex(TokenType::Assignment, ">>=", nnp);
			else
				return JudgingComplex(TokenType::Bitwise, ">>", np);
		}
	}
	return JudgingComplex(TokenType::Relation, ">", pos);
}

JudgingComplex Alphabet::dealwithSignNum(std::string input, int pos)
{
	int nowpos = pos;
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
	if (input[nowpos] == 'e' || input[nowpos] == 'E')  // e or E
	{
		numstr += input[nowpos];
		nowpos++;
		while (input[nowpos] == '+' || input[nowpos] == '-' || (input[nowpos] >= '0' && input[nowpos] <= '9'))
		{
			numstr += input[nowpos];
			nowpos++;
		}
	}
	
    return JudgingComplex(TokenType::Number, numstr, nowpos);
}
