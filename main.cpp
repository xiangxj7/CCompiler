#include "FILEOperator.h"
#include "TkAnalysis.h"
#include "GMAnalysis.h"
#include <iostream>

extern string file_string;
int main() {
	open_file();
	read_from_file();
	/*tk_analyse tk = tk_analyse();
	while (tk.now_token.type != EMPTYS) {
		tk.output();
		tk.get_token();
	}*/
	gm_analyse gm = gm_analyse();
	gm.isProgram();
	close_file();
	return 0;
}