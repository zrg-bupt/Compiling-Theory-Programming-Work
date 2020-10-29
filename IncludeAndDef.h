#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<vector>

#define TRUE 1
#define FALSE 0

#define TERMIN 1
#define UNTERM 0

#define FIRST 0
#define FOLLOW 1

#define HEAD 0

#define EPSILON 'e'

#define SERIAL_TIAL g.serial.size()-1
using namespace std;

struct Symbol
{
	Symbol()
	{
		this->c = '\0';
		this->t = UNTERM;
	}
	Symbol(char c, bool t)
	{
		this->c = c;
		this->t = t;
	}
	char c;
	bool t;

	bool operator < (const Symbol& ope) const {
		return this->c < ope.c;
	}
	bool operator ==(const Symbol& ope) const {
		return (this->c == ope.c && this->t == ope.t);
	}
};
struct Grammar
{
	unsigned seq;
	Symbol init;
	vector<Symbol> serial;
};
