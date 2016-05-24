#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>

class Matrix
{
private:
    double *mat = NULL;

public:
    int row = -1, coloum = -1;

    Matrix();
    Matrix(int r, int c);
    ~Matrix();
    Matrix(const Matrix& rhs);
    Matrix& operator = (const Matrix& rhs);
    friend const Matrix operator + (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator - (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator / (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator * (const int lhs, const Matrix& rhs);
    friend const Matrix operator * (const Matrix& lhs, const int rhs);
    friend const Matrix operator * (const Matrix& lhs, const Matrix& rhs);

    double* operator [] (int index);
    const double* operator [] (int index) const;
    void print();
    void zero();
};

#endif // MATRIX_H
