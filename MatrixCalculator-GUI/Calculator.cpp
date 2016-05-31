#include "Calculator.h"
#include <cstring>
#include <QMap>
#include <QDebug>
#include <QString>
#include <QChar>
#include <QStack>

class Matrix;

Calculator::Calculator()
{

}

Calculator::~Calculator()
{

}


Calculator::Calculator(QMap<QString, Matrix> &mats)
{
    this->matrices = mats;
}

int Calculator::get_next_token()
{
    char *last_pos = NULL;
    int token;

    while (token = *src)
    {
        qDebug() << QChar(token);
        ++src;
        if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_'))
        {
            last_pos = src - 1;

            while (isalnum(*src) || (*src == '_'))
                src++;

            char* tmp = new char[src - last_pos + 1];
            strncpy(tmp, last_pos, src - last_pos);
            tmp[src - last_pos] = '\0';

            qDebug() << "identifier" << QString(tmp);

            if(!matrices.contains(QString(tmp)))
            {
                qDebug() << "undefined identifier :" << tmp;
                return -1;
            }

            mat_val.push(matrices[QString(tmp)]);
            delete []tmp;
            return Id;
        }
        else if (token >= '0' && token <= '9')
        {
            last_pos = src - 1;

            while ((*src >= '0' && *src <= '9') || *src == '.')
                src++;

            char* tmp = new char[src - last_pos + 1];
            strncpy(tmp, last_pos, src - last_pos);
            tmp[src - last_pos] = '\0';

            double value = QString(tmp).toDouble();

            delete []tmp;
            num_val.push(value);
            return Num;
        }
        else if (token == '+') {
            return Add;
        }
        else if (token == '-') {
            return Sub;
        }
        else if (token == '*') {
            return Mul;
        }
        else if(token == '/') {
            return Div;
        }
        else if(token == '|') {
            if(*src != '1') {
                qDebug() << "inverse error";
            }
            src++;
            return Inv;
        }
        else if (token == '(' || token == ')') {
            return token;
        }
    }
    return 0;
}


void Calculator::calc(int token)
{
    Matrix rhs, lhs;
    double lhs_num, rhs_num;
    switch(token)
    {
        case Add :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = mat_val.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = mat_val.pop();
                    value_stack.push(Id);
                    mat_val.push(lhs + rhs);
                }
                else if(value_stack.top() == Num)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs_num = num_val.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs_num = num_val.pop();
                    value_stack.push(Num);
                    num_val.push(lhs_num + rhs_num);
                }
                else if(value_stack.top() == Id)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else
                qDebug() << "add operation error, unknown";
            break;
        }
        case Sub :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = mat_val.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = mat_val.pop();
                    value_stack.push(Id);
                    mat_val.push(lhs - rhs);
                }
                else if(value_stack.top() == Num)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs_num = num_val.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs_num = num_val.pop();
                    value_stack.push(Num);
                    num_val.push(lhs_num - rhs_num);
                }
                else if(value_stack.top() == Id)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else
                qDebug() << "add operation error, unknown";
            break;
        }
        case Mul :
        {

            if(value_stack.top() == Num)
            {
                rhs_num = num_val.pop();
                value_stack.pop();
                if(value_stack.top() == Num)
                {
                    lhs_num = num_val.pop();
                    num_val.push(lhs_num * rhs_num);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = mat_val.pop();
                    mat_val.push(lhs * rhs_num);
                }
                else
                {
                    //handle error
                }
            }
            else if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = mat_val.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs_num = num_val.pop();
                    value_stack.push(Id);
                    mat_val.push(lhs_num * rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = mat_val.pop();
                    mat_val.push(lhs * rhs);
                }
                else
                {
                    //handle error
                }
            }
            break;
        }
        case Div :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = mat_val.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = mat_val.pop();
                    value_stack.push(Id);
                    mat_val.push(lhs / rhs);
                }
                else if(value_stack.top() == Num)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs_num = num_val.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs_num = num_val.pop();
                    value_stack.push(Num);
                    num_val.push(lhs_num / rhs_num);
                }
                else if(value_stack.top() == Id)
                {
                    qDebug() << "add operation error, not matrix";
                }
                else
                    qDebug() << "add operation error, unknown";
            }
            else
                qDebug() << "add operation error, unknown";
            break;
        }
        case Inv :
        {
            if(value_stack.pop() != Id)
                qDebug() << "inverse operation error, not matrix";
            Matrix mat = mat_val.pop();
            value_stack.push(Id);
            mat_val.push(mat.inverse());
            break;
        }
    }
}

Matrix Calculator::calculate(const char* exp)
{
    this->src = new char[strlen(exp) + 1];
    char* s = src;
    strcpy(src, exp);

    symbol_stack.push(-100);
    Matrix rhs, lhs;
    int token;
    while(token = get_next_token())
    {
        if(token == -1)
            return Matrix();
        if(token == Id)
            value_stack.push(Id);
        else if(token == Num)
            value_stack.push(Num);
        else if(token == '(')
            symbol_stack.push('(');
        else if(token == ')')
        {
            while((token = symbol_stack.pop()) != '(')
                calc(token);
        }
        else
        {
            if(token > symbol_stack.top())
                symbol_stack.push(token);
            else
            {     
                while(token <= symbol_stack.top())
                    calc(symbol_stack.pop());
                symbol_stack.push(token);
            }
        }
    }
    while(!symbol_stack.empty())
        calc(symbol_stack.pop());
    if(value_stack.top() == Num)
    {
        qDebug() << num_val.top();
    }
    if(value_stack.top() != Id)
    {
        qDebug() << "result is not matrix";
        return Matrix();
    }
    Matrix res = mat_val.pop();
    res.print();
    delete []s;
    src = NULL;
    return res;
}



