#include"IncludeAndDef.h"

extern vector<Token>tokenstream;
extern vector<Token>::iterator ite;

bool ReadE();
bool ReadT();
bool ReadF();
bool ReadA();
bool ReadB();
bool ReadC();
bool ReadD();

int check_token(TOKENTYPE t, unsigned v = INVALID)
{
	if (ite -> type != t) return FALSE;
	else
	{
		if (ite->type == NUM) return TRUE;
		if (ite->value != v) return FALSE;
	}
	return TRUE;
}

bool ReadS()
{
	return ReadE();
}

bool ReadE()
{
	if (ReadT())
	{
		if (ReadB())
		{
			cout << "Do E->TB" << endl;
			return TRUE;
		}
		return FALSE;
	}
	else return FALSE;
}
bool ReadT()
{
	if (ReadF())
	{
		if (ReadD())
		{
			cout << "Do T->FD" << endl;
			return TRUE;
		}
		return FALSE;
	}
	else return FALSE;
}
bool ReadF()
{
	if (check_token(OPE, LBRAC))
	{
		ite++;
		if (ReadE())
		{
			if (check_token(OPE, RBRAC))
			{
				ite++;
				cout << "Do  F->(E)" << endl;
				return TRUE;
			}
		}
		return FALSE;
	}
	if (check_token(NUM))
	{
		ite++;
		cout << "Do F->num" << endl;
		return TRUE;
	}
	return FALSE;
}
bool ReadA()
{
	if (check_token(OPE, PLUS) || check_token(OPE, MINUS))
	{
		ite++;
		if (ReadT())
		{
			cout << "Do A-> +or- T" << endl;
			return TRUE;
		}
	}
	return FALSE;
}
bool ReadB()
{
	if (ite == tokenstream.end())
	{
		cout << "Do B->NULL" << endl;
		return TRUE;
	}
	if (check_token(OPE, PLUS) || check_token(OPE, MINUS))
	{
		if (ReadA())
		{
			if (ReadB())
			{
				cout << "Do B->AB" << endl;
				return TRUE;
			}
		}
		return FALSE;
	}
	cout << "Do B->NULL" << endl;
	return TRUE;
}
bool ReadC()
{
	if (check_token(OPE, MUL) || check_token(DIV))
	{
		ite++;
		if (ReadF())
		{
			cout << "Do C-> *or/ F " << endl;
			return TRUE;
		}
	}
	return FALSE;
}
bool ReadD()
{
	if (ite == tokenstream.end())
	{
		cout << "Do D->NULL" << endl;
		return TRUE;
	}
	if (check_token(OPE, MUL) || check_token(DIV))
	{
		if (ReadC())
		{
			if (ReadD())
			{
				cout << "Do D->CD" << endl;
				return TRUE;
			}
		}
		return FALSE;
	}
	/*if (check_token(OPE, PLUS) || check_token(OPE, MINUS))
	{
		cout << "Do D->NULL" << endl;
		return TRUE;
	}*/
	cout << "Do D->NULL" << endl;
	return TRUE;
}