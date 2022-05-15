#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "RPN.h"
#include <QtWidgets>


class Calculator : public QWidget {
    Q_OBJECT

public:
    explicit Calculator(QWidget* parent = nullptr);
    ~Calculator();

private:
    QLineEdit* _exampleLineEdit;
    QString _userInput;

    QPushButton* createButton(const QString& str);
    QString calculate();

public slots:
    void slotButtonClicked();
    void slotExampleEdited(const QString& line);

};


#endif //CALCULATOR_H
