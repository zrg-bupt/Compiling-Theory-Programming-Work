#include"IncludeAndDef.h"
#include"func.h"
//test code

int main(void)
{
	loadGrammar();
	getSymStream_forTask2();
	cout << "Load OK!" << endl;
	printGrammar();
	initSet();
	makeFIRST();
	makeFOLLOW();
	cout << "FIRST" << endl;
	printSet(FIRST);
	cout << "FOLLOW" << endl;
	printSet(FOLLOW);
	cout << "TABLE" << endl;
	makeTABLE();
	printTable();
	//doAnalyze();
	cout << "All done!" << endl;
	
	return 0;
}