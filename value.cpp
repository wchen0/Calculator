#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include "head.h"
#include "value.h"

//This file provides a hash table to store all the variables
//Seperating chaining is used to avoid confliction


struct variable {
	char str[100];
	double val;
	variable* next;
};

#define tableSize 31

variable* variableArray[tableSize];

//hash function
int hash(char* key) {
	int hashVal = 0;
	while (*key != 0)
		hashVal = (hashVal << 5) + *key++;
	return hashVal % tableSize;
}

double findValue(char* str) {
	register variable* ptr = variableArray[hash(str)];
	for (; ptr; ptr = ptr->next) {
		if (strcmp(ptr->str, str) == 0) break;
	}
	if (ptr) return ptr->val;
	else {
		printf("cannot find variable %s\n", str);
		return INF;
	}
}

bool variableExist(char* str) {
	for (register variable* ptr = variableArray[hash(str)]; ptr; ptr = ptr->next) {
		if (strcmp(ptr->str, str) == 0) return true;
	}
	return false;
}

void addVariable(char* str, const double val) {
	register int index = hash(str);
	register variable* ptr = variableArray[index];
	if (ptr == NULL) {
		variableArray[index] = new variable;
		variableArray[index]->next = NULL;
		variableArray[index]->val = val;
		strcpy(variableArray[index]->str, str);
	}
	else {
		for (; ptr->next; ptr = ptr->next) {
			if (strcmp(ptr->str, str) == 0) {
				printf("The variable '%s' has existed. Choose another name\n", str);
			}
		}
		ptr->next = new variable;
		ptr->next->next = NULL;
		ptr->next->val = val;
		strcpy(ptr->next->str, str);
	}
}

void showVariables(void) {
	printf("Name\tValue\n");
	register int cnt = 0;
	for (register int i = 0; i < tableSize; i++) {
		for (variable* ptr = variableArray[i]; ptr; ptr = ptr->next) {
			printf("%s\t%f\n", ptr->str, ptr->val);
			cnt++;
		}
	}
	std::cout << cnt << " variable(s) in total" << std::endl;
}

void changeValue(char* str, const double newVal) {
	register variable* ptr = variableArray[hash(str)];
	for (; ptr; ptr = ptr->next) {
		if (strcmp(ptr->str, str) == 0) break;
	}
	if (ptr == NULL) {
		printf("Cannot Find Variable %s\n", str);
		return;
	}
	ptr->val = newVal;
	return;
}
