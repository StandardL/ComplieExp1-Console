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

struct JudgingComplex
{
	TokenType ID;
	std::string value;
	int newpos;  // �������������һ��λ��
	JudgingComplex() {}
	JudgingComplex(TokenType id, std::string value, int np) : ID(id), value(value), newpos(np) {}
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
	JudgingComplex judgePlus(std::string, int pos);  // �Ӻ��Ƿ񸴺ϣ��ǵĻ����ظ��������
	JudgingComplex judgeMinus(std::string, int pos);  // �����Ƿ񸴺ϣ��ǵĻ����ظ��������
	JudgingComplex judgeMultiply(std::string, int pos);  // �˺��Ƿ񸴺ϣ��ǵĻ����ظ��������
	JudgingComplex judgeDivide(std::string, int pos);  // �����Ƿ񸴺ϣ��ǵĻ����ظ��������
	JudgingComplex judgeMod(std::string, int pos);  // ȡģ�Ƿ񸴺ϣ��ǵĻ����ظ��������

};

