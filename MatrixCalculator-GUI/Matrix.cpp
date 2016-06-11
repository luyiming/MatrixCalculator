#include "Matrix.h"
#include <cstring>
#include <QDebug>
#include <QMessageBox>

Matrix::Matrix()
{
    this->mat = NULL;
}

Matrix::Matrix(int r, int c)
{
    if(r > 0 && c > 0)
    {
        this->row = r;
        this->column = c;
        this->mat = new double[r * c];
        memset(mat, 0, sizeof(double) * r * c);
    }
    else
    {
        this->row = 0;
        this->column = 0;
        this->mat = NULL;
    }
}

Matrix::~Matrix()
{
    if(this->mat != NULL)
        delete []mat;
}

Matrix::Matrix(const Matrix& rhs)
{
    if(rhs.row > 0 && rhs.column > 0)
    {
        this->row = rhs.row;
        this->column = rhs.column;
        this->mat = new double[row * column];
        memcpy(this->mat, rhs.mat, sizeof(double) * row * column);
    }
    else
    {
        this->row = 0;
        this->column = 0;
        this->mat = NULL;
    }
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
    this->column = c;
}

Matrix& Matrix::operator = (const Matrix& rhs)
{
    if(this == &rhs)
        return *this;
    this->row = rhs.row;
    this->column = rhs.column;
    if(this->mat != NULL)
        delete []mat;
    this->mat = new double[row * column];
    memcpy(this->mat, rhs.mat, sizeof(double) * row * column);
    return *this;
}

const Matrix operator + (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, lhs.column);
    if(lhs.row != rhs.row || lhs.column != rhs.column)
    {
        qDebug() << "matrix add not match";
        return res;
    }
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.column; ++c)
        {
            res[r][c] = lhs[r][c] + rhs[r][c];
        }
    }
    return res;
}

const Matrix operator - (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, lhs.column);
    if(lhs.row != rhs.row || lhs.column != rhs.column)
    {
        qDebug() << "matrix sub not match";
        return res;
    }
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.column; ++c)
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
    Matrix res(rhs.row, rhs.column);
    for(int r = 0; r < rhs.row; ++r)
    {
        for(int c = 0; c < rhs.column; ++c)
        {
            res[r][c] = lhs * rhs[r][c];
        }
    }
    return res;
}

const Matrix operator * (const Matrix& lhs, const double rhs)
{
    Matrix res(lhs.row, lhs.column);
    for(int r = 0; r < lhs.row; ++r)
    {
        for(int c = 0; c < lhs.column; ++c)
        {
            res[r][c] = lhs[r][c] * rhs;
        }
    }
    return res;
}

const Matrix operator * (const Matrix& lhs, const Matrix& rhs)
{
    Matrix res(lhs.row, rhs.column);
    if(lhs.column != rhs.row)
    {
        qDebug() << "matrix multiply not match";
        return res;
    }
    for(int r = 0; r < res.row; ++r)
    {
        for(int c = 0; c < res.column; ++c)
        {
            res[r][c] = 0.0;
            for(int k = 0; k < lhs.column; ++k)
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
    return &mat[index * column];
}

const double* Matrix::operator [] (int index) const
{
    if(this->mat == NULL)
    {
        qDebug() << "matrix has no value, index out of range";
        return NULL;
    }
    return &mat[index * column];
}

const Matrix Matrix::transpose() const
{
    Matrix res(column, row);
    for(int r = 0; r < row; ++r)
    {
        for(int c = 0; c < column; ++c)
        {
            res[c][r] = (*this)[r][c];
        }
    }
    return res;
}

//求行列式的值：
double Matrix::det() const
{
    if(row != column)
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
    if(this->row != this->column)
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
    qDebug() << "matrix column :" << this->column;
    for(int r = 0; r < this->row; ++r)
    {
        QString str;
        for(int c = 0; c < this->column; ++c)
        {
            str += QString::number(this->mat[r * column + c]);
            str += "  ";
        }
        qDebug() << str;
    }
}


bool Matrix::isEmpty() const
{
    if(mat == NULL)
        return true;
    else
        return false;
}

bool isValid2(const Matrix& lhs, const Matrix& rhs, int token)
{
    switch(token)
    {
        case '+': case '-':
        {
            if(lhs.row == rhs.row && lhs.column == rhs.column)
                return true;
            else
                return false;
        }
        case '*':
        {
            if(lhs.column == rhs.row)
                return true;
            else
                return false;
        }
        case '/':
        {
            if(rhs.det() != 0 && lhs.column == rhs.row)
                return true;
            else
                return false;
        }
    }
    return false;
}


//void multiple(double**A,double *V,int dim_x,int dim_y);
//double max(double *V,int dim);
//void div_matrix(double *V,int dim,double m);

//int main(){
//    FILE *file = fopen("dengjin.txt","r");
//    int dim_x,dim_y;
//    double **A,*V;
//    double miu0 = 0,miu1 = 10;/*just make sure to enter the loop*/
//    fscanf(file,"%d %d",&dim_x,&dim_y);

///*load in data*/
//    A = (double **)malloc(sizeof(double *)*dim_x);
//    V = (double *)malloc(sizeof(double)*dim_y);
//    for(int i=0;i<dim_x;i++)
//        A[i] = (double *)malloc(sizeof(double)*dim_y);

//    for(int i=0;i<dim_x;i++)
//        for(int j=0;j<dim_y;j++)
//            fscanf(file,"%lf",&A[i][j]);

//    for(int i=0;i<dim_y;i++)
//        V[i] = 1;/*initialing a vector with any value*/

//    while(fabs(miu1-miu0) >= 1E-8){
//        multiple(A,V,dim_x,dim_y);
//        miu0 = miu1;
//        miu1 = max(V,dim_y);
//        div_matrix(V,dim_y,miu1);
//        for(int i=0;i<dim_y;i++)
//            printf("%10.8lf ",V[i]);
//        puts("");
//    }
//    printf("Eigenvalue: %10.8lf/n", miu1);
//    //delocating

//}

//void multiple(Matrix A, double *V, int dim_x, int dim_y){
//    double *tmp = (double *)malloc(sizeof(double)*dim_y);
//    for(int i=0;i<dim_y;i++)
//        tmp[i] = 0;

//    for(int i=0;i<dim_x;i++)
//        for(int j=0;j<dim_y;j++)
//            tmp[i] += A[i][j]*V[j];

//    for(int i=0;i<dim_y;i++)
//        V[i] = tmp[i];
//    free(tmp);
//}

//double max(double *V,int dim){
//    double tmp = V[0];
//    for(int i=1;i<dim;i++)
//            if(fabs(V[i]) > fabs(tmp))
//                tmp = V[i];
//    return tmp;
//}

//void div_matrix(double *V,int dim,double m){
//    for(int i=0;i<dim;i++)
//        V[i] /= m;
//}
