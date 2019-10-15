#include <iostream>
#include <string>
#include <unordered_map>
#include "TkAnalysis.h"
#include "FILEOperator.h"
extern string file_string;

const static vector<string> tk_type_string = {
	"EMPTYS",
	"IDENFR", "INTCON", "CHARCON", "STRCON", "CONSTTK",
	"INTTK", "CHARTK", "VOIDTK", "MAINTK", "IFTK",
	"ELSETK", "DOTK", "WHILETK", "FORTK", "SCANFTK",
	"PRINTFTK", "RETURNTK", "PLUS", "MINU", "MULT",
	"DIV", "LSS", "LEQ", "GRE", "GEQ",
	"EQL", "NEQ", "ASSIGN", "SEMICN", "COMMA",
	"LPARENT", "RPARENT", "LBRACK", "RBRACK", "LBRACE",
	"RBRACE"
};	//����token���������͵Ķ�Ӧ�ַ�����ע����enum�ṹһһ��Ӧ��

typedef unordered_map<string, int> string_int_map;
string_int_map reserved_word = {
	{"const", CONSTTK}, {"int", INTTK}, {"char", CHARTK}, {"void", VOIDTK}, {"main",MAINTK},
	{"if",IFTK}, {"else",ELSETK}, {"do",DOTK}, {"while", WHILETK}, {"for",FORTK},
	{"scanf",SCANFTK},{"printf",PRINTFTK}, {"return", RETURNTK }
};			//�����ֵĶ�Ӧmap(string->ENUM)

token_info::token_info() {
	;
}//Ĭ�Ϲ���


//���õĹ��캯��
token_info::token_info(int type, string& token) {
	this->type = type;
	this->token = token;
}


//��type(int�ͱ���)ת�����ַ�����tostring
string token_info::out_type_string(){
	return tk_type_string[this->type];
}


//�ʷ���������Ĭ�Ϲ�����ɳ�ʼ�����̣�a)ָ���0    b)��ǰ�ַ���'\0'   c)��ȡ��һ�����ʣ����뵥�ʼĴ���
tk_analyse::tk_analyse() {
	this->ch_pt = 0;
	this->ch = '\0';
	get_token();
}

//�ʷ������ĺ��ģ���ȡ�������ַ������������ĵ��ʴ���Ĵ���now_token
void tk_analyse::get_token() {
	get_char();
	while (isspace(ch))
		get_char();
	if (is_alpha()) {
		string tk_temp;
		while (is_alnum()) {
			tk_temp.push_back(ch); get_char();
		}
		unget_char();
		if (reserved_word.find(tk_temp) == reserved_word.end()) { //IDENFR
			now_token = token_info(IDENFR, tk_temp);
		}
		else {
			now_token = token_info(reserved_word[tk_temp], tk_temp);
		}
	}
	else if(is_digit()){
		string tk_temp;
		if (ch == '0') {
			get_char();
			if (is_digit()) {
				//error
			}
			else {
				unget_char();
				tk_temp.push_back(ch);
			}
		}
		else {
			while (is_digit()) {
				tk_temp.push_back(ch); get_char();
			}
			unget_char();
		}
		now_token = token_info(INTCON, tk_temp);
	}
	else {
		string tk_temp = "";
		switch (ch)
		{
		case '\'': {
			get_char();
			if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || is_alnum()) {
				tk_temp.push_back(ch);
				get_char();
				if (ch == '\'')
					now_token = token_info(CHARCON, tk_temp);
				else {/*error*/ }
			}
			else {
				//error
			}
			break;
		}
		case '"': {
			get_char();
			if (ch == '"') {
				tk_temp = "";
				now_token = token_info(STRCON, tk_temp);
				break;
			}
			while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
				tk_temp.push_back(ch);
				get_char();
			}
			if (ch == '"')
				now_token = token_info(STRCON, tk_temp);
			else {/*error*/}
			break;
		}
		case '+':{
			tk_temp = "+";
			now_token = token_info(PLUS, tk_temp);
			break;
		}
		case '-' : {
			tk_temp = "-";
			now_token = token_info(MINU, tk_temp);
			break;
		}
		case '*': {
			tk_temp = "*";
			now_token = token_info(MULT, tk_temp);
			break;
		}
		case '/': {
			tk_temp = "/";
			now_token = token_info(DIV, tk_temp);
			break;
		}
		case '<': {
			get_char();
			if (ch == '=') {
				tk_temp = "<=";
				now_token = token_info(LEQ, tk_temp);
				break;
			}
			unget_char();
			tk_temp = "<";
			now_token = token_info(LSS, tk_temp);
			break;
		}
		case '>': {
			get_char();
			if (ch == '=') {
				tk_temp = ">=";
				now_token = token_info(GEQ, tk_temp);
				break;
			}
			unget_char();
			tk_temp = ">";
			now_token = token_info(GRE, tk_temp);
			break;
		}
		case '!': {
			get_char();
			if (ch == '=') {
				tk_temp = "!=";
				now_token = token_info(NEQ, tk_temp);
				break;
			}
			else{/*error*/ }
			break;
		}
		case '=': {
			get_char();
			if (ch == '=') {
				tk_temp = "==";
				now_token = token_info(EQL, tk_temp);
				break;
			}
			unget_char();
			tk_temp = "=";
			now_token = token_info(ASSIGN, tk_temp);
			break;
		}
		case ';': {
			tk_temp = ";";
			now_token = token_info(SEMICN, tk_temp);
			break;
		}
		case ',': {
			tk_temp = ",";
			now_token = token_info(COMMA, tk_temp);
			break;
		}
		case '(': {
			tk_temp = "(";
			now_token = token_info(LPARENT, tk_temp);
			break;
		}
		case ')': {
			tk_temp = ")";
			now_token = token_info(RPARENT, tk_temp);
			break;
		}
		case '[': {
			tk_temp = "[";
			now_token = token_info(LBRACK, tk_temp);
			break;
		}
		case ']': {
			tk_temp = "]";
			now_token = token_info(RBRACK, tk_temp);
			break;
		}
		case '{': {
			tk_temp = "{";
			now_token = token_info(LBRACE, tk_temp);
			break;
		}
		case '}': {
			tk_temp = "}";
			now_token = token_info(RBRACE, tk_temp);
			break;
		}
		case '\0': {
			tk_temp = "";
			now_token = token_info(EMPTYS, tk_temp);
			break;
		}
		default:
			break;
		}
	}

}


//������һ���ַ���ָ����Զָ��ǰ�ַ�����һ���ַ���
//tips�������б���ָ��Խ�磬��ch��'\0'
void tk_analyse::get_char() {
	if (ch_pt >= file_string.size()) {
		this->ch = '\0';
	}
	this->ch = file_string[this->ch_pt++];
}


//������һ���ַ�����ԭָ��
void tk_analyse::unget_char() {
	this->ch_pt--;
	this->ch = file_string[this->ch_pt - 1];
}


//����Ϊ��Ӧ���ж�ch�ַ�������䣬������ĸ������'_'
int tk_analyse::is_alpha() {
	return (isalpha(this->ch) || this->ch == '_');
}

int tk_analyse::is_space() {
	return isspace(this->ch);
	
}

int tk_analyse::is_digit() {
	return isdigit(this->ch);
}

int tk_analyse::is_alnum() {
	return (isalnum(this->ch) || this->ch == '_');
}

void tk_analyse::output() {
	write_into_file(this->now_token);
}

