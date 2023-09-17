#pragma once
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>

enum class TokenType
{
	Keyword,  // �ؼ���
	Var, String, Number,   // ��������ʶ�������ַ���������
	Arithmetic, Logic, Relation, Bitwise, Assignment, Others,  // �������߼�����ϵ��λ���㡢��ֵ�����������
	Comment,  // ע��
	Division,  // �ָ��
	Special,  // �������
	Endinput, // ������
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
	std::vector<Token> tokens;  // �ָ����ַ���
	bool GetToken(std::string);  // �����ַ�����׼������
	void PrintToken();  // ��ӡ�ָ����ַ���
private:
	std::set<std::string> keywords;  // �����Ĺؼ���
	bool judgeComplex(std::string, int pos);  // �ж��Ƿ�Ϊ���������
};

