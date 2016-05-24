#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include <QIcon>
#include <QStringList>
#include <QTableWidget>
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <cstring>

class Matrix;
class Calculator;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->addButton->setIcon(QIcon(":/src/add.png"));
    ui->deleteButton->setIcon(QIcon(":/src/delete.png"));
    ui->modifyButton->setIcon(QIcon(":/src/modify.png"));
    ui->importButton->setIcon(QIcon(":/src/in.png"));

    ui->matrixtTable->setColumnCount(4);
    ui->matrixtTable->setRowCount(0);
    QStringList headers;
    headers << QString::fromLocal8Bit("矩阵")
            << QString::fromLocal8Bit("行")
            << QString::fromLocal8Bit("列")
            << QString::fromLocal8Bit("类型");
    ui->matrixtTable->setHorizontalHeaderLabels(headers);
    ui->matrixtTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // uneditable
    ui->matrixtTable->setSelectionBehavior(QAbstractItemView::SelectRows);//select entire row
    ui->matrixtTable->verticalHeader()->setVisible(false); //hide vertical header
    ui->matrixtTable->setSelectionMode(QAbstractItemView::SingleSelection);//single selection
    ui->matrixtTable->resizeColumnsToContents();
    ui->matrixtTable->setColumnWidth(1, 42);
    ui->matrixtTable->setColumnWidth(2, 42);
    ui->matrixtTable->setSortingEnabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::on_addButton_2_clicked()
{
    AddMatrixDialog *dlg = new AddMatrixDialog(this);
    dlg->show();
}

void MainWindow::on_addButton_clicked()
{
    int row, coloum;
    QString matName = ui->matrixNameLine->text().trimmed();
    if(matrices.contains(matName))
    {
        qDebug() << "already have matrix" << matName;
        return;
    }

    QStringList list = ui->matrixText->toPlainText().trimmed().split('\n');
    row = list.size();
    if(row == 0)
        return;
    coloum = list.at(0).trimmed().split(QRegExp("[ ,|]")).size();
    Matrix mat(row, coloum);
    for(int r = 0; r < row; r++)
    {
        QStringList list2 = list.at(r).trimmed().split(QRegExp("[ ,|]"));
        if(coloum != list2.size())
        {
            qDebug() << "coloum not equal";
            return;
        }
        for(int c = 0; c < coloum; c++)
            mat[r][c] = list2.at(c).toDouble();
    }
    matrices.insert(matName, mat);
    mat.print();

    int curRow = ui->matrixtTable->rowCount();
    ui->matrixtTable->insertRow(curRow);
    ui->matrixtTable->setItem(curRow, 0 , new QTableWidgetItem(matName));
    ui->matrixtTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(row)));
    ui->matrixtTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(coloum)));
}

void MainWindow::on_calcButton_clicked()
{
    char *exp = new char[ui->expLine->toPlainText().trimmed().size() + 1];
    strcpy(exp, ui->expLine->toPlainText().trimmed().toStdString().data());
    qDebug() << exp;
    Calculator calc(matrices);
    Matrix result = calc.calculate(exp);
    delete exp;
}
