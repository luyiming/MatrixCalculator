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
            while (isalnum(*src) || (*src == '_'))
                src++;
            QString matName = QString::fromLatin1(last_pos, src - last_pos);
            qDebug() << "identifier" << matName;
            if(!matrices.contains(matName))
            {
                QMessageBox::critical(NULL, QString("错误"), QString("未定义的矩阵名 : %1").arg(matName));
                return -1;
            }
            mat_val.push(matrices[matName]);
            return Id;
        }
        else if (token >= '0' && token <= '9')
        {
            last_pos = src - 1;

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
                QString str = QString::fromLatin1(head, src - head);
                QMessageBox::critical(NULL, QString("错误"), QString("矩阵求逆符号错误,位置为 %1 <---").arg(str));
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
    Matrix rhs, lhs;
    double lhs_num, rhs_num;
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
                rhs = mat_val.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = mat_val.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs, rhs, '+'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("加法阶数不匹配"));
                        return -1;
                    }
                    mat_val.push(lhs + rhs);
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
                rhs = mat_val.pop();
                if(value_stack.top() == Id)
                {
                    value_stack.pop();
                    lhs = mat_val.pop();
                    value_stack.push(Id);
                    if(!isValid2(lhs, rhs, '-'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("减法阶数不匹配"));
                        return -1;
                    }
                    mat_val.push(lhs - rhs);
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
                    if(!isValid2(lhs, rhs, '*'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("乘法阶数不匹配"));
                        return -1;
                    }
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
                    if(!isValid2(lhs, rhs, '/'))
                    {
                        QMessageBox::critical(NULL, QString("错误"), QString("除法阶数不匹配"));
                        return -1;
                    }
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
            if(mat.det() == 0)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("矩阵无逆矩阵, %1 <----").arg(QString::fromLatin1(head, src - head)));
                return -1;
            }
            mat_val.push(mat.inverse());
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
            while((token = symbol_stack.pop()) != '(')
                if(calc(token) == -1)
                    return Matrix();
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
        if(calc(symbol_stack.pop()) == -1)
            return Matrix();
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
    delete []head;
    src = NULL;
    return res;
}

bool Calculator::isValid(const char* exp)
{
    this->src = new char[strlen(exp) + 1];
    head = src;
    strcpy(src, exp);

    QStack<int> st;
    int prevToken, token;
    while(token = get_next_token())
    {
        if(token == -1)
        {
            return false;
        }
        if(token == Id)
        {
            if(prevToken == Num)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("数字和矩阵之间缺少运算符"));
                return false;
            }
            prevToken = Id;
        }
        else if(token == Num)
        {
            if(prevToken == Id)
            {
                QMessageBox::critical(NULL, QString("错误"), QString("数字和矩阵之间缺少运算符"));
                return false;
            }
        }
        else if(token == Add || token == Sub || token == Mul || token == Div || token == Inv)
        {
            if(prevToken == Id || prevToken == Num || prevToken == ')')
                prevToken = token;
            else if(prevToken == Inv && token == Inv)
                prevToken = token;
            else
            {
                QMessageBox::critical(NULL, QString("错误"), QString("多余操作符"));
                return false;
            }
        }
        else if(token == '(')
            st.push('(');
        else if(token == ')')
        {
            if(st.isEmpty())
            {
                QMessageBox::critical(NULL, QString("错误"), QString("缺少左括号"));
                return false;
            }
            else
                st.pop();
        }
        prevToken = token;
    }
    if(!st.isEmpty())
    {
        QMessageBox::critical(NULL, QString("错误"), QString("缺少右括号"));
        return false;
    }
    return true;
}
