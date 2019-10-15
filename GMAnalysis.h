#pragma once
#include <iostream>
#include <string>
#include <stack>
#include "TkAnalysis.h"
#include <set>
using namespace std;

class gm_analyse {
public:
	gm_analyse();
	stack<unsigned> sp;
	tk_analyse tk;
	set<string> return_func;
	set<string> void_func;
	void isProgram(void);
	void isMainFunction(void);
	void isConstDeclaration(void);
	void isConstDefinition(void);
	void isVariDeclaration(void);
	void isVariDefinition(void);
	void isReturnFunction(void);
	void isDeclarationHeader(void);
	void isInteger(void);
	void isUnsigned(void);
	void isChar(void);
	void isVariTable(void);
	void isValueVariTable(void);
	void isTypeIden(void);
	void isComStatement(void);
	void isStatementColumn(void);
	void isStatement(void);
	void isConditionStatement(void);
	void isLoopStatement(void);
	void isCondition(void);
	void isAssignStatement(void);
	void isScanfStatement(void);
	void isPrintfStatement(void);
	void isReturnStatement(void);
	void isPlusOperator(void);
	void isMultOperator(void);
	void isRelationshipOperator(void);
	void isVoidFunction(void);
	void isReturnFunctionCall(void);
	void isVoidFunctionCall(void);

	void isExpression(void);
	void isTerm(void);
	void isFactor(void);
	void isStep(void);

};