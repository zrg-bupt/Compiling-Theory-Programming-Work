#pragma once
#include "IncludeAndDef.h"

extern string src;
extern int read_ptr;
extern char cur_char;
extern string cur_token;
extern unsigned ptr_loc[2];

bool inputChar()
{
	if (read_ptr < src.size())
	{
		cur_char = src[read_ptr++];
		if (cur_char != '\n') ptr_loc[ROW]++;
		else
		{
			ptr_loc[LINE]++;
			ptr_loc[ROW] = 0;
		}
		return TRUE;
	}
	else
	{
		cur_char = '\0';
		return FALSE;
	}
}
void ptr_back()
{
	if (cur_char != '\n')
	{
		read_ptr--;
		ptr_loc[ROW]--;
	}
}

Token yylex()
{
    cur_token = "";
	while (inputChar() && cur_char == '\n');
	switch (cur_char)
	{
	case '+':
		return Token(OPE, PLUS);
	case '-':
		return Token(OPE, MINUS);
	case '*':
		return Token(OPE, MUL);
	case '/':
		return Token(OPE, DIV);
	case '(':
		return Token(OPE, LBRAC);
	case ')':
		return Token(OPE, RBRAC);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		while (cur_char >= '0' && cur_char <= '9')
		{
			cur_token = cur_token + cur_char;
			if (!inputChar())
			{
				return Token(NUM, stoi(cur_token), FALSE);
				break;
			}
		}
		if (cur_char != '\n') ptr_back();
		return Token(NUM, stoi(cur_token));
	case '\0':
		return Token(ERROR, INVALID, FALSE);
	default:
		return Token(ERROR, INVALID);
	}
}
