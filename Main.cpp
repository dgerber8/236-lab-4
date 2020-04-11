#include <iostream> 
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Header.h"
#include "Grammar.h"
#include "SchemeClass.h"

using namespace std;

int main(int argc, char* argv[])
{
	// open the in stream and get the first line (number of students and exams)
	ifstream in(argv[1]);

	DataLogProgram(in);
	int i;
	cin >> i;
	return 0;
}

