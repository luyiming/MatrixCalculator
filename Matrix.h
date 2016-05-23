#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class Matrix()
{
private:
    vector<vector<int> > matrix;

public:
    Matrix();
    Matrix(Matrix& rhs);
    Matrix(const Matrix& rhs);
    Matrix& operator = (Matrix &rhs);
    const Matrix& operator = (const Matrix &rhs);
    Matrix& operator * (Matrix &rhs);
    Matrix& operator + (Matrix &rhs);
    Matrix& operator - (Matrix &rhs);
    Matrix& operator / (Matrix &rhs);
};

class Calculator()
{
private:
    char token[80];
    const char* expr = NULL;
    const char* next();

    public:
    eval(const char* exp);

};
#endif

--         --    --            --
|12 21 3  3 |    |              |
|31 2  55 45|    |              |
|           |  x |              |
|21 12 11 33|    |              |
|15 64 8  97|    |              |
--         --    --            --
