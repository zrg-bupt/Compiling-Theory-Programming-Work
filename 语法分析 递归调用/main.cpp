#include "IncludeAndDef.h"
#include "func.h"

string path;
string src;

vector<Token>tokenstream;
vector<Token>::iterator ite;

unsigned ptr_loc[2] = { 1,0 };

int read_ptr;
char cur_char;
string cur_token;

int main(int args, char* argv[])
{
	while (TRUE)
	{
		if (args == 1)
		{
			cout << "Please input the path of file being analyzed:" << endl;
			cin >> path;
			break;
		}
		else if (args == 2)
		{
			path = string(argv[1]);
			break;
		}
		else
			cout << "Too many parameters!" << endl;
	}
	
	load_file();
	//print_src();
	Token temp;
	while (temp=yylex(),temp.is_char_left)
	{
		if (temp.type != ERROR) tokenstream.push_back(temp);
		else
			cout << "An error detected in Line " << ptr_loc[LINE] << ", Row " << ptr_loc[ROW] << ", abort it... Analyzing continues..." << endl;
	}
	print_token();
	cout << endl;
	ite = tokenstream.begin();
	if (ReadS()) cout << "Analyzing Successful!" << endl;
	else cout << "Analyzing Failed... Some syntax goes wrong..." << endl;
	return 0;
}

void load_file()
{
	src = "";
	string temp;
	fstream reader;
	reader.open(path, ios::in);
	while (!reader.eof())
	{
		getline(reader, temp);
		src = src + temp + '\n';
	}
	reader.close();
}
void print_src()
{
	cout << "-----------------------------------------------------------" << endl;
	cout << src;
	cout << "-----------------------------------------------------------" << endl;
}
void print_token()
{
	fstream writer;
	writer.open("D:\\Users\\tokenstream.txt", ios::out);

	vector<Token>::iterator ite = tokenstream.begin();
	unsigned endl_count = 0;
	while (ite != tokenstream.end())
	{
		if (ite->type == OPE)
		{
			string temp;
			switch (ite->value)
			{
			case PLUS:
				temp = '+';
				break;
			case MINUS:
				temp = '-';
				break;
			case MUL:
				temp = '*';
				break;
			case DIV:
				temp = '/';
				break;
			case LBRAC:
				temp = '(';
				break;
			case RBRAC:
				temp = ')';
				break;
			}
			writer << temp;
		}
		else writer << ite->value;

		endl_count++;
		if (endl_count % 10==0)
		{
			endl_count = 0;
			writer << endl;
		}
		ite++;
	}
}