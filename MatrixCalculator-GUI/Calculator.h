#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Matrix.h"
#include <QMap>

enum {
  Num = 128, Id, Add, Sub, Mul, Div, Inv,/*inverse*/ Brak
};

class Calculator
{
public:
    Calculator();
    ~Calculator();
    Calculator(QMap<QString, Matrix> &matrices_);
    Matrix calculate(char* exp);

private:
    void next();
    QMap<QString, Matrix> matrices;
    char* src = NULL;
    int token;
    int token_val;                // value of current token (mainly for number)
    Matrix mat;

    void match(int tk);
    Matrix factor();
    Matrix term_tail(Matrix lvalue);
    Matrix term();
    Matrix expr_tail(Matrix lvalue);
    Matrix expr();
};

#endif // CALCULATOR_H
