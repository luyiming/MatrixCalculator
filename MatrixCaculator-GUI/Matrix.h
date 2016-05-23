#ifndef MATRIX_H
#define MATRIX_H


class Matrix
{
private:
    double *mat;

public:
    int row = -1, coloum = -1;

    Matrix();
    Matrix(int r, int c);
    ~Matrix();
    Matrix(const Matrix& rhs);
    Matrix& operator = (const Matrix& rhs);
    const Matrix& operator + (const Matrix& rhs) const;
    const Matrix& operator - (const Matrix& rhs) const;
    const Matrix& operator * (const Matrix& rhs) const;
    const Matrix& operator / (const Matrix& rhs) const;
    double* operator [] (int index);
    const double* operator [] (int index) const;
    void print();
    void zero();
};

#endif // MATRIX_H
