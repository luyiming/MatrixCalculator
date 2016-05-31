#include "Matrix.h"
#include <cstring>
#include <QDebug>

Matrix::Matrix()
{
    this->mat = NULL;
}

Matrix::Matrix(int r, int c)
{
    if(r > 0 && c > 0)
    {
        this->row = r;
        this->coloum = c;
        this->mat = new double[r * c];
        memset(mat, 0, sizeof(double) * r * c);
    }
}

Matrix::~Matrix()
{
    if(this->mat != NULL)
        delete []mat;
}

Matrix::Matrix(const Matrix& rhs)
{
    this->row = rhs.row;
    this->coloum = rhs.coloum;
    this->mat = new double[row * coloum];
    memcpy(this->mat, rhs.mat, sizeof(double) * row * coloum);
}


void Matrix::resize(int r, int c)
{
    if(r <= 0 || c <= 0)
        return;
    if(this->mat != NULL)
        delete []mat;
    this->mat = new double[r * c];
    memset(mat, 0, sizeof(double) * r * c);
    this->row = r;
    this->coloum = c;
}

Matrix& Matrix::operator = (const Matrix& rhs)
{
    if(this == &rhs)
        return *this;
    this->row = rhs.row;
    this->coloum = rhs.coloum;
    if(this->mat != NULL)
        delete []mat;
    this->mat = new double[row * coloum];
    memcpy(this->mat, rhs.mat, sizeof(double) * row * coloum);
    return *this;
}

const Matrix operator + (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, lhs.coloum);
    if(lhs.row != rhs.row || lhs.coloum != rhs.coloum)
    {
        qDebug() << "matrix add not match";
        return res;
    }
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.coloum; ++c)
        {
            res[r][c] = lhs[r][c] + rhs[r][c];
        }
    }
    return res;
}

const Matrix operator - (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, lhs.coloum);
    if(lhs.row != rhs.row || lhs.coloum != rhs.coloum)
    {
        qDebug() << "matrix sub not match";
        return res;
    }
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.coloum; ++c)
        {
            res[r][c] = lhs[r][c] - rhs[r][c];
        }
    }
    return res;
}

const Matrix operator / (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res = lhs * rhs.inverse();
    return res;
}

const Matrix operator * (const double lhs, const Matrix& rhs)
{
    Matrix res(rhs.row, rhs.coloum);
    for(int r = 0; r < rhs.row; ++r)
    {
        for(int c = 0; c < rhs.coloum; ++c)
        {
            res[r][c] = lhs * rhs[r][c];
        }
    }
    return res;
}

const Matrix operator * (const Matrix& lhs, const double rhs)
{
    Matrix res(lhs.row, lhs.coloum);
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.coloum; ++c)
        {
            res[r][c] = lhs[r][c] * rhs;
        }
    }
    return res;
}

const Matrix operator * (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, rhs.coloum);
    if(lhs.coloum != rhs.row)
    {
        qDebug() << "matrix multiply not match";
        return res;
    }
    for(int r = 0; r < res.row; ++r)
    {
        for(int c = 0; c < res.coloum; ++c)
        {
            res[r][c] = 0.0;
            for(int k = 0; k < lhs.coloum; ++k)
                res[r][c] += (lhs[r][k] * rhs[k][c]);
        }
    }
    return res;
}

double* Matrix::operator [] (int index)
{
    if(this->mat == NULL)
    {
        qDebug() << "matrix has no value, index out of range";
        return NULL;
    }
    return &mat[index * coloum];
}

const double* Matrix::operator [] (int index) const
{
    if(this->mat == NULL)
    {
        qDebug() << "matrix has no value, index out of range";
        return NULL;
    }
    return &mat[index * coloum];
}

const Matrix Matrix::transpose() const
{
    Matrix res(row, coloum);
    for(int r = 0; r < row; ++r)
    {
        for(int c = 0; c < coloum; ++c)
        {
            res[c][r] = (*this)[r][c];
        }
    }
    return res;
}

//求行列式的值：
double Matrix::det() const
{
    if(row != coloum)
    {
        qDebug() << "matrix should be n*n";
        return 0.0;
    }
    int n = row;
    if(n == 2)
        return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
    if(n < 2)
        return 0.0;
    double res = 0.0;
    for(int i = 0; i < n; i++)
    {
        Matrix temp(n - 1, n - 1);
        for(int r = 1; r < n; ++r)
        {
            for(int c = 0; c < i; ++c)
                temp[r - 1][c] = (*this)[r][c];
            for(int c = i + 1; c < n; ++c)
                temp[r - 1][c - 1] = (*this)[r][c];
        }
        if(i % 2 == 0)
            res = res + (*this)[0][i] * temp.det();
        else
            res = res - (*this)[0][i] * temp.det();
    }
    return res;
}

//求逆矩阵： 伴随矩阵行列式的值除以行列式的值
const Matrix Matrix::inverse() const
{
    if(this->row != this->coloum)
    {
        qDebug() << "inverse should be n * n";
        return Matrix();
    }
    int n = this->row;
    Matrix res(n, n);
    double resultSum = this->det();
    if(resultSum == 0)
    {
        qDebug() << "no inverse";
        return Matrix();
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; ++j)
        {
            Matrix temp(n - 1, n - 1);
            for(int r = 0; r < i; ++r)
            {
                for(int c = 0; c < j; ++c)
                    temp[r][c] = (*this)[r][c];
                for(int c = j + 1; c < n; ++c)
                    temp[r][c - 1] = (*this)[r][c];
            }
            for(int r = i + 1; r < n; ++r)
            {
                for(int c = 0; c < j; ++c)
                    temp[r - 1][c] = (*this)[r][c];
                for(int c = j + 1; c < n; ++c)
                    temp[r - 1][c - 1] = (*this)[r][c];
            }
            if((i + j) % 2 == 0)
                res[j][i] = temp.det() / resultSum;
            else
                res[j][i] = -(temp.det() / resultSum);
        }
    }
    return res;
}

void Matrix::print()
{
    if(this->mat == NULL)
    {
        qDebug() << "empty matrix";
        return;
    }
    qDebug() << "matrix row :" << this->row;
    qDebug() << "matrix coloum :" << this->coloum;
    for(int r = 0; r < this->row; ++r)
    {
        QString str;
        for(int c = 0; c < this->coloum; ++c)
        {
            str += QString::number(this->mat[r * coloum + c]);
            str += "  ";
        }
        qDebug() << str;
    }
}


