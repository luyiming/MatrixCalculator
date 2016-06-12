#ifndef MATRIX_H
#define MATRIX_H

#include <cstdlib>
#include <QString>

class Matrix
{
private:
    double *mat = NULL;
    void multiple(Matrix A,double *V,int dim_x,int dim_y);
    double max(double *V,int dim);
    void div_matrix(double *V,int dim,double m);

public:
    int row = 0, column = 0;

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
    bool isEmpty() const;
    double det() const;
    const Matrix transpose() const;
    double* operator [] (int index);
    const double* operator [] (int index) const;
    void print();
    double test();
    friend bool isValid2(const Matrix& lhs, const Matrix& rhs, int token);


    int rank();
    void exchang_row(double *a,double *b,int n);
    void mul_row(double *a,double k,int n);
    void add_row(double *a1,double *a2,double k,int n);
};





#endif // MATRIX_H
