#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stdlib.h>
#include "TkAnalysis.h"

using namespace std;

static const char* input_filename = "testfile.txt";
static const char* output_filename = "output.txt";
static ifstream ifile;
static ofstream ofile;

string file_string;

void open_file() {
	ifile.open(input_filename);
	ofile.open(output_filename);
	if (!ifile)
		cout << "!!!open input file failed!!!" << endl;
	if (!ofile)
		cout << "!!!open output file failed!!!" << endl;
}

void read_from_file() {
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	file_string = buf.str();
	//return buf.str();
}

void write_into_file(token_info& tk) {
	cout << tk.out_type_string() << " " << tk.token << endl;
	ofile << tk.out_type_string() << " " << tk.token << endl;
}

void write_into_file(string s) {
	cout << s << endl;
	ofile << s << endl;
}
void close_file() {
	if (ifile)
		ifile.close();
	if (ofile)
		ofile.close();
}