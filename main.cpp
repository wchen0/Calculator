#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cctype>
#include "head.h"
#include "value.h"



/*
* Author: Chen Weitian
* Date: 2021-06
* Description: School work for 'Fundamental of Programming'
*/




int main(void) {
	char cmd[100] = "";

	printf("This is a simple calculator which allows '+', '-', '*', '/'\n"
		"Operants can be numbers such as '1300', '50.20', '.234', '24.' or variables\n"
		"A variable is defined by 'Var var_name = expression'\n"
		"Enter 'SHOW VAR' to see all the variables you've defined\n"
		"Enter '%s' to end this program\nThis program is CAse-SeNsItiVe\n\n", _END_WORD);


	for (;;) {
		printf("> ");
		std::cin >> cmd;
		if (strcmp(cmd, "Var") == 0) {
			char variableName[LEN] = "", val_expr[STRLEN] = "";
			int len = 0, newLen = 0;
			if (getVariableName(variableName) == false) {
				while (getchar() != '\n');
				continue;
			}
			std::cin >> val_expr;
			node* a = str_to_arr(val_expr, &len);
			if (a == NULL) continue;

			a = inffix_to_suffix(a, len, &newLen);
			if (a == NULL) continue;

			double ans = calculate(a, newLen);
			delete[] a;
			if (fabs(ans - INF) < EPS) continue;
			addVariable(variableName, ans);
		}
		else if (strcmp(cmd, "SHOW") == 0) {
			std::cin >> cmd;
			if (strcmp(cmd, "VAR") == 0) showVariables();
			else printf("Invalid Input. Cannot identify %s\n", cmd);
		}
		else if (strcmp(cmd, _END_WORD) == 0) break;
		else {
			char temp[STRLEN] = "";
			int len = 0, newLen = 0;
			std::cin.getline(temp, STRLEN);

			strcat(cmd, temp);   //concatenate strings

			node* a = str_to_arr(cmd, &len);
			if (a == NULL) continue;
			
			a = inffix_to_suffix(a, len, &newLen);
			if (a == NULL) continue;
			
			double ans = calculate(a, newLen);
			delete[] a;
			if (fabs(ans - INF) < EPS) continue;
			else std::cout << "> " << ans << std::endl;
		}
	}
	return 0;
}
