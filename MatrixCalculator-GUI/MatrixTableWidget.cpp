#include "Matrix.h"
#include "MatrixTableWidget.h"
#include "ui_MatrixTableWidget.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"

#include <QDebug>

#include <QIcon>
#include <QStringList>



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
    connect(addDialog, &AddMatrixDialog::matrix_updated, this, &MatrixTableWidget::slot_update_matrix);
    addDialog->show();
}

void MatrixTableWidget::slot_update_matrix(Matrix mat, QString name, QString info, bool isOverwritten)
{
    matrices[name] = mat;
     qDebug() << 1;
    if(isOverwritten)
    {
         qDebug() << 2;
        for(int i = 0; i < ui->matrixTable->rowCount(); ++i)
        {
            if(ui->matrixTable->item(i, 0)->text() == name)
            {
                ui->matrixTable->removeRow(i);
                ui->matrixTable->insertRow(i);
                ui->matrixTable->setItem(i, 0 , new QTableWidgetItem(name));
                ui->matrixTable->setItem(i, 1 , new QTableWidgetItem(QString::number(mat.row)));
                ui->matrixTable->setItem(i, 2 , new QTableWidgetItem(QString::number(mat.coloum)));
                ui->matrixTable->setItem(i, 3 , new QTableWidgetItem(QString(info)));
                break;
            }
        }
    }
    else
    {
        qDebug() << 123;
        int curRow = ui->matrixTable->rowCount();
        ui->matrixTable->insertRow(curRow);
        ui->matrixTable->setItem(curRow, 0 , new QTableWidgetItem(name));
        ui->matrixTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(mat.row)));
        ui->matrixTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(mat.coloum)));
        ui->matrixTable->setItem(curRow, 3 , new QTableWidgetItem(QString(info)));
    }
}
