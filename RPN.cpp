#include "RPN.h"

enum { NUMBER, SIN, COS, TAN, COT, PI, EXIT };
enum { FIRST, SECOND, THIRD };

void printAnswer(const std::string& userInput) {
    std::string rpnLine = convertToRPN(userInput);
    double answer = std::stoi(calculateExample(rpnLine));

    std::cout << rpnLine << '\n';
    std::cout << std::fixed << std::setprecision(2) << answer << '\n';
}

std::string convertToRPN(const std::string& userInput) {
    std::stack<char> operationsStack;
    std::string line;
    std::string num;
    int pos = 0;
    char type;
    char typeTwo;
    bool valueFlag;

    while ( pos < userInput.size() ) {
        type = getElement(pos, userInput);

        if (type == NUMBER) {
            num = getNextNum(pos, userInput);
            line += num + ' ';
            valueFlag = true;
            continue;
        }

        if (type == PI) {
            line += "3,14 ";
            valueFlag = true;
            continue;
        }

        if (!operationsStack.empty() && operationsStack.top() != ')' && !valueFlag && type == '-' &&
            (line[line.size() - 2] != '-' || operationsStack.top() == '(') || (type == '-' && line.empty())) {
            line += "0 ";
        }

        valueFlag = false;

        if (operationsStack.empty() || type == '(') {
            if (type == ')') {
                return "error";
            }

            operationsStack.push(type);
            continue;
        }

        if (type == ')') {
            while (operationsStack.top() != '(') {
                if (operationsStack.size() > 0) {
                    line.push_back(operationsStack.top());
                    line.push_back(' ');
                    operationsStack.pop();
                }
                else {
                    return "error";
                }
            }

            operationsStack.pop();
            valueFlag = true;
            continue;
        }

        typeTwo = operationsStack.top();

        if (typeTwo == '(') {
            operationsStack.push(type);
            continue;
        }

        try {
            while (true) {
                if (operationPower(type) <= operationPower(typeTwo)) {
                    line.push_back(typeTwo);
                    line.push_back(' ');
                    operationsStack.pop();

                    if ( operationsStack.empty() ) {
                        operationsStack.push(type);
                        break;
                    }

                    typeTwo = operationsStack.top();

                    if (typeTwo == '(') {
                        operationsStack.push(type);
                        break;
                    }
                }
                else {
                    operationsStack.push(type);
                    break;
                }
            }
        }

        catch (const char* line) {
            return line;
        }
    }

    while ( !operationsStack.empty() ) {
        if (operationsStack.top() == '(') {
            return "error";
        }

        line.push_back(operationsStack.top());
        line.push_back(' ');
        operationsStack.pop();
    }

    for (int i = 0; i < line.size(); ++i) {        //for RPN visualisation, program work without this
        if (line[i] == SIN) {
            line.replace(i, 1, "sin");
        }
        else if (line[i] == COS) {
            line.replace(i, 1, "cos");
        }
        else if (line[i] == TAN) {
            line.replace(i, 1, "tg");
        }
        else if (line[i] == COT) {
            line.replace(i, 1, "ctg");
        }
    }

    return line;
}

int operationPower(char operation) {
    if (operation == '+' || operation == '-') {
        return FIRST;
    }
    else if (operation == '*' || operation == '/') {
        return SECOND;
    }
    else if (operation == '^' || operation == '(' || operation == SIN || operation == COS || operation == TAN
             || operation == COT) {

        return THIRD;
    }

    throw "error";
}

std::string calculateExample(const std::string& rpnLine) {
    std::stack<double> valuesStack;
    int pos = 0;
    char type;
    double operator2;

    while ( pos < rpnLine.size() ) {
        type = getElement(pos, rpnLine);

        try {
            switch (type) {
                case NUMBER:
                    valuesStack.push(std::stod(getNextNum(pos, rpnLine)));
                    break;

                case SIN:
                    valuesStack.push(std::sin( stackPop(valuesStack) ));
                    break;

                case COS:
                    valuesStack.push(std::cos( stackPop(valuesStack) ));
                    break;

                case TAN:
                    valuesStack.push(std::tan( stackPop(valuesStack) ));
                    break;

                case COT:
                    valuesStack.push(1 / std::tan( stackPop(valuesStack) ));
                    break;

                case '+':
                    valuesStack.push(stackPop(valuesStack) + stackPop(valuesStack));
                    break;

                case '*':
                    valuesStack.push(stackPop(valuesStack) * stackPop(valuesStack));
                    break;

                case '-':
                    operator2 = stackPop(valuesStack);
                    valuesStack.push(stackPop(valuesStack) - operator2);
                    break;

                case '/':
                    operator2 = stackPop(valuesStack);
                    valuesStack.push(stackPop(valuesStack) / operator2);
                    break;

                case '^':
                    operator2 = stackPop(valuesStack);
                    valuesStack.push(std::pow(stackPop(valuesStack), operator2));
                    break;

                case EXIT:
                    break;

                default:
                    return "error";
            }
        }

        catch (const char* line) {
            return line;
        }
    }

    if (valuesStack.size() != 1) {
        return "error";
    }

    std::string answer = std::to_string(stackPop(valuesStack));
    return answer;
}

char getElement(int& pos, const std::string& line) {
    char curElem;

    while ((curElem = line[pos]) == ' ') {
        ++pos;
    }

    if (line.substr(pos, 3) == "sin") {
        pos += 3;
        return SIN;
    }

    if (line.substr(pos, 3) == "cos") {
        pos += 3;
        return COS;
    }

    if (line.substr(pos, 2) == "tg") {
        pos += 2;
        return TAN;
    }

    if (line.substr(pos, 3) == "ctg") {
        pos += 3;
        return COT;
    }

    if (line.substr(pos, 2) == "pi") {
        pos += 2;
        return PI;
    }

    if (!isdigit(curElem) && curElem != '.') {
        if (pos >= line.size()) {
            return EXIT;
        }

        ++pos;
        return curElem;
    }

    return NUMBER;
}

std::string getNextNum(int& pos, const std::string& line) {
    char curElem;
    std::string num;

    getNumberPart(pos, line, curElem, num);

    if (curElem == ',') {
        num.push_back(curElem);
        ++pos;

        getNumberPart(pos, line, curElem, num);
    }

    return num;
}

void getNumberPart(int& pos, const std::string& line, char& curElem, std::string& num) {
    while( isdigit(curElem = line[pos]) ) {
        num.push_back(curElem);
        ++pos;
    }
}

double stackPop(std::stack<double>& valuesStack) {
    if ( valuesStack.empty() ) {
        throw "error";
    }

    double buf = valuesStack.top();
    valuesStack.pop();
    return buf;
}
