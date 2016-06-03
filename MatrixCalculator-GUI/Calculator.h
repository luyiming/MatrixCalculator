#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Matrix.h"
#include <QMap>
#include <QStack>
#include <QObject>
#include "MatrixTableWidget.h"
#include "ui_MatrixTableWidget.h"

enum {
  Num = 128, Id, Add, Sub, Mul, Div, Inv,/*inverse*/ Brak
};

class Calculator
{

public:
    Calculator();
    ~Calculator();
    Calculator(QMap<QString, Matrix> &mats);
    Matrix calculate(const char* exp);
    bool isValid(const char* exp);

private:
    QMap<QString, Matrix> matrices;

    char *src = NULL, *head = NULL;
    int token;
    QStack<int> symbol_stack;
    QStack<int> value_stack;
    QStack<Matrix> mat_val;
    QStack<double> num_val;
    void match(int tk);

    int get_next_token();
    int calc(int token);
};

#endif // CALCULATOR_H
