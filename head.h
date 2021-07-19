#pragma once

//#define DEBUG

#define _END_WORD "ESC"

#define STRLEN 100
#define LEN 13
#define INF 1000000000.0
#define EPS 1e-3

enum node_type { Optr, Cons, Varb };
//Optr: Operant
//Cons: Constant
//Varb: Variable

struct node {
	node_type type;
	double value;
	char str[LEN];
};

node* str_to_arr(char* src, int* ansLen);

node* inffix_to_suffix(node* src, int len, int* newLen);

double calculate(node* suffix, int len);

bool isoptr(char c);

bool getVariableName(char* str);

void unary_operator_scan(node* a, int& len);

#ifdef DEBUG

void printNodeArray(node* a, int len);

#endif // DEBUG
