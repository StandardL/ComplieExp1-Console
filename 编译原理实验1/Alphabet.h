#pragma once
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

enum class TokenType
{
	Keyword,  // 关键字
	Var, String, Char, Number,  // 变量（标识符），字符串，字符，数字
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

struct JudgingComplex
{
	TokenType ID;
	std::string value;
	int newpos;  // 复合运算符的下一个位置
	JudgingComplex() {}
	JudgingComplex(TokenType id, std::string value, int np) : ID(id), value(value), newpos(np) {}
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
	void ReadKeywords();  // 从文件中读取关键字到SET中
	bool judgeComplex(std::string, int pos);  // 判断是否为复合运算符
	JudgingComplex judgePlus(std::string, int pos);  // 加号是否复合，是的话返回复合运算符
	JudgingComplex judgeMinus(std::string, int pos);  // 减号是否复合，是的话返回复合运算符
	JudgingComplex judgeMultiply(std::string, int pos);  // 乘号是否复合，是的话返回复合运算符
	JudgingComplex judgeDivide(std::string, int pos);  // 除号是否复合，是的话返回复合运算符
	JudgingComplex judgeMod(std::string, int pos);  // 取模是否复合，是的话返回复合运算符
	JudgingComplex judgeEqual(std::string, int pos);  // 等号是否复合
	JudgingComplex judgeAnd(std::string, int pos);  // &
	JudgingComplex judgeOr(std::string, int pos);  // |
	JudgingComplex judgeNot(std::string, int pos);  // !
	JudgingComplex judgeLess(std::string, int pos);  // <
	JudgingComplex judgeGreat(std::string, int pos);  // >

	JudgingComplex dealwithSignNum(std::string, int pos);  //+-
};

