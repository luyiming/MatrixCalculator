#include "Matrix.h"
#include <cstring>
#include <QDebug>

Matrix::Matrix()
{
    this->mat = NULL;
}

Matrix::Matrix(int r, int c)
{
    this->row = r;
    this->coloum = c;
    this->mat = new double[r * c];
    memset(mat, 0, sizeof(double) * r * c);
}

Matrix::~Matrix()
{
    if(this->mat != NULL)
    {
        delete []mat;
        mat = NULL;
    }
}

Matrix::Matrix(const Matrix& rhs)
{
    this->row = rhs.row;
    this->coloum = rhs.coloum;
    this->mat = new double[row * coloum];
    memcpy(this->mat, rhs.mat, sizeof(double) * row * coloum);
}

Matrix& Matrix::operator = (const Matrix& rhs)
{
    if(this == &rhs)
        return *this;
    this->row = rhs.row;
    this->coloum = rhs.coloum;
    if(this->mat != NULL)
    {
        delete []mat;
        mat = NULL;
    }
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

}

const Matrix operator * (const int lhs, const Matrix& rhs)
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

const Matrix operator * (const Matrix& lhs, const int rhs)
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

const Matrix Matrix::transpose()
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

void Matrix::print()
{
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
