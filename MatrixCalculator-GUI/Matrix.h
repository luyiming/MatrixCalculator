#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <QString>

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
    void resize(int r, int c);

    Matrix& operator = (const Matrix& rhs);
    friend const Matrix operator + (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator - (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator / (const Matrix& lhs, const Matrix& rhs);
    friend const Matrix operator * (const double lhs, const Matrix& rhs);
    friend const Matrix operator * (const Matrix& lhs, const double rhs);
    friend const Matrix operator * (const Matrix& lhs, const Matrix& rhs);
    const Matrix inverse() const;
    double det() const;
    const Matrix transpose() const;
    double* operator [] (int index);
    const double* operator [] (int index) const;
    void print();
};



#endif // MATRIX_H
