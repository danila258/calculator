#ifndef RPN_H
#define RPN_H


#include <string>
#include <stack>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <iomanip>

void printAnswer(const std::string& userInput);

std::string convertToRPN(const std::string& userInput);
int operationPower(char operation);

std::string calculateExample(const std::string& rpnLine);
double stackPop(std::stack<double>& valuesStack);

char getElement(int& pos, const std::string& line);
std::string getNextNum(int& pos, const std::string& line);
void getNumberPart(int& pos, const std::string& line, char& curElem, std::string& num);


#endif //RPN_H