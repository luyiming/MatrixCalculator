#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include "Matrix.h"
#include <QMessageBox>
#include <QDebug>
#include <QStringList>

AddMatrixDialog::AddMatrixDialog(QWidget *parent, QMap<QString, Matrix> mats) :
    QDialog(parent),
    ui(new Ui::AddMatrixDialog)
{
    this->matrices = mats;
    ui->setupUi(this);
    connect(ui->matrixList,SIGNAL(currentRowChanged(int)), ui->stack, SLOT(setCurrentIndex(int)));
    ui->matrixTable_0->setRowCount(3);
    ui->matrixTable_0->setColumnCount(3);
    ui->matrixTable_3->setRowCount(3);
    ui->matrixTable_3->setColumnCount(3);
}

AddMatrixDialog::~AddMatrixDialog()
{
    delete ui;
}

void AddMatrixDialog::on_rowLine_textChanged(const QString &arg1)
{
    int row = arg1.toInt();
    if(row < 0)
        return;
    switch(ui->stack->currentIndex())
    {
        case 0 :
        {
            ui->matrixTable_0->setColumnCount(row);
        }
        case 3 :
        {
            ui->matrixTable_3->setColumnCount(row);
        }
    }
}

void AddMatrixDialog::on_columnLine_textChanged(const QString &arg1)
{
    int coloum = arg1.toInt();
    if(coloum < 0)
        return;
    switch(ui->stack->currentIndex())
    {
        case 0 :
        {
            ui->matrixTable_0->setRowCount(coloum);
        }
        case 3 :
        {
            ui->matrixTable_3->setRowCount(coloum);
        }
    }
}

void AddMatrixDialog::on_quitButton_clicked()
{
    this->close();
}

void AddMatrixDialog::on_addButton_clicked()
{
    QString name = ui->nameLine->text();

    if(name.isEmpty())
    {
        QMessageBox::warning(this, QString("失败"), QString("缺少矩阵名字"));
        return;
    }

    switch(ui->stack->currentIndex())
    {
        case 0 :
        {
            int row = ui->matrixTable_0->rowCount();
            int column = ui->matrixTable_0->columnCount();
            Matrix mat(row, column);
            for(int r = 0; r < row; ++r)
            {
                for(int c = 0; c < column; ++c)
                {
                    if(ui->matrixTable_0->item(r, c) == NULL || ui->matrixTable_0->item(r, c)->text().isEmpty())
                    {
                        QMessageBox::warning(this, QString("失败"), QString("矩阵不完整"));
                        return;
                    }
                    mat[r][c] = ui->matrixTable_0->item(r, c)->text().toInt();
                }
            }
//            if(matrices.contains(name))
//            {
//                if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
//                    return;
//                else
//                    emit matrix_updated(mat, name, "自定义矩阵", true);
//            }
//            else
                emit matrix_updated(mat, name, "自定义矩阵", false);
            break;
        }
        case 1 :
        {
            int order = ui->matrixOrder_1->value();
            Matrix mat(order, order);
            for(int i = 0; i < order; ++i)
                mat[i][i] = 1;
            if(matrices.contains(name))
            {
                if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
                    return;
                else
                    emit matrix_updated(mat, name, "单位矩阵", true);
            }
            else
                emit matrix_updated(mat, name, "单位矩阵", false);
            break;
        }
        case 3 :
        {

        }
    }

    this->close();
}

void AddMatrixDialog::on_matrixList_currentRowChanged(int currentRow)
{
    if(currentRow == 1 || currentRow == 2)
    {
        ui->rowLine->setEnabled(false);
        ui->columnLine->setEnabled(false);
    }
    else
    {
        ui->rowLine->setEnabled(true);
        ui->columnLine->setEnabled(true);
    }
}
