#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stack>   //STL
#include <cstring>
#include <cctype>
#include "head.h"
#include "value.h"


bool getVariableName(char* str) {
	char c = 0;
	int i = 0;
	for (i = 0;; i++) {
		std::cin >> c;
		if (c == '=') break;
		else if (c == ' ') {
			printf("Space is not allowed in variable's name\n");
			return false;
		}
		else if (isoptr(c)) {
			printf("'%c' is used as an operator which cannot be included in variable's name\n", c);
			return false;
		}
		else str[i] = c;
	}
	str[i] = 0;
	if (strcmp(str, _END_WORD) == 0 || strcmp(str, "SHOW") == 0 || strcmp(str, "VAR") == 0 || strcmp(str, "Var") == 0) {
		printf("%s cannot be used as the name of a variale\n", str);
		return false;
	}
	else if (variableExist(str)) {
		printf("Variable %s has existed\n", str);
		return false;
	}
	else if (isdigit(str[0])) {
		printf("Variable's name cannot begin with a number\n");
		return false;
	}
	else if (str[0] == '.') {
		printf("Variable's name cannot begin with '%c'\n", str[0]);
	}
	for (int i = 0; str[i]; i++) {
		if (!isdigit(str[i]) && !isalpha(str[i]) && str[i] != '_' && str[i] != '.') {
			printf("Variable's name cannot involve '%c'\n", str[i]);
			return false;
		}
	}
	return true;
}


double str_to_float(const char* s) {
	int len = strlen(s), dotIndex = 0, integer = 0;
	double decimal = 0;
	for (dotIndex = 0; s[dotIndex] != '.' && dotIndex < len; dotIndex++);
	for (int j = 0; j < dotIndex; j++) {
		integer = integer * 10 + s[j] - '0';
	}
	if (dotIndex != len) {
		for (int j = len - 1; j > dotIndex; j--) {
			decimal += s[j] - '0';
			decimal /= 10;
		}
	}
	return decimal + 1.0 * integer;
}

bool isoptr(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

//将操作符、操作数全部分开
node* str_to_arr(char* src, int* ansLen) {
	int index = 0, len = strlen(src);
	node* a = new node[100];
	for (int i = 0; i < len; i++) {
		if (src[i] == ' ') continue;   //space is allowed between operants and operators
		else if (isoptr(src[i])) {
			a[index].str[0] = src[i];
			a[index].str[1] = 0;
			a[index].type = Optr;
			index++;
		}
		else if (isdigit(src[i]) || src[i] == '.') {
			char tempStr[STRLEN] = "";
			int tempIndex = 0, j = i;
			for (j = i; (isdigit(src[j]) || src[j] == '.') && j < len; j++) {
				tempStr[tempIndex++] = src[j];
			}
			tempStr[tempIndex] = 0;
			a[index].value = str_to_float(tempStr);
			a[index].type = Cons;
			index++;
			i = j - 1;
		}
		else if (isalpha(src[i])) {
			char tempStr[STRLEN] = "";
			int tempIndex = 0, j = i;   //变量名可包含数字，但不能作为开头的字符
			for (j = i; !isoptr(src[j]) && /*!isdigit(src[j]) && src[j] != '.' &&*/ j < len; j++) {
				tempStr[tempIndex++] = src[j];
			}
			tempStr[tempIndex] = 0;
			strcpy(a[index].str, tempStr);
			
			if (variableExist(a[index].str)) a[index].type = Varb;
			else {
				printf("Invalid Input. Cannot identify '%s'\n", a[index].str);
				delete[] a;
				return NULL;
			}
			index++;
			i = j - 1;
		}
		else {
			printf("Invalid Input. Cannot identify '%c'\n", src[i]);
			delete[] a;
			return NULL;
		}
	}

	unary_operator_scan(a, index);

	*ansLen = index;
	//printNodeArray(a, index);
	return a;
}

#ifdef DEBUG


//for Debug
void printNode(node a) {
	if (a.type == Optr) putchar(a.str[0]), putchar(' ');
	else if (a.type == Cons) printf("%f ", a.value);
	else if (a.type == Varb) std::cout << a.str << " ";
}
//for Debug
void printNodeArray(node* a, int len) {
	printf("printNodeArray\n");
	for (int i = 0; i < len; i++) {
		printNode(a[i]);
	}
	putchar('\n');
}
//for Debug
void displayStack(std::stack<node> ans) {
	printf("Stack: ");
	std::stack<node> s;
	while (ans.empty() == false) {
		s.push(ans.top());
		ans.pop();
		printNode(s.top());
	}
	while (s.empty() == false) {
		ans.push(s.top());
		s.pop();
	}
	printf("\n");
}


#endif // DEBUG


node* inffix_to_suffix(node* src, int len, int* newLen) {
	std::stack<node> ans_stack;
	std::stack<node> optr_stack;
	for (int i = 0; i < len; i++) {
		//displayStack(ans_stack);
		//displayStack(optr_stack);
		if (src[i].type == Cons || src[i].type == Varb) ans_stack.push(src[i]);
		else if (src[i].str[0] == '(') optr_stack.push(src[i]);
		else if (src[i].str[0] == ')') {
			while (optr_stack.empty() == false && optr_stack.top().str[0] != '(') {
				ans_stack.push(optr_stack.top());
				optr_stack.pop();
			}
			if (optr_stack.empty()) {
				printf("Invalid Input. Bracket is missing\n");
				return NULL;
			}
			else optr_stack.pop();
		}
		else if (optr_stack.empty() == true) optr_stack.push(src[i]);
		else if (src[i].str[0] == '*' || src[i].str[0] == '/') {
			while (optr_stack.empty() == false && optr_stack.top().str[0] != '+' && optr_stack.top().str[0] != '-' && optr_stack.top().str[0] != '(') {
				ans_stack.push(optr_stack.top());
				optr_stack.pop();
			}
			optr_stack.push(src[i]);
		}
		else if (src[i].str[0] == '+' || src[i].str[0] == '-') {
			while (optr_stack.empty() == false && optr_stack.top().str[0] != '(') {
				ans_stack.push(optr_stack.top());
				optr_stack.pop();
			}
			optr_stack.push(src[i]);
		}
	}

	while (optr_stack.empty() == false) {
		ans_stack.push(optr_stack.top());
		optr_stack.pop();
	}
	node* tempArray = new node[STRLEN];
	int index = 0;
	while (ans_stack.empty() == false) {
		tempArray[index++] = ans_stack.top();
		ans_stack.pop();
	}
	for (int i = 0; i < index / 2; i++) {
		node tempNode = tempArray[i];
		tempArray[i] = tempArray[index - 1 - i];
		tempArray[index - 1 - i] = tempNode;
	}
	*newLen = index;
	return tempArray;
}


static void deleteNode(node* a, int& len, int target) {
	for (int t = target; t < len - 1; t++) {
		a[t] = a[t + 1];
	}
	len--;
}

//find unary operators in inffix expression

void unary_operator_scan(node* a, int& len) {
	if (a[0].str[0] == '+') {
		deleteNode(a, len, 0);
	}
	else if (a[0].str[0] == '-') {
		if (a[1].type == Varb) {
			a[1].type = Cons;
			a[1].value = findValue(a[1].str);
		}
		a[1].value *= (-1);
		deleteNode(a, len, 0);
	}
	for (int i = 0; i < len - 2; i++) {
		if (a[i].type == Optr && a[i+1].type == Optr) {
			if (a[i].str[0] == '+' || a[i].str[0] == '-' || a[i].str[0] == '*' || a[i].str[0] == '/' || a[i].str[0] == '(') {
				if (a[i + 1].str[0] == '-') {
					if (a[i + 2].type == Varb) {
						a[i + 2].type = Cons;
						a[i + 2].value = findValue(a[i + 2].str);
					}
					a[i + 2].value *= (-1);
					deleteNode(a, len, i + 1);
				}
				else if (a[i + 1].str[0] == '+') {
					deleteNode(a, len, i + 1);
				}
			}
		}
	}
}
