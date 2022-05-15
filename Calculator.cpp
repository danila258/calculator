#include "Calculator.h"

Calculator::Calculator (QWidget* parent) : QWidget(parent) {
    setWindowTitle("Calculator");
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);

    resize(500, 300);
    setFixedHeight(290);

    _exampleLineEdit = new QLineEdit();
    _exampleLineEdit->setMinimumSize(150, 40);
    _exampleLineEdit->setStyleSheet("font-size: 30px");

    connect(_exampleLineEdit, SIGNAL(textChanged(const QString&)), SLOT(slotExampleEdited(const QString&)));

    QString aButtons [5][5] = { {"pi", "(", ")", "^", "CE"},
                                {"sin", "7", "8", "9", "/"},
                                {"cos", "4", "5", "6", "*"},
                                {"tg", "1", "2", "3", "-"},
                                {"ctg", "0", ",", "=", "+"} };

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(_exampleLineEdit, 0, 0, 1, 5);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            layout->addWidget(createButton(aButtons[i][j]), i + 1, j);
        }
    }

    setLayout(layout);
}

QPushButton* Calculator::createButton(const QString& str) {
    QPushButton* calcButton = new QPushButton(str);
    connect (calcButton, SIGNAL(clicked()), SLOT(slotButtonClicked()));
    calcButton->setMinimumSize(40, 40);

    if (str == "CE") {
        calcButton->setPalette(Qt::red);
    }
    else if (str == "=") {
        calcButton->setPalette(Qt::green);
    }

    return calcButton;
}

QString Calculator::calculate() {
    std::string rpnLine = convertToRPN(_userInput.toStdString());

    if (rpnLine == "error") {
        return QString::fromStdString(rpnLine);
    }

    std::string answer = calculateExample(rpnLine);

    for (int i = 0; i < answer.size(); ++i) {
        if (answer[i] == ',') {
            if (answer[i + 1] == '0' && answer[i + 2] == '0') {
                answer.erase(i, answer.size() - 1);
            }
            else if (answer[i + 2] == '0') {
                answer.erase(i + 2, answer.size() - 1);
            }
            else {
                answer.erase(i + 3, answer.size() - 1);
            }
        }
    }

    return QString::fromStdString(answer);
}

void Calculator::slotButtonClicked() {
    QString str = ((QPushButton*) sender())->text();

    if (str == "CE") {
        _exampleLineEdit->clear();
        _userInput.clear();
    }
    else if (str == '=') {
        _userInput = calculate();
        _exampleLineEdit->setText(_userInput);
    }
    else {
        _userInput += str;
        _exampleLineEdit->setText(_userInput);
    }
}

void Calculator::slotExampleEdited(const QString& line) {
    _userInput = line;
}

Calculator::~Calculator() {
    delete _exampleLineEdit;
}
