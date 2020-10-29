#include"IncludeAndDef.h"
//输入规则 非终结符用大写字母表示，终结符可用除大写字母外的字母表示
vector<Symbol> SymList;
set<Symbol> epsilonMark;
vector<Grammar> GramList;
map<Symbol, vector<Symbol>>Set[2];
map<Symbol, map<Symbol, unsigned>>AnalyzeTable;
Symbol S;

vector<Symbol> SymStream;
vector<Symbol> AnalyzeStack;

void loadGrammar()
{
	fstream reader;
	reader.open("D:\\Users\\testLL\\grammar.txt", ios::in);

	char read_char;
	Symbol temp_symbol;
	bool state = TERMIN, t = FALSE;
	while (TRUE)
	{
		reader >> read_char;
		if (read_char == ';' && state == TERMIN)
		{
			state = UNTERM;
			t = TRUE;
			continue;
		}
		else if (read_char == ';' && state == UNTERM) break;
		temp_symbol.c = read_char;
		temp_symbol.t = state;
		if (t)
		{
			S = temp_symbol;
			t = FALSE;
		}
		SymList.push_back(temp_symbol);
	}
	unsigned count = 0;
	while (TRUE)
	{
		Grammar temp_gram;
		reader >> read_char;
		if (read_char == '$')
		{
			reader.close();
			return;
		}
		temp_symbol.c = read_char;
		temp_symbol.t = UNTERM;
		temp_gram.init = temp_symbol;
		temp_gram.seq = count++;
		while (TRUE)
		{
			reader >> read_char;
			if (read_char == ';')
			{
				GramList.push_back(temp_gram);
				break;
			}
			temp_symbol.c = read_char;
			temp_symbol.t = (read_char >= 'A' && read_char <= 'Z') ? UNTERM : TERMIN;
			temp_gram.serial.push_back(temp_symbol);
		}
	}
}
void printGrammar()
{
	vector<Grammar>::iterator ite_g = GramList.begin();
	vector<Symbol>::iterator ite_s=SymList.begin();
	cout << "All Symbol are shown below:" << endl;
	while (ite_s != SymList.end())
	{
		cout << "Symbol: " << ite_s->c << " Type: " << ite_s->t << endl;
		ite_s++;
	}
	cout << "All grammar phrases are shown below:" << endl << "---------------------------------" << endl;
	while (ite_g != GramList.end())
	{
		cout << "No: " << ite_g->seq << '\t' << ite_g->init.c << " -> ";
		ite_s = ite_g->serial.begin();
		while (ite_s != ite_g->serial.end())
		{
			cout << ite_s->c;
			ite_s++;
		}

		cout << endl;
		ite_g++;
	}
	cout << "All done!" << "---------------------------------" << endl;
}

void initSet()
{
	Set[FIRST].clear();
	Set[FOLLOW].clear();
	vector<Symbol>::iterator ite_sym;
	for (ite_sym = SymList.begin(); ite_sym != SymList.end(); ite_sym++)
	{
		Set[FIRST][*ite_sym] = vector<Symbol>();
		Set[FIRST][*ite_sym].clear();
		if (ite_sym->t == TERMIN)
		{
			Set[FIRST][*ite_sym].push_back(*ite_sym);
			continue;
		}
		Set[FOLLOW][*ite_sym] = vector<Symbol>();
		Set[FOLLOW][*ite_sym].clear();
	}
}

bool conflateSet(Symbol a, Symbol b)
{
	vector<Symbol>::iterator iteA, iteB;
	bool is_add = FALSE, mark;
	for (iteB = Set[FOLLOW][b].begin(); iteB != Set[FOLLOW][b].end(); iteB++)
	{
		mark = FALSE;
		for (iteA = Set[FOLLOW][a].begin(); iteA != Set[FOLLOW][a].end(); iteA++)
		{
			if (*iteA == *iteB)
			{
				mark = TRUE;
				break;
			}
		}
		if (!mark)
		{
			Set[FOLLOW][a].push_back(*iteB);
			is_add = TRUE;
		}
	}
	return is_add;
}
bool conflateSet(const Symbol& a, const Grammar& g,vector<Symbol>::iterator cur_ite, const bool& mode)
{
	if (a == *cur_ite)
	{
		if (mode == FIRST && epsilonMark.find(a) != epsilonMark.end() && cur_ite + 1 != g.serial.end())
			return conflateSet(a, g, cur_ite + 1, FIRST);
		else if (mode == FOLLOW)
		{
			if (epsilonMark.find(a) != epsilonMark.end())
			{
				if (cur_ite + 1 != g.serial.end()) return conflateSet(a, g, cur_ite + 1, FOLLOW);
				else
				{
					if (g.init == a) return FALSE;
					else return conflateSet(a, g.init);
				}
			}
		}
		else return FALSE;
	}

	bool is_add = FALSE, mark;
	vector<Symbol>::iterator  iteA, iteB;
	for (iteB = Set[FIRST][*cur_ite].begin(); iteB != Set[FIRST][*cur_ite].end(); iteB++)
	{
		if (iteB->c == EPSILON)//如果FIRST集中有ε
		{
			if (cur_ite + 1 == g.serial.end())
			{
				if (mode == FIRST)
				{
					if (epsilonMark.find(a) != epsilonMark.end())	continue;
					Set[mode][a].push_back(Symbol(EPSILON, TERMIN));
					//cout << "Add epsilon to symbol: " << a.c << endl;
					epsilonMark.insert(a);
					return TRUE;
				}
				else
					is_add = (conflateSet(a, g.init) || is_add);
			}
			else
			{
				//cout << "Do this where Symbol a: " << a.c << ", next bit: " << cur_ite_plus->c << endl;
				is_add = (conflateSet(a, g, cur_ite + 1, mode) || is_add);
			}
		}
		else
		{
			mark = FALSE;
			for (iteA = Set[mode][a].begin(); iteA != Set[mode][a].end(); iteA++)
			{
				if (*iteA == *iteB)
				{
					mark = TRUE;
					break;
				}
			}
			if (!mark)
			{
				Set[mode][a].push_back(*iteB);
				//cout << "Add " << iteB->c << " to " << a.c << "'s FIRST" << endl;
				is_add = TRUE;
			}
		}
	}
	return is_add;
}

void makeFIRST()
{
	epsilonMark.clear();
	vector<Grammar>::iterator ite_g;
	bool mark;
	int count = 0;
	do
	{
		mark = FALSE;
		for (ite_g = GramList.begin(); ite_g != GramList.end(); ite_g++)
		{
			//cout << "Now at " << ite_g->init.c <<ite_g->init.t<< " Serial Begin: "<<ite_g->serial.begin()->c<< ite_g->serial.begin()->t <<endl;
			mark = (conflateSet(ite_g->init, *ite_g, ite_g->serial.begin(), FIRST) || mark);
		}
		//cout << "Round " << count++ << endl;
	} while (mark);
}
void makeFOLLOW()
{
	vector<Grammar>::iterator ite_g;
	vector<Symbol>::iterator ite_sym;
	Set[FOLLOW][S].push_back(Symbol('$', TERMIN));
	bool mark;
	do
	{
		mark = FALSE;
		for (ite_g = GramList.begin(); ite_g != GramList.end(); ite_g++)
		{
			for (ite_sym = ite_g->serial.begin(); ite_sym != ite_g->serial.end(); ite_sym++)
			{
				if (ite_sym->t == TERMIN) continue;
				if (ite_sym+1 == ite_g->serial.end())
					mark = (conflateSet(*ite_sym, ite_g->init) || mark);
				else
					mark = (conflateSet(*ite_sym, *ite_g, ite_sym+1, FOLLOW) || mark);
			}
		}
	} while (mark);
}
void printSet(bool mode)
{
	map<Symbol, vector<Symbol>>::iterator ite_out;
	vector<Symbol>::iterator ite_in;
	for (ite_out = Set[mode].begin(); ite_out != Set[mode].end(); ite_out++)
	{
		cout << "Symbol " << ite_out->first.c <<"'s set:"<< endl;
		for (ite_in = ite_out->second.begin(); ite_in != ite_out->second.end(); ite_in++)
			cout << ite_in->c << ' ';
		cout << endl;
	}
	return;
}

void makeTABLE()
{
	vector<Grammar>::iterator ite_g;
	vector<Symbol>::iterator ite_sym, ite_inner;
	for (ite_g = GramList.begin(); ite_g != GramList.end(); ite_g++)
	{
		if (ite_g->serial[HEAD].t == TERMIN)
		{
			//cout << "OK here!" << endl;
			AnalyzeTable[ite_g->init][ite_g->serial[HEAD]] = ite_g->seq;
			continue;
		}

		ite_sym = ite_g->serial.begin();
		bool mark;
		do
		{
			if (ite_sym == ite_g->serial.end()) break;
			mark = FALSE;
			for (ite_inner = Set[FIRST][*ite_sym].begin(); ite_inner != Set[FIRST][*ite_sym].end(); ite_inner++)
			{
				if (ite_inner->c == EPSILON)
				{
					if (ite_inner + 1 == Set[FIRST][*ite_sym].end())
					{
						vector<Symbol>::iterator temp_ite;
						for (temp_ite = Set[FOLLOW][ite_g->init].begin(); temp_ite != Set[FOLLOW][ite_g->init].end(); temp_ite++)
							AnalyzeTable[ite_g->init][*temp_ite] = ite_g->seq;
					}
					else mark = TRUE;
				}
				else AnalyzeTable[ite_g->init][*ite_inner] = ite_g->seq;
			}
			ite_sym++;
		}while (mark);
	}
}
void printTable()
{
	map<Symbol, map<Symbol, unsigned>>::iterator ite_out;
	map<Symbol, unsigned>::iterator ite_in;
	vector<Symbol>::iterator  ite_sym;
	for (ite_out = AnalyzeTable.begin(); ite_out != AnalyzeTable.end(); ite_out++)
	{
		cout << "------------------------------------------------------------" << endl;
		cout << "Symbol: " << ite_out->first.c << endl;
		for (ite_in = ite_out->second.begin(); ite_in != ite_out->second.end(); ite_in++)
		{
			cout << "When read: "<<ite_in->first.c<<" Phrase: " << GramList[ite_in->second].init.c<<' ';
			for (ite_sym = GramList[ite_in->second].serial.begin(); ite_sym != GramList[ite_in->second].serial.end(); ite_sym++)
				cout <<ite_sym->c;
			cout << endl;
		}
	}
}

void getSymStream_forTask2()
{
	fstream reader;
	reader.open("D:\\Users\\SymStream.txt", ios::in);

	char cur_char;
	while (!reader.eof())
	{
		reader >> cur_char;
		if (cur_char == '$') break;
		switch (cur_char)
		{
		case'(':
			SymStream.push_back(Symbol('(', TERMIN));
			break;
		case')':
			SymStream.push_back(Symbol(')', TERMIN));
			break;
		case'*':
			SymStream.push_back(Symbol('*', TERMIN));
			break;
		case'/':
			SymStream.push_back(Symbol('/', TERMIN));
			break;
		case'+':
			SymStream.push_back(Symbol('+', TERMIN));
			break;
		case'-':
			SymStream.push_back(Symbol('-', TERMIN));
			break;
		case'n':
			SymStream.push_back(Symbol('n', TERMIN));
			break;
		}
	}
	reader.close();
}
void doAnalyze()
{
	unsigned stream_ptr = 0;
	Symbol analyze_end('$', TERMIN);
	Symbol stackback;
	vector<Symbol>::iterator ite_sym;
	map<Symbol, unsigned>::iterator g_ite;
	Grammar cur_g;
	SymStream.push_back(analyze_end);
	AnalyzeStack.push_back(Symbol(analyze_end));
	AnalyzeStack.push_back(S);

	while (!AnalyzeStack.empty())
	{
		stackback = AnalyzeStack.back();
		if (AnalyzeStack.back().t == TERMIN)
		{
			if (stream_ptr < SymStream.size() && SymStream[stream_ptr] == stackback)
			{
				cout << "TERMIN matches ,Pop Symbol:" << AnalyzeStack.back().c << " and " << SymStream[stream_ptr++].c << " out from AnalyzeStack and SymbolStream" << endl;
				AnalyzeStack.pop_back();
			}
			else
			{
				cout << "Analyzing Wrong! In No." << stream_ptr << " of SymbolStream!" << endl;
				std::exit(1);
			}
		}
		else
		{
			g_ite = AnalyzeTable[stackback].find(SymStream[stream_ptr]);
			if (g_ite == AnalyzeTable[stackback].end())
			{
				cout << "Analyzing Wrong! In No." << stream_ptr << " of SymbolStream!" << endl;
				std::exit(1);
			}
			else cur_g = GramList[g_ite->second];

			cout << "SymbolStream's top is:"<<SymStream[stream_ptr].c<< " Now using phrase: " << cur_g.init.c << ' ';
			for (ite_sym = cur_g.serial.begin(); ite_sym != cur_g.serial.end(); ite_sym++)
				cout << ite_sym->c;
			cout << endl;
			for (int i = cur_g.serial.size() - 1; i >= 0; i--)
				AnalyzeStack.push_back(cur_g.serial[i]);
		}
	}
}