#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include<vector>
#include "func.h"

#define TOKENTYPE unsigned
#define INVALID -1

#define F_EOF -1
#define ERROR 0
#define OPE 1
#define NUM 2

#define PLUS 0
#define MINUS 1
#define MUL 2
#define DIV 3
#define LBRAC 4
#define RBRAC 5

#define LINE 0
#define ROW 1

using namespace std;

struct Token
{
	Token()
	{
		this->type = 0;
		this->value = 0;
		this->is_char_left = 0;
	}
	Token(TOKENTYPE t, int v, bool is_left = TRUE)
	{
		this->type = t;
		this->value = v;
		this->is_char_left = is_left;
	}
	TOKENTYPE type;
	int value;
	bool is_char_left;
};