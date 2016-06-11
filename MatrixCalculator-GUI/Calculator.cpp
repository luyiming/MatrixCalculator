#include "Calculator.h"
#include "Matrix.h"
#include <cstring>
#include <QMap>
#include <QDebug>
#include <QString>
#include <QChar>
#include <QStack>
#include <QMessageBox>

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

            Token_value tk_val;
            tk_val.position = src - 1 - head;

            while (isalnum(*src) || (*src == '_'))
                src++;
            QString matName = QString::fromLatin1(last_pos, src - last_pos);
            qDebug() << "identifier" << matName;
            if(!matrices.contains(matName))
            {
                QMessageBox::critical(NULL, QString("错误"), QString("未定义的矩阵名 : %1").arg(matName));
                return -1;
            }

            tk_val.mat = matrices[matName];
            tk_val.offset = src - head - tk_val.position;
            token_values.push(tk_val);

            return Id;
        }
        else if (token >= '0' && token <= '9')
        {
            last_pos = src - 1;

            Token_value tk_val;
            tk_val.position = src - 1 - head;

            while ((*src >= '0' && *src <= '9') || *src == '.')
                src++;
            QString str = QString::fromLatin1(last_pos, src - last_pos);
            bool ok;
            double value = str.toDouble(&ok);
            if(!ok)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("%1 数字格式错误").arg(str));
                return -1;
            }

            tk_val.offset = src - head - tk_val.position;
            tk_val.value = value;
            token_values.push(tk_val);

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
                QMessageBox::critical(NULL, QString("错误"), QString("矩阵求逆符号错误"));
                return -1;
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


int Calculator::calc(int token)
{
    Token_value rhs, lhs;
    switch(token)
    {
        case Add :
        {
//            if(value_stack.isEmpty())
//            {
//                QMessageBox::critical(NULL, QString("错误"), QString("加法+缺少操作数 %1 <----").arg(QString::fromLatin1(head, src - head)));
//                return -1;
//            }
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '+'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("加法阶数不匹配"));
                        return -1;
                    }
                    token_values.push(lhs + rhs);
                }
                else if(value_stack.top() == Num)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("加法两边不是矩阵"));
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs + rhs);
                }
                else if(value_stack.top() == Id)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("加法两边不是矩阵"));
                    return -1;
                }
            }
            else
            {
                QMessageBox::critical(NULL, QString("错误"), QString("加法未知错误"));
                return -1;
            }
            break;
        }
        case Sub :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '-'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("减法阶数不匹配"));
                        return -1;
                    }
                    token_values.push(lhs - rhs);
                }
                else if(value_stack.top() == Num)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("减法两边不是矩阵"));
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs - rhs);
                }
                else if(value_stack.top() == Id)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("减法两边不是矩阵"));
                    return -1;
                }
            }
            else
            {
                QMessageBox::critical(NULL, QString("错误"), QString("减法未知错误"));
                return -1;
            }
            break;
        }
        case Mul :
        {

            if(value_stack.top() == Num)
            {
                rhs = token_values.pop();
                value_stack.pop();
                if(value_stack.top() == Num)
                {
                    lhs = token_values.pop();
                    token_values.push(lhs * rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    token_values.push(lhs.val_right_mul(rhs));
                }
                else
                {
                    //handle error
                }
            }
            else if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    token_values.push(rhs.val_left_mul(lhs));
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    if(!isValid2(lhs.mat, rhs.mat, '*'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("乘法阶数不匹配"));
                        return -1;
                    }
                    token_values.push(lhs * rhs);
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
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '/'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("除法阶数不匹配"));
                        return -1;
                    }
                    token_values.push(lhs / rhs);
                }
                else if(value_stack.top() == Num)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("除法两边不是矩阵"));
                    return -1;
                }
                else
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("除法:未知错误"));
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs / rhs);
                }
                else if(value_stack.top() == Id)
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("除法两边不是矩阵"));
                    return -1;
                }
                else
                {
                    QMessageBox::critical(NULL, QString("错误"), QString("除法:未知错误"));
                    return -1;
                }
            }
            else
            {
                QMessageBox::critical(NULL, QString("错误"), QString("除法:未知错误"));
                return -1;
            }
            break;
        }
        case Inv :
        {
            if(value_stack.pop() != Id)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("求逆运算操作数不是矩阵"));
                return -1;
            }
            rhs = token_values.pop();
            value_stack.push(Id);
            if(rhs.mat.det() == 0)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("矩阵无逆矩阵"));
                return -1;
            }
            if(rhs.mat.row != rhs.mat.column)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("矩阵无逆矩阵"));
                return -1;
            }
            rhs.mat = rhs.mat.inverse();
            token_values.push(rhs);
            break;
        }
    }
    return 1;
}

Matrix Calculator::calculate(const char* exp)
{
    this->src = new char[strlen(exp) + 1];
    head = src;
    strcpy(src, exp);

    symbol_stack.clear();
    token_values.clear();

    symbol_stack.push(-100);
    Matrix rhs, lhs;
    int token;
    while(token = get_next_token())
    {
        if(token == -1)
        {
            return Matrix();
        }
        if(token == Id)
            value_stack.push(Id);
        else if(token == Num)
            value_stack.push(Num);
        else if(token == '(')
            symbol_stack.push('(');
        else if(token == ')')
        {
            bool ok = false;
            while(!symbol_stack.isEmpty())
            {
                token = symbol_stack.pop();
                if(token == '(')
                {
                    ok = true;
                    break;
                }
                if(calc(token) == -1)
                    return Matrix();
            }
            if(!ok)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("缺少左括号"));
                return Matrix();
            }

        }
        else
        {
            if(token > symbol_stack.top())
                symbol_stack.push(token);
            else
            {     
                while(token <= symbol_stack.top())
                    if(calc(symbol_stack.pop()) == -1)
                        return Matrix();
                symbol_stack.push(token);
            }
        }
    }
    while(!symbol_stack.empty())
    {
        token = symbol_stack.pop();
        if(token == '(')
        {
            QMessageBox::critical(NULL, QString("错误"), QString("缺少右括号"));
            return Matrix();
        }
        if(calc(token) == -1)
            return Matrix();
    }

    Matrix res;
    if(value_stack.top() == Num)
    {
        res.resize(1, 1);
        res[0][0] = token_values.pop().value;
    }
    else if(value_stack.top() == Id)
    {
        res = token_values.pop().mat;
        res.print();
    }

    delete []head;
    src = NULL;
    return res;
}

bool Calculator::isValid(const char* exp, int *position, int *offset)
{
    *position = -1;
    *offset = 0;
    this->src = new char[strlen(exp) + 1];
    head = src;
    strcpy(src, exp);

    symbol_stack.clear();
    token_values.clear();

    symbol_stack.push(-100);
    Matrix rhs, lhs;
    int token;
    while(token = get_next_token())
    {
        if(token == -1) //undefined matrix
        {
            return false;
        }
        if(token == Id)
            value_stack.push(Id);
        else if(token == Num)
            value_stack.push(Num);
        else if(token == '(')
            symbol_stack.push('(');
        else if(token == ')')
        {
            bool ok = false;
            while(!symbol_stack.isEmpty())
            {
                token = symbol_stack.pop();
                if(token == '(')
                {
                    ok = true;
                    break;
                }
                if(isValid_helper(token, position, offset) == -1)
                    false;
            }
            if(!ok)
            {
                return false;
            }
        }
        else
        {
            if(token > symbol_stack.top())
                symbol_stack.push(token);
            else
            {
                while(token <= symbol_stack.top())
                    if(isValid_helper(symbol_stack.pop(), position, offset) == -1)
                        return false;
                symbol_stack.push(token);
            }
        }
    }

    while(!symbol_stack.empty())
    {
        token = symbol_stack.pop();
        if(token == '(')
        {
            return false;
        }
        if(isValid_helper(token, position, offset) == -1)
            return false;
    }


    delete []head;
    src = NULL;
    return true;
}

int Calculator::isValid_helper(int token, int *position, int *offset)
{
    *position = -1;
    *offset = 0;
    Token_value rhs, lhs;
    switch(token)
    {
        case Add :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '+'))
                    {
                        *position = lhs.position;
                        *offset = rhs.position - lhs.position + rhs.offset;
                        return -1;
                    }
                    token_values.push(lhs + rhs);
                }
                else if(value_stack.top() == Num)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs + rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else
            {
                return -1;
            }
            break;
        }
        case Sub :
        {
            if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '-'))
                    {
                        *position = lhs.position;
                        *offset = rhs.position - lhs.position + rhs.offset;
                        return -1;
                    }
                    token_values.push(lhs - rhs);
                }
                else if(value_stack.top() == Num)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs - rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else
            {
                return -1;
            }
            break;
        }
        case Mul :
        {

            if(value_stack.top() == Num)
            {
                rhs = token_values.pop();
                value_stack.pop();
                if(value_stack.top() == Num)
                {
                    lhs = token_values.pop();
                    token_values.push(lhs * rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    token_values.push(lhs.val_right_mul(rhs));
                }
                else
                {
                    //handle error
                }
            }
            else if(value_stack.top() == Id)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    token_values.push(rhs.val_left_mul(lhs));
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    if(!isValid2(lhs.mat, rhs.mat, '*'))
                    {
                        *position = lhs.position;
                        *offset = rhs.position - lhs.position + rhs.offset;
                        return -1;
                    }
                    token_values.push(lhs * rhs);
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
                rhs = token_values.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs.mat, rhs.mat, '/'))
                    {
                        *position = lhs.position;
                        *offset = rhs.position - lhs.position + rhs.offset;
                        return -1;
                    }
                    token_values.push(lhs / rhs);
                }
                else if(value_stack.top() == Num)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
                else
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else if(value_stack.top() == Num)
            {
                value_stack.pop();
                rhs = token_values.pop();
                if(value_stack.top() == Num)
                {
                    value_stack.pop();
                    lhs = token_values.pop();
                    value_stack.push(Num);
                    token_values.push(lhs / rhs);
                }
                else if(value_stack.top() == Id)
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
                else
                {
                    lhs = token_values.pop();
                    *position = lhs.position;
                    *offset = rhs.position - lhs.position + rhs.offset;
                    return -1;
                }
            }
            else
            {
                return -1;
            }
            break;
        }
        case Inv :
        {
            if(value_stack.pop() != Id)
            {
                rhs = token_values.pop();
                *position = rhs.position;
                *offset = rhs.offset + 2;
                return -1;
            }
            rhs = token_values.pop();
            value_stack.push(Id);
            if(rhs.mat.det() == 0)
            {
                *position = rhs.position;
                *offset = rhs.offset + 2;
                return -1;
            }
            if(rhs.mat.row != rhs.mat.column)
            {
                *position = rhs.position;
                *offset = rhs.offset + 2;
                return -1;
            }
            rhs.mat.inverse();
            token_values.push(rhs);
            break;
        }
    }
    return 1;
}
