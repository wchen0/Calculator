#include "head.h"
#include "value.h"
#include <stack>
#include <cmath>

/*
* Use suffix expression to calculate
*/


double calculate(node* suffix, int len) {   //返回INF代表无效
	for (int i = 0; i < len; i++) {
		if (suffix[i].type == Varb) suffix[i].value = findValue(suffix[i].str);
		if (fabs(suffix[i].value - INF) < EPS) return INF;
	}
	std::stack<double> s;
	for (int i = 0; i < len; i++) {
		if (suffix[i].type == Optr) {
			if (suffix[i].str[0] == '*') {
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double a = s.top();
				s.pop();
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double b = s.top();
				s.pop();
				s.push(a * b);
			}
			else if (suffix[i].str[0] == '/') {
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double a = s.top();
				s.pop();
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double b = s.top();
				s.pop();
				if (fabs(a) < EPS) {
					printf("Invalid Input: Denominator is zero\n");
					return INF;
				}
				s.push(b / a);
			}
			else if (suffix[i].str[0] == '+') {
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double a = s.top();
				s.pop();
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double b = s.top();
				s.pop();
				s.push(a + b);
			}
			else if (suffix[i].str[0] == '-') {
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double a = s.top();
				s.pop();
				if (s.empty()) { printf("Invalid Input\n"); return INF; }
				double b = s.top();
				s.pop();
				s.push(b - a);
			}
		}
		else s.push(suffix[i].value);
	}
	if (s.empty()) { printf("Invalid Input\n"); return INF; }
	return s.top();
}
