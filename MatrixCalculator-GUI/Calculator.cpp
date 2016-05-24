#include "Calculator.h"
#include <cstring>
#include <QMap>
#include <QDebug>
#include <QString>
#include <QChar>

class Matrix;

Calculator::Calculator()
{

}

Calculator::~Calculator()
{

}


Calculator::Calculator(QMap<QString, Matrix> &matrices_)
{
    this->matrices = matrices_;
}

Matrix Calculator::calculate(char *exp)
{
    this->src = new char[strlen(exp) + 1];
    char* s = src;
    strcpy(src, exp);
    next();
    Matrix res = expr();
    res.print();
    delete []s;
    src = NULL;
    return res;
}

void Calculator::next()
{
    char *last_pos = NULL;
    int hash;

    while (token = *src)
    {
        qDebug() << QChar(token);
        ++src;
        if(token == '#')
        {
            // skip comment
            while (*src != 0 && *src != '\n') {
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
        {
            last_pos = src - 1;
            hash = token;

            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_'))
            {
                hash = hash * 147 + *src;
                src++;
            }
            char* tmp = new char[src - last_pos + 1];
            strncpy(tmp, last_pos, src - last_pos);
            tmp[src - last_pos] = '\0';

            qDebug() << "identifier" << QString(tmp);

            if(!matrices.contains(QString(tmp)))
            {
                qDebug() << "undefined identifier :" << tmp;
            }
            mat = matrices[QString(tmp)];
            token = Id;
            return;
        }
        else if (token >= '0' && token <= '9')
        {
            // parse number
            token_val = token - '0';

            while (*src >= '0' && *src <= '9') {
                token_val = token_val * 10 + *src++ - '0';
            }

            token = Num;
            return;
        }
        else if (token == '+') {
            //token = Add;
            return;
        }
        else if (token == '-') {
            //token = Sub;
            return;
        }
        else if (token == '*') {
            //token = Mul;
            return;
        }
        else if(token == '/') {
            //token = Div;
            return;
        }
        else if(token == '|') {
            if(*src == '1') {
                src++;
                token = Inv;
            }
            else {
                qDebug() << "inverse expression error";
            }
            return;
        }
        else if (token == '(' || token == ')') {
            // directly return the character as token;
            return;
        }
    }
    return;
}

void Calculator::match(int tk)
{
    if (token != tk) {
        qDebug() << QString("expected token: %1(%2), got: %3(%4)\n").arg(QString::number(tk), QChar(tk), QString::number(token), QChar(token));
    }
    next();
}

Matrix Calculator::factor()
{

    Matrix value;
    if (token == '(') {
        match('(');
        value = expr();
        match(')');
    }
    else {
        value = mat;
        match(Id);
    }
    return value;
}

Matrix Calculator::term_tail(Matrix lvalue)
{
    if (token == '*')
    {
        match('*');
        Matrix value = lvalue * factor();
        return term_tail(value);
    }
    else if (token == '/')
    {
        match('/');
        Matrix value = lvalue / factor();
        return term_tail(value);
    }
    else
    {
        return lvalue;
    }
}

Matrix Calculator::term()
{
    Matrix lvalue = factor();
    return term_tail(lvalue);
}

Matrix Calculator::expr_tail(Matrix lvalue)
{

    if (token == '+')
    {
        match('+');
        Matrix value = lvalue + term();
        return expr_tail(value);
    }
    else if (token == '-')
    {
        match('-');
        Matrix value = lvalue - term();
        return expr_tail(value);
    }
    else
    {
        return lvalue;
    }
}

Matrix Calculator::expr()
{
    Matrix lvalue = term();
    return expr_tail(lvalue);
}
