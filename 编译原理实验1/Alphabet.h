#pragma once
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>

enum class TokenType
{
	Keyword,  // 关键字
	Var, String, Number,   // 变量（标识符），字符串，数字
	Arithmetic, Logic, Relation, Bitwise, Assignment, Others,  // 算术、逻辑、关系、位运算、赋值、杂项运算符
	Comment,  // 注释
	Division,  // 分割符
	Special,  // 特殊符号
	Endinput, // 结束符
};

struct Token
{
	TokenType ID;
	std::string value;
};

class Alphabet
{
public:
	Alphabet();
	~Alphabet();
	std::vector<Token> tokens;  // 分割后的字符串
	bool GetToken(std::string);  // 读入字符串的准备工作
	void PrintToken();  // 打印分割后的字符串
private:
	std::set<std::string> keywords;  // 保留的关键字
	bool judgeComplex(std::string, int pos);  // 判断是否为复合运算符
};

