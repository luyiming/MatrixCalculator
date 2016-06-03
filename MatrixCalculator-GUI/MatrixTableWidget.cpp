#include "Matrix.h"
#include "Calculator.h"
#include "MatrixTableWidget.h"
#include "ui_MatrixTableWidget.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"

#include <QDebug>

#include <QIcon>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>


MatrixTableWidget::MatrixTableWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MatrixTableWidget)
{
    ui->setupUi(this);
    ui->addButton->setIcon(QIcon(":/src/add.png"));
    ui->deleteButton->setIcon(QIcon(":/src/delete.png"));
    ui->modifyButton->setIcon(QIcon(":/src/modify.png"));

    ui->matrixTable->setColumnCount(4);
    ui->matrixTable->setRowCount(0);
    QStringList headers;
    headers << QString::fromLocal8Bit(" 矩阵 ")
            << QString::fromLocal8Bit(" 行 ")
            << QString::fromLocal8Bit(" 列 ")
            << QString::fromLocal8Bit("         类型         ");
    ui->matrixTable->setHorizontalHeaderLabels(headers);
    ui->matrixTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // uneditable
    ui->matrixTable->setSelectionBehavior(QAbstractItemView::SelectRows);//select entire row
    ui->matrixTable->verticalHeader()->setVisible(false); //hide vertical header
    ui->matrixTable->setSelectionMode(QAbstractItemView::SingleSelection);//single selection
    ui->matrixTable->resizeColumnsToContents();
    ui->matrixTable->setColumnWidth(1, 42);
    ui->matrixTable->setColumnWidth(2, 42);
    ui->matrixTable->setSortingEnabled(true);
}

MatrixTableWidget::~MatrixTableWidget()
{
    delete ui;
}

void MatrixTableWidget::on_addButton_clicked()
{
    AddMatrixDialog* addDialog = new AddMatrixDialog(this, this->matrices);
    connect(addDialog, &AddMatrixDialog::matrix_added, this, &MatrixTableWidget::slot_update_matrix);
    addDialog->show();
}

void MatrixTableWidget::slot_update_matrix(Matrix mat, QString name, QString info)
{
    if(matrices.contains(name))
    {
        matrices.insert(name, mat);
        for(int i = 0; i < ui->matrixTable->rowCount(); ++i)
        {
            if(ui->matrixTable->item(i, 0)->text() == name)
            {
                ui->matrixTable->removeRow(i);
                ui->matrixTable->insertRow(i);
                ui->matrixTable->setItem(i, 0 , new QTableWidgetItem(name));
                ui->matrixTable->setItem(i, 1 , new QTableWidgetItem(QString::number(mat.row)));
                ui->matrixTable->setItem(i, 2 , new QTableWidgetItem(QString::number(mat.column)));
                ui->matrixTable->setItem(i, 3 , new QTableWidgetItem(QString(info)));
                break;
            }
        }
    }
    else
    {
        matrices.insert(name, mat);
        int curRow = ui->matrixTable->rowCount();
        ui->matrixTable->insertRow(curRow);
        ui->matrixTable->setItem(curRow, 0 , new QTableWidgetItem(name));
        ui->matrixTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(mat.row)));
        ui->matrixTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(mat.column)));
        ui->matrixTable->setItem(curRow, 3 , new QTableWidgetItem(QString(info)));
    }
}

void MatrixTableWidget::on_deleteButton_clicked()
{
    int row = ui->matrixTable->currentRow();
    if(row == -1)
    {
        QMessageBox::warning(this, QString("失败"), QString("没有指定矩阵"));
        return;
    }
    QString name = ui->matrixTable->item(row, 0)->text();
    if(QMessageBox::No == QMessageBox::question(this, QString("删除确认"), QString("确认删除矩阵 ") + name + QString(" 吗?")))
        return;
    if(!matrices.contains(name))
    {
        qDebug() << "delete error, no matrix";
        return;
    }
    matrices.remove(name);
    ui->matrixTable->removeRow(row);
}

void MatrixTableWidget::on_modifyButton_clicked()
{
    int row = ui->matrixTable->currentRow();
    if(row == -1)
    {
        QMessageBox::warning(this, QString("失败"), QString("没有指定矩阵"));
        return;
    }
    QString name = ui->matrixTable->item(row, 0)->text();
    Matrix mat = matrices[name];
    QString info = ui->matrixTable->item(row, 3)->text();
    AddMatrixDialog* addDialog = new AddMatrixDialog(this, this->matrices);
    addDialog->setModifyMode(mat, name, info);
    connect(addDialog, &AddMatrixDialog::matrix_changed, this, &MatrixTableWidget::slot_change_matrix);
    addDialog->show();
}

void MatrixTableWidget::slot_change_matrix(QString prevName, Matrix mat, QString name, QString info)
{
    for(int i = 0; i < ui->matrixTable->rowCount(); ++i)
    {
        if(ui->matrixTable->item(i, 0)->text() == prevName)
        {
            ui->matrixTable->removeRow(i);
            ui->matrixTable->insertRow(i);
            ui->matrixTable->setItem(i, 0 , new QTableWidgetItem(name));
            ui->matrixTable->setItem(i, 1 , new QTableWidgetItem(QString::number(mat.row)));
            ui->matrixTable->setItem(i, 2 , new QTableWidgetItem(QString::number(mat.column)));
            ui->matrixTable->setItem(i, 3 , new QTableWidgetItem(QString(info)));
            break;
        }
    }
    matrices.remove(prevName);
    matrices.insert(name, mat);
}

void MatrixTableWidget::on_matrixTable_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    QString name = ui->matrixTable->item(row, 0)->text();
    if(!matrices.contains(name))
    {
        qDebug() << "connot find matrix";
        return;
    }
    Matrix mat = matrices[name];
    emit matrix_changed(mat);
}


void MatrixTableWidget::slot_calculate(QString expression)
{

    char *exp = new char[expression.size() + 1];
    strcpy(exp, expression.toStdString().data());
    qDebug() << exp;
    Calculator calc(matrices);
    if(calc.isValid(exp))
    {
        Matrix res = calc.calculate(exp);
        if(res.isEmpty())
        {
            qDebug() << "未定义矩阵";
            emit info_updated("未定义矩阵");
            return;
        }
        else
            emit output_updated(res);
    }
}

void MatrixTableWidget::on_matrixTable_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)
    QString name = ui->matrixTable->item(row, 0)->text();
    Matrix mat = matrices[name];
    QString info = ui->matrixTable->item(row, 3)->text();
    AddMatrixDialog* addDialog = new AddMatrixDialog(this, this->matrices);
    addDialog->setModifyMode(mat, name, info);
    connect(addDialog, &AddMatrixDialog::matrix_changed, this, &MatrixTableWidget::slot_change_matrix);
    addDialog->show();
}

void MatrixTableWidget::slot_export_matrix()
{
    QString path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择导出文件"), ".");
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    QFile file(path);
    file.open(QFile::WriteOnly);
    QTextStream fout(&file);
    for(int i = 0; i < ui->matrixTable->rowCount(); i++)
    {
        QString name = ui->matrixTable->item(i, 0)->text();
        Matrix &mat = matrices[name];
        fout << name << " " << mat.row << " " << mat.column << " " << ui->matrixTable->item(i, 3)->text() << endl;
        for(int r = 0; r < mat.row; ++r)
        {
            for(int c = 0; c < mat.column; ++c)
            {
                fout << mat[r][c] << " ";
            }
            fout << endl;
        }
    }
    QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导出文件成功"));
}

void MatrixTableWidget::slot_import_matrix()
{
    QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择导入文件"), ".");
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    QFile file(path);
    file.open(QFile::ReadOnly);
    QTextStream fin(&file);
    int row, column;
    QString matName, info;
    while(!fin.atEnd())
    {
        fin >> matName >> row >> column >> info;
        if(fin.atEnd())
            break;
qDebug() << matName << " " << row << " " << column << info;
        Matrix mat(row, column);
        for(int r = 0; r < row; r++)
        {
            for(int c = 0; c < column; c++)
            {
                fin >> mat[r][c];
            }
        }
        if(matrices.contains(matName))
            continue;

        matrices.insert(matName, mat);
        mat.print();

        int curRow = ui->matrixTable->rowCount();
        ui->matrixTable->insertRow(curRow);
        ui->matrixTable->setItem(curRow, 0 , new QTableWidgetItem(matName));
        ui->matrixTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(row)));
        ui->matrixTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(column)));
        ui->matrixTable->setItem(curRow, 3 , new QTableWidgetItem(info));
    }
    QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导入文件成功"));
}
