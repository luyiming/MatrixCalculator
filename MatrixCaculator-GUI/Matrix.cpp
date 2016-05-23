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

Matrix& Matrix::operator = (const Matrix& rhs)
{
    if(this == &rhs)
        return *this;
    this->row = rhs.row;
    this->coloum = rhs.coloum;
    delete []mat;
    this->mat = new double[row * coloum];
    memcpy(this->mat, rhs.mat, sizeof(double) * row * coloum);
    return *this;
}

const Matrix& Matrix::operator + (const Matrix& rhs) const;
const Matrix& Matrix::operator - (const Matrix& rhs) const;
const Matrix& Matrix::operator * (const Matrix& rhs) const;
const Matrix& Matrix::operator / (const Matrix& rhs) const;

double* Matrix::operator [] (int index)
{
    if(this->mat == NULL)
    {
        qDebug() << "matrix has no value, index out of range";
        return NULL;
    }
    return &mat[index * coloum];
}

void Matrix::zero();

void Matrix::print()
{
    for(int r = 0; r < this->row; ++r)
    {
        for(int c = 0; c < this->coloum; ++c)
        {
            qDebug() << this->mat[r * coloum + c] << "\t";
        }
        qDebug() << endl;
    }
}
