#include <iostream>
#include <string>
#include <stack>
#include "GMAnalysis.h"
#include "FILEOperator.h"
#include <set>

//函数宏简介
//WA用来debug,做代码静态检查
//type && token 用来表征词法分析器中单词寄存器的type和token
//getsym 调用词法分析器读入下一个单词
//！！！PUSH表示预读开始前保存现场，通常是现行单词分析(但没有解析输出)完成后使用PUSH记录栈帧
//！！！POP回到PUSH现场，通常是确认进入分支，进入之前使用
//OUT 用于输出到文件

#define WA do{cout << "----------WARNING-------" << __LINE__ << "---------" << endl;}while(0)
#define type (tk.now_token.type)
#define token (tk.now_token.token)
#define getsym do{(tk.get_token());}while(0)
#define PUSH do{(sp.push(tk.ch_pt - token.size()));}while(0)
#define POP do{tk.ch_pt = sp.top(); sp.pop(); tk.get_token();}while(0)
#define OUT(A) do{write_into_file(A);}while(0)


//WARNING!

extern string file_string;

gm_analyse::gm_analyse() {
	this->tk = tk_analyse();
}

void gm_analyse::isProgram() {
	if (type == CONSTTK) {
		isConstDeclaration();
	}
	if (type == INTTK || type == CHARTK) {
		//pre-read
		PUSH; getsym; 
		if (type == IDENFR) {
			getsym;
			if (type != LPARENT) {
				POP;
				isVariDeclaration();
			}
			else {
				POP;
			}
		}
	}
	while (type == INTTK || type == CHARTK || type == VOIDTK) {
		if (type == INTTK || type == CHARTK) {
			isReturnFunction();
		}
		else if (type == VOIDTK) {
			PUSH; getsym;
			if (type == MAINTK) {
				POP;  break;
			}
			else {
				POP;
				isVoidFunction();
			}
		}
	}

	//-------------main function ----------
	isMainFunction();
	//-------------OUT--------------------
	string s("<程序>");
	OUT(s);
}

void gm_analyse::isMainFunction(void){
	//--------------main function-----------------------
	if (type == VOIDTK) {
		OUT(tk.now_token); getsym;
	} //else error
	if (type == MAINTK) {
		OUT(tk.now_token); getsym;
	} //else error
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	} //else error
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	} //else error
	if (type == LBRACE) {
		OUT(tk.now_token); getsym;
	}
	isComStatement();

	if (type == RBRACE) {
		OUT(tk.now_token); getsym;
	}
	//---------------------pass-----------------
	string s("<主函数>");
	OUT(s);
}

void gm_analyse::isConstDeclaration(void){
	if (type == CONSTTK) {
		OUT(tk.now_token); getsym;
	} //else error
	isConstDefinition();
	if (type == SEMICN) {
		OUT(tk.now_token); getsym;
	} //else 
	while (type == CONSTTK) {
		OUT(tk.now_token); getsym;
		isConstDefinition();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
	}
	//----------------pass--------------------
	string s("<常量说明>");
	OUT(s);
}

void gm_analyse::isConstDefinition(void){
	if (type == INTTK) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == ASSIGN) {
			OUT(tk.now_token); getsym;
		}//else
		isInteger();
		while (type == COMMA) {
			OUT(tk.now_token); getsym;
			if (type == IDENFR) {
				OUT(tk.now_token); getsym;
			}//else
			if (type == ASSIGN) {
				OUT(tk.now_token); getsym;
			}//else
			isInteger();
		}
	}
	else if (type == CHARTK) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == ASSIGN) {
			OUT(tk.now_token); getsym;
		} //else
		if (type == CHARCON) {
			OUT(tk.now_token); getsym;
		}//else
		while (type == COMMA) {
			OUT(tk.now_token); getsym;
			if (type == IDENFR) {
				OUT(tk.now_token); getsym;
			}//else
			if (type == ASSIGN) {
				OUT(tk.now_token); getsym;
			}//else
			if (type == CHARCON) {
				OUT(tk.now_token); getsym;
			}//else
		}
	}//else
	//---------------pass------------
	string s("<常量定义>");
	OUT(s);
}

void gm_analyse::isVariDeclaration(void){
	isVariDefinition();
	if (type == SEMICN) {
		OUT(tk.now_token); getsym;
	}//else
	while (type == INTTK || type == CHARTK) {
		PUSH; getsym;
		if (type == IDENFR) {
			getsym;
			if (type != LPARENT) {
				POP;
				isVariDefinition();
				if (type == SEMICN) {
					OUT(tk.now_token); getsym;
				}
			}
			else {
				POP;
				break;
			}
		}
	}
	//----------------pass-------------
	string s("<变量说明>");
	OUT(s);
}
void gm_analyse::isVariDefinition(void){
	isTypeIden();
	if (type == IDENFR) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LBRACK) {
		OUT(tk.now_token); if (tk.ch != '0') {
			getsym;
			isUnsigned();
		}//else
		if (type == RBRACK) {
			OUT(tk.now_token); getsym;
		}
	}//not else
	while (type == COMMA) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}
		if (type == LBRACK) {
			OUT(tk.now_token); getsym;
			if (tk.ch != '0') {
				isUnsigned();
			}//else
			if (type == RBRACK) {
				OUT(tk.now_token); getsym;
			}//else
		}
	}
	//----------------pass-------------
	string s("<变量定义>");
	OUT(s);
}

void gm_analyse::isReturnFunction(void){
	isDeclarationHeader();
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isVariTable();
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LBRACE) {
		OUT(tk.now_token); getsym;
	}//else
	isComStatement();
	if (type == RBRACE) {
		OUT(tk.now_token); getsym;
	}
	//-------------pass----------------
	string s("<有返回值函数定义>");
	OUT(s);
}
void gm_analyse::isDeclarationHeader(void){
	if (type == INTTK) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			this->return_func.insert(token);
			OUT(tk.now_token); getsym;
		}
	}
	else if (type == CHARTK) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			this->return_func.insert(token);
			OUT(tk.now_token); getsym;
		}
	}//else
	//------------pass-------------------
	string s("<声明头部>");
	OUT(s);
}
void gm_analyse::isInteger(void){
	if (type == PLUS || type == MINU) {
		OUT(tk.now_token); getsym;
	}
	isUnsigned();
	//----------------pass------------------------
	string s("<整数>");
	OUT(s);
}
void gm_analyse::isUnsigned(void){
	if (type == INTCON) {
		OUT(tk.now_token); getsym;
	}//else
	//------------------pass-----------------
	string s("<无符号整数>");
	OUT(s);
}
void gm_analyse::isChar(void){
	if (type == CHARCON) {
		OUT(tk.now_token); getsym;
	}//else
	//-----------pass-------------
}
void gm_analyse::isVariTable(void){
	string s("<参数表>");
	if (type == RPARENT) {
		//empty
		OUT(s);
		return;
	}
	isTypeIden();
	if (type == IDENFR) {
		OUT(tk.now_token); getsym;
	}
	while (type == COMMA) {
		OUT(tk.now_token); getsym;
		isTypeIden();
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
	}
	//------------pass-----------------
	OUT(s);
}
void gm_analyse::isValueVariTable(void){
	string s("<值参数表>");
	if (type == RPARENT) {
		//empty
		OUT(s);
		return;
	}
	isExpression();
	while (type == COMMA) {
		OUT(tk.now_token); getsym;
		isExpression();
	}
	//----------pass------------------
	OUT(s);
}
void gm_analyse::isTypeIden(void){
	if (type == INTTK || type == CHARTK) {
		OUT(tk.now_token); getsym;
	}
	//-----------pass--------------
	
}
void gm_analyse::isComStatement(void){
	if (type == CONSTTK) {
		isConstDeclaration();
	}
	if (type == INTTK || type == CHARTK) {
		isVariDeclaration();
	}
	isStatementColumn();
	//------------------pass-----------------
	string s("<复合语句>");
	OUT(s);
}
void gm_analyse::isStatementColumn(void){
	string s("<语句列>");
	if (type == RBRACE) {
		OUT(s);
		return;
	}
	while (type == IFTK || type == WHILETK || type == FORTK || type == DOTK || type == LBRACE || type == IDENFR || type == SCANFTK || type == PRINTFTK || type == SEMICN || type == RETURNTK) {
		isStatement();
	}
	//------------------pass-----------------
	OUT(s);
}
void gm_analyse::isStatement(void){
	string s("<语句>");
	if (type == IFTK) {
		isConditionStatement();
	}
	else if (type == WHILETK || type == FORTK || type == DOTK) {
		isLoopStatement();
	}
	else if (type == LBRACE) {
		OUT(tk.now_token); getsym;
		isStatementColumn();
		if (type == RBRACE) {
			OUT(tk.now_token); getsym;
		}
	}
	else if (type == SCANFTK) {
		isScanfStatement();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
	}
	else if (type == PRINTFTK) {
		isPrintfStatement();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
	}
	else if (type == RETURNTK) {
		isReturnStatement();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
	}
	else if (type == SEMICN) {
		OUT(tk.now_token); getsym;
	}
	else if (type == IDENFR) {
		PUSH; getsym;
		if (type == LPARENT) {
			POP;
			if(type == IDENFR && return_func.find(token) != return_func.end()){
				isReturnFunctionCall();
			}
			else if (type == IDENFR && void_func.find(token) != void_func.end()) {
				isVoidFunctionCall();
			}
			if (type == SEMICN) {
				OUT(tk.now_token); getsym;
			}//else
		}
		else {
			POP;
			isAssignStatement();
			if (type == SEMICN) {
				OUT(tk.now_token); getsym;
			}//else
		}
	}//else
	OUT(s);
}
void gm_analyse::isConditionStatement(void){
	if (type == IFTK) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isCondition();
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isStatement();
	if (type == ELSETK) {
		OUT(tk.now_token); getsym;
		isStatement();
	}//not else
	//----------------------pass---------------------
	string s("<条件语句>");
	OUT(s);
}
void gm_analyse::isLoopStatement(void){
	if (type == WHILETK) {
		OUT(tk.now_token); getsym;
		if (type == LPARENT) {
			OUT(tk.now_token); getsym;
		}//else
		isCondition();
		if (type == RPARENT) {
			OUT(tk.now_token); getsym;
		}//else
		isStatement();
	}
	else if (type == DOTK) {
		OUT(tk.now_token); getsym;
		isStatement();
		if (type == WHILETK) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == LPARENT) {
			OUT(tk.now_token); getsym;
		}//else
		isCondition();
		if (type == RPARENT) {
			OUT(tk.now_token); getsym;
		}//else
	}
	else if (type == FORTK) {
		OUT(tk.now_token); getsym;
		if (type == LPARENT) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == ASSIGN) {
			OUT(tk.now_token); getsym;
		}//else
		isExpression();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
		isCondition();
		if (type == SEMICN) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == ASSIGN) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}//else
		if (type == PLUS || type == MINU) {
			OUT(tk.now_token); getsym;
		}//else
		isStep();
		if (type == RPARENT) {
			OUT(tk.now_token); getsym;
		}//else
		isStatement();
	}//else

	//---------------pass--------------------
	string s("<循环语句>");
	OUT(s);
}
void gm_analyse::isCondition(void){
	isExpression();
	if (type == LSS || type == LEQ || type == GRE || type == GEQ || type == NEQ || type == EQL) {
		isRelationshipOperator();
		isExpression();
	}// not else
	//-----------------pass--------------
	string s("<条件>");
	OUT(s);
}
void gm_analyse::isAssignStatement(void){
	if (type == IDENFR) {
		OUT(tk.now_token); getsym;
		if (type == LBRACK) {
			OUT(tk.now_token); getsym;
			isExpression();
			if (type == RBRACK) {
				OUT(tk.now_token); getsym;
			}
		}
	}
	if (type == ASSIGN) {
		OUT(tk.now_token); getsym;
	}
	isExpression();
	//----------------pass---------------
	string s("<赋值语句>");
	OUT(s);
}
void gm_analyse::isScanfStatement(void){
	if (type == SCANFTK) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == IDENFR) {
		OUT(tk.now_token); getsym;
	}//else
	while (type == COMMA) {
		OUT(tk.now_token); getsym;
		if (type == IDENFR) {
			OUT(tk.now_token); getsym;
		}
	}
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}
	//------------pass---------------
	string s("<读语句>");
	OUT(s);
}
void gm_analyse::isPrintfStatement(void){
	if (type == PRINTFTK) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == STRCON) {
		OUT(tk.now_token); getsym;
		string ss("<字符串>");
		OUT(ss);
		if (type == COMMA) {
			OUT(tk.now_token); getsym;
			isExpression();
		}
	}
	else{
		isExpression();
	}
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}
	//------------pass---------------
	string s("<写语句>");
	OUT(s);
}
void gm_analyse::isReturnStatement(void){
	if (type == RETURNTK) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
		isExpression();
		if (type == RPARENT) {
			OUT(tk.now_token); getsym;
		}
	}
	//------------pass---------------
	string s("<返回语句>");
	OUT(s);
}
void gm_analyse::isPlusOperator(void){
	if (type == PLUS || type == MINU) {
		OUT(tk.now_token); getsym;
	}
}
void gm_analyse::isMultOperator(void){
	if (type == MULT || type == DIV) {
		OUT(tk.now_token); getsym;
	}
}
void gm_analyse::isRelationshipOperator(void){
	if (type == LSS || type == LEQ || type == GRE || type == GEQ || type == NEQ || type == EQL) {
		OUT(tk.now_token); getsym;
	}
}
void gm_analyse::isVoidFunction(void){
	if (type == VOIDTK) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == IDENFR) {
		this->void_func.insert(token);//Warning!
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isVariTable();
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LBRACE) {
		OUT(tk.now_token); getsym;
	}//else
	isComStatement();
	if (type == RBRACE) {
		OUT(tk.now_token); getsym;
	}//else
	//-----------------------pass-----------------------
	string s("<无返回值函数定义>");
	OUT(s);
}
void gm_analyse::isReturnFunctionCall(void){
	if (type == IDENFR && return_func.find(token) != return_func.end()) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isValueVariTable();
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}
	string s("<有返回值函数调用语句>");
	OUT(s);
}
void gm_analyse::isVoidFunctionCall(void) {
	if (type == IDENFR && void_func.find(token) != void_func.end()) {
		OUT(tk.now_token); getsym;
	}//else
	if (type == LPARENT) {
		OUT(tk.now_token); getsym;
	}//else
	isValueVariTable();
	if (type == RPARENT) {
		OUT(tk.now_token); getsym;
	}
	string s("<无返回值函数调用语句>");
	OUT(s);
}
void gm_analyse::isExpression(void){
	if (type == PLUS || type == MINU) {
		OUT(tk.now_token); getsym;
	}// not else
	isTerm();
	while (type == PLUS || type == MINU) {
		isPlusOperator();
		isTerm();
	}
	//---------------pass------------------
	string s("<表达式>");
	OUT(s);
}
void gm_analyse::isTerm(void){
	isFactor();
	while (type == MULT || type == DIV) {
		isMultOperator();
		isFactor();
	}
	//--------------pass------------------------
	string s("<项>");
	OUT(s);
}
void gm_analyse::isFactor(void) {
	if (type == IDENFR) {
		PUSH; getsym;
		if (type == LPARENT) {
			POP;
			isReturnFunctionCall();
		}
		else {
			POP; OUT(tk.now_token); getsym;
			if (type == LBRACK) {
				OUT(tk.now_token); getsym;
				isExpression();
				if (type == RBRACK) {
					OUT(tk.now_token); getsym;
				}
			}// not else

		}
	}          
	else if (type == LPARENT) {
		OUT(tk.now_token); getsym;
		isExpression();
		if (type == RPARENT) {
			OUT(tk.now_token); getsym;
		}
	}
	else if (type == PLUS || type == MINU || type == INTCON) {
		isInteger();
	}
	else if (type == CHARCON) {
		isChar();
	}//else
	//------------pass----------------
	string s("<因子>");
	OUT(s);
}
void gm_analyse::isStep(void) {
	isUnsigned();
	//---------pass-----------
	string s("<步长>");
	OUT(s);
}
