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

struct Token_value
{
    Matrix mat;
    double value = -1;
    int position = -1;
    int offset = 1;

    Token_value& operator = (const Token_value& rhs)
    {
        if(this == &rhs)
            return *this;
        this->mat = rhs.mat;
        this->value = rhs.value;
        this->position = rhs.position;
        this->offset = rhs.offset;
        return *this;
    }

    const Token_value operator + (const Token_value& rhs)
    {
        Token_value res;
        res.mat = this->mat + rhs.mat;
        res.value = this->value + rhs.value;
        res.position = this->position;
        res.offset = rhs.position - this->position + rhs.offset;
        return res;
    }

    const Token_value operator - (const Token_value& rhs)
    {
        Token_value res;
        res.mat = this->mat - rhs.mat;
        res.value = this->value - rhs.value;
        res.position = this->position;
        res.offset = rhs.position - this->position + rhs.offset;
        return res;
    }

    const Token_value operator * (const Token_value& rhs)
    {
        Token_value res;
        res.mat = this->mat * rhs.mat;
        res.value = this->value * rhs.value;
        res.position = this->position;
        res.offset = rhs.position - this->position + rhs.offset;
        return res;
    }

    const Token_value operator / (const Token_value& rhs)
    {
        Token_value res;
        res.mat = this->mat / rhs.mat;
        res.value = this->value / rhs.value;
        res.position = this->position;
        res.offset = rhs.position - this->position + rhs.offset;
        return res;
    }

    const Token_value val_left_mul (const Token_value& lhs)
    {
        Token_value res;
        res.mat = lhs.value * this->mat;
        res.position = lhs.position;
        res.offset = this->position - lhs.position + this->offset;
        return res;
    }

    const Token_value val_right_mul (const Token_value& rhs)
    {
        Token_value res;
        res.mat = this->mat * rhs.value;
        res.position = this->position;
        res.offset = rhs.position - this->position + rhs.offset;
        return res;
    }
};

class Calculator
{

public:
    Calculator();
    ~Calculator();
    Calculator(QMap<QString, Matrix> &mats);
    Matrix calculate(const char* exp);
    bool isValid(const char* exp, int *position, int *offset);

private:
    QMap<QString, Matrix> matrices;

    char *src = NULL, *head = NULL;
    int token;
    QStack<int> symbol_stack;
    QStack<int> value_stack;
    QStack<Token_value> token_values;
    void match(int tk);

    int get_next_token();
    int calc(int token);

    int isValid_helper(int token, int *position, int *offset);
};

#endif // CALCULATOR_H
