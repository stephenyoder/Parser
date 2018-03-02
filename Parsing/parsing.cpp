#include "parsing.h"
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<cmath>
using namespace std;

Expression::Expression(string inputExpression)
{
	exprstring = inputExpression;
}

Expression::~Expression() //destructor
{
	exprVector.clear();
	exprstring = "";
	finalAnswer = 0.0;
}

//determines the precedence level (1-4) of an operator
int Expression::getPrecedence(string oper) {
	int precedence;
	if (oper == "(" || oper == ")")
		precedence = 4;
	else if (oper == "^")
		precedence = 3;
	else if (oper == "/" || oper == "*")
		precedence = 2;
	else if (oper == "-" || oper == "+")
		precedence = 1;
	else //invalid input
		precedence = 0;

	return precedence;
}

//evaluates an expression of two numbers and an operator
string Expression::evaluate(string oper, string leftVal, string rightVal) {
	double answer = 0;
	double leftValue = stod(leftVal);
	double rightValue = stod(rightVal);

	if (oper == "^") 
		answer = pow(leftValue, rightValue);
	else if (oper == "*") 
		answer = leftValue * rightValue;
	else if (oper == "/") 
		answer = leftValue / rightValue;
	else if (oper == "-")
		answer = leftValue - rightValue;
	else if (oper == "+")
		answer = leftValue + rightValue;

	return to_string(answer);
}

//tokenizes a string into a vector
void Expression::tokenize(string expr, vector<string> &vec) {

	string token;
	stringstream iss(expr);

	//get each token from the string separated by a space and add it to the vector
	while (getline(iss, token, ' ')) {
		vec.push_back(token);
	}
}

//tokenizes expr string into vector exprVector
void Expression::tokenize() {

	string token;
	stringstream iss(exprstring);

	//get each token from the string separated by a space and add it to the vector
	while (getline(iss, token, ' ')) {
		exprVector.push_back(token);
	}
}

//prints the contents of the vector separated by a space
void Expression::printVector() {
	for (unsigned i = 0; i < exprVector.size(); i++) {
		cout << exprVector[i] << " ";
	}
}

//returns true if the operator the iterator is pointing to has greater precedence than the next operator to the right
bool Expression::leftPrecedence(string oper1, string oper2) {
	bool precedence;
	if ((getPrecedence(oper1) < getPrecedence(oper2)) || (oper1 == "^" && oper2 == "^"))
		precedence = false;
	else
		precedence = true;

	return precedence;
}

//solves a subexpression contained in parenthesis from the input expression 
void Expression::solveSubExpression(string &expression) {

	string strRightOfOpeningParenthesis;
	string substring;
	vector<string> subexpVector;

	int lastOpeningParenthesis = expression.find_last_of("("); //find the location of the last ( to ensure the innermost subexpression will be evaluated
	strRightOfOpeningParenthesis = expression.substr(lastOpeningParenthesis, expression.length() + 1); //creates a new string of substring that is right of the last (
	int closingParenthesis = strRightOfOpeningParenthesis.find_first_of(")"); //finds the location of the first ) that is right of the last (
	substring = strRightOfOpeningParenthesis.substr(1, closingParenthesis - 1); //retrieves substring of epression inside the parenthesis
	tokenize(substring, subexpVector); //tokenizes substring
	substring = solve(subexpVector); //solves tokenized substring and returns it as a string

	expression.replace(lastOpeningParenthesis, closingParenthesis + 1, substring, 0, substring.length()); //updates the substring with the value evaluated within the parenthesis
}

//evaluates an exprssion of tokens stored in a vector
string Expression::solve(vector<string> &expVec) { //double check this
	string tempAnswer;
	string finalAnswer;
	bool resetIterator = false; //when true, sets the iterator to first operator at position [1] or begin() + 1
	
	if (expVec.size() == 3) { //if expression is just to values and an operator (ex: 2 + 2), just evaluate expression
		finalAnswer = evaluate(expVec.at(1), expVec.at(0), expVec.at(2));
	}
	else { //to execute when the size of the initial vector is greater than 3
		   //each iteration of the for loop below will simplify the expression and decrease the size of the vector until the vector's size is 3. When that condition occurs, the last expression will be evaluated
		for (vector<string>::iterator i = expVec.begin() + 1; i <= expVec.end() - 2; i += 2) { //simplifies expression each loop. iterator jumps by 2 b/c every other vector element is an expression
			
			//resets iterator to first operator or position vectorExpression[1] or begin() + 1
			if (resetIterator) {
				i = expVec.begin() + 1;
				resetIterator = false;
			}

			if (leftPrecedence(expVec[distance(expVec.begin(), i)], expVec[distance(expVec.begin(), i) + 2])) { //true if the left operator has precedence over the right operator																						//following three lines evaluate an expression and replace the elements. Ex: 2 * 2 + 4 becomes 4 + 4

				//next 3 lines evaluates the expression the iterator was pointing to and modifies the vector with the new value
				tempAnswer = evaluate(expVec.at(distance(expVec.begin(), i)), expVec.at(distance(expVec.begin(), i) - 1), expVec.at(distance(expVec.begin(), i) + 1));
				i = expVec.insert(i + 2, 1, tempAnswer) - 1; //inserts new answer into vector
				i = expVec.erase(i - 2, i + 1) + 1; //erases the old values in the vector and resets the iterator position

				//if the iterator at the penultimate operator, reset the iterator
				if (expVec.size() > 3 && i >= expVec.end() - 4) {
					i = expVec.begin();
					resetIterator = true;
				}
			}
			//following line throws vector iterators incompatible error
			//this condition is if all operators do not have precedence over the operator to the right. ex: 2 + 2 * 2 ^ 2. in that case, evaluate the last subexpression

			//case where the iterator reaches the penultimate operator in the expression and it does not have precedence over the last operator
			else if ((i == expVec.end() - 4) && (!leftPrecedence(expVec.at(distance(expVec.begin(), i)), expVec.at(distance(expVec.begin(), i) + 2)))) {

				//next few lines replace the last subexpression. ex: 2 + 2 * 2 ^ 2 becomes 2 + 2 * 4
				tempAnswer = evaluate(expVec.at(distance(expVec.begin(), i) + 2), expVec.at(distance(expVec.begin(), i) + 1), expVec.at(distance(expVec.begin(), i) + 3));
				expVec.push_back(tempAnswer);
				i = expVec.erase(i + 1, i + 4);
				resetIterator = true; //will reset the iterator to the first operator when the for loop executes
				i = expVec.begin(); //resets the iterator so when it is incremented by 2 from the for loop, iterator does not go out of range
			}

			//Because iterator i will be incremented by 2 before the conditional statement in the for loop is executed, break to prevent iterator from going out of range
			if (expVec.size() <= 3) {
				break;
			}

			//reset the iterator if the iterator will go out of range the next execution of the for loop
			if (i >= expVec.end() - 2) {
				resetIterator = true;
				i = expVec.begin();
			}
		}
		finalAnswer = evaluate(expVec.at(1), expVec.at(0), expVec.at(2)); //evaluate expression when the size of the vector is down to 3 (ex: 2 + 2)
	}
	return finalAnswer;
}

//returns true if the expression contains parenthesis
bool Expression::containsParenthesis(string expression) {
	if (expression.find("(") != string::npos)
		return true;
	else
		return false;
}

//evalutes an expression of tokens stored in exprVector
double Expression::solveExpr() {
	string tempAnswer;
	string finalAnswer;
	bool resetIterator = false; //when true, sets the iterator to first operator at position [1] or begin() + 1

	simplifyParenthesis(); //simplifies expression by evaluating and replacing expressions within parenthesis into exprstring 
	tokenize(); //tokenizes the simplified string expression into a vector of strings

	if (exprVector.size() == 3) { //if expression is just to values and an operator (ex: 2 + 2), just evaluate expression
		finalAnswer = evaluate(exprVector.at(1), exprVector.at(0), exprVector.at(2));
	}
	else { //to execute when the size of the initial vector is greater than 3
		   //each iteration of the for loop below will simplify the expression and decrease the size of the vector until the vector's size is 3. When that condition occurs, the last expression will be evaluated
		for (vector<string>::iterator i = exprVector.begin() + 1; i <= exprVector.end() - 2; i += 2) { //simplifies expression each loop. iterator jumps by 2 b/c every other vector element is an expression
			
			//resets iterator to first operator or position vectorExpression[1] or begin() + 1
			if (resetIterator) {
				i = exprVector.begin() + 1;
				resetIterator = false;
			}

			//true if the left operator has precedence over the right operator
			if (leftPrecedence(exprVector.at(distance(exprVector.begin(), i)), exprVector.at(distance(exprVector.begin(), i) + 2))) { 																						
				
				//following three lines evaluate an expression and replace the elements. Ex: 2 * 2 + 4 becomes 4 +																																	//next 3 lines evaluates the expression the iterator was pointing to and modifies the vector with the new value
				tempAnswer = evaluate(exprVector.at(distance(exprVector.begin(), i)), exprVector.at(distance(exprVector.begin(), i) - 1), exprVector.at(distance(exprVector.begin(), i) + 1));
				i = exprVector.insert(i + 2, 1, tempAnswer) - 1; //inserts new answer into vector
				i = exprVector.erase(i - 2, i + 1) + 1; //erases the old values in the vector and resets the iterator position

				if (exprVector.size() > 3 && i >= exprVector.end() - 4) { //if the iterator at the penultimate operator, reset the iterator
					i = exprVector.begin(); //resets to beginning to prevent iterator from going out of range when i is icremented
					resetIterator = true;
				}
			}

			//this condition is if all operators do not have precedence over the operator to the right. ex: 2 + 2 * 2 ^ 2. in that case, evaluate the last subexpression: 2 ^ 2
			//case where the iterator reaches the penultimate operator in the expression and it does not have precedence over the last operator
			else if ((i == exprVector.end() - 4) && (!leftPrecedence(exprVector.at(distance(exprVector.begin(), i)), exprVector.at(distance(exprVector.begin(), i) + 2)))) {

				//next few lines replace the last subexpression. ex: 2 + 2 * 2 ^ 2 becomes 2 + 2 * 4
				tempAnswer = evaluate(exprVector.at(distance(exprVector.begin(), i) + 2), exprVector.at(distance(exprVector.begin(), i) + 1), exprVector.at(distance(exprVector.begin(), i) + 3));
				exprVector.push_back(tempAnswer);
				i = exprVector.erase(i + 1, i + 4);
				resetIterator = true; //will reset the iterator to the first operator when the for loop executes
				i = exprVector.begin(); //resets the iterator so when it is incremented by 2 from the for loop, iterator does not go out of range
			}

			//Because iterator i will be incremented by 2 before the conditional statement in the for loop is executed, break to prevent iterator from going out of range
			if (exprVector.size() <= 3) {
				break;
			}

			//reset the iterator if the iterator will go out of range the next execution of the for loop
			if (i >= exprVector.end() - 2) {
				resetIterator = true;
				i = exprVector.begin();
			}
		}
		finalAnswer = evaluate(exprVector.at(1), exprVector.at(0), exprVector.at(2)); //evaluate expression when the size of the vector is down to 3 (ex: 2 + 2)
	}
	return stod(finalAnswer);
}

//simplifies all expressions within parenthesis in string exprstring
void Expression::simplifyParenthesis() {
	while (containsParenthesis(exprstring)) {
		solveSubExpression(exprstring);
	}
}

int main() {
	
	string exprstring = "2 * 2"; //input expression. Note: if doing a power operation (^), the base of the exponent must not be less than 0 or it will return NaN

	Expression expr(exprstring); //Expression object created

	cout << expr.solveExpr() << endl; //Result of evaluated expression (double data type)

	return 0;
}