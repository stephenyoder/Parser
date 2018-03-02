// Developed by:  Stephen Yoder
// Email Address: stephenayoder@gmail.com
// 
// Designed for PLEXSYS Interface Products, Inc.
//
// FILE: parsing.h
//
// This program parses and evaluates arithmetic expressions with operators including: +, -, *, /, and ^
//
// The main algorithm of this program receives and input string representing an arithmetic expression.
// Each value and operator is then tokenized and inserted into a vector of strings.
// An iterator then traverses through the vector and compares the precedence an operator compared to the next operator.
// If the first operator has precedence, the expression around the first operator is evaluted and the result replaces the two values and operator in the vector.
// The iterator increments by two until the penultimate operator and then resets to the beginning of the list.
// If at any point the size of the vector reaches 3 (just 2 values and 1 operator left), the loop will exit and the last two values are evaluated and the final answer is generated.
// 
// If parenthesis are found in the input expression, the above process is executed on the subexpression inside the parenthesis
// The resulting answer from the subexpression then replaces the parenthesis and the expression inside in the input expression.
// This is repeated until there are no parenthesis in the arithmetic expression.
// When there are no more parenthesis in the expression, the process above (lines 10-15) is invoked and the final result is generated.
//
// The input arithmetic expression can be changed at line 261 of parsing.cpp
//
#ifndef PARSING_H
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<cmath>
using namespace std;

class Expression 
{

public:
	Expression(string inputExpression); //constructor

	~Expression(); //destructor

	int getPrecedence(string oper); //determines the precedence level (1-4) of an operator

	string evaluate(string oper, string leftVal, string rightVal); //evaluates an expression of two numbers and an operator

	void tokenize(string expr, vector<string> &vec); // tokenizes a string into a vector

	void tokenize(); //tokenizes expr string into vector exprVector

	void printVector(); // prints the contents of the vector

	bool leftPrecedence(string oper1, string oper2); //returns true if the operator the iterator is pointing to has greater precedence than the next operator to the right

	string solve(vector<string> &expressionVector); //solves a subexpression contained in parenthesis from the input expression

	void solveSubExpression(string &expression); //solves a subexpression contained in parenthesis from the input expression 

	bool containsParenthesis(string expression); //returns true if the string expression contains parenthesis

	double solveExpr(); //evalutes an expression of tokens stored in exprVector

	void simplifyParenthesis(); //simplifies all expressions within parenthesis in string exprstring

private:
	vector<string> exprVector; //vector that holds the tokenized expression
	string exprstring; //input string to be evaluated
	double finalAnswer; //result
};

#endif