#include "DisplayWidget.h"
#include "ui_DisplayWidget.h"
#include <QMessageBox>
#include <QDebug>

DisplayWidget::DisplayWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DisplayWidget)
{
    ui->setupUi(this);
    ui->precisionBox->setRange(0, 20);
    ui->precisionBox->setValue(3);
}

DisplayWidget::~DisplayWidget()
{
    delete ui;
}

void DisplayWidget::slot_display_matrix(Matrix mat)
{
    int row = mat.row;
    int column = mat.column;
    ui->rowLine->setText(QString::number(row));
    ui->columnLine->setText(QString::number(column));
    ui->displayTable->clear();
    ui->displayTable->setRowCount(row);
    ui->displayTable->setColumnCount(column);
    for(int r = 0; r < row; ++r)
        for(int c = 0; c < column; ++c)
            ui->displayTable->setItem(r, c , new QTableWidgetItem(QString::number(mat[r][c])));
    ui->displayTable->resizeColumnsToContents();
}

void DisplayWidget::slot_display_output(Matrix mat)
{
    int row = mat.row;
    int column = mat.column;
    ui->outputTable->clear();
    ui->outputTable->setRowCount(row);
    ui->outputTable->setColumnCount(column);
    for(int r = 0; r < row; ++r)
        for(int c = 0; c < column; ++c)
            ui->outputTable->setItem(r, c , new QTableWidgetItem(QString("%1").arg(mat[r][c], 0, 'g', ui->precisionBox->value())));
    ui->outputTable->resizeColumnsToContents();
}

void DisplayWidget::on_transposeButton_clicked()
{
    int row = ui->displayTable->rowCount();
    int column = ui->displayTable->columnCount();
    if(row == 0 || column == 0)
        return;

    Matrix mat(row, column);
    for(int r = 0; r < row; ++r)
    {
        for(int c = 0; c < column; ++c)
        {
            if(ui->displayTable->item(r, c) == NULL || ui->displayTable->item(r, c)->text().isEmpty())
            {
                QMessageBox::warning(this, QString("失败"), QString("矩阵不完整"));
                return;
            }
            mat[r][c] = ui->displayTable->item(r, c)->text().toDouble();
        }
    }
    Matrix res = mat.transpose();
    slot_display_output(res);
    return;
}

void DisplayWidget::on_detButton_clicked()
{
    int row = ui->displayTable->rowCount();
    int column = ui->displayTable->columnCount();
    if(row == 0 || column == 0)
        return;

    Matrix mat(row, column);
    for(int r = 0; r < row; ++r)
    {
        for(int c = 0; c < column; ++c)
        {
            if(ui->displayTable->item(r, c) == NULL || ui->displayTable->item(r, c)->text().isEmpty())
            {
                QMessageBox::warning(this, QString("失败"), QString("矩阵不完整"));
                return;
            }
            mat[r][c] = ui->displayTable->item(r, c)->text().toDouble();
        }
    }
    Matrix res(1, 1);
    res[0][0] = mat.det();
    slot_display_output(res);
    return;
}

void DisplayWidget::on_inverseButton_clicked()
{
    int row = ui->displayTable->rowCount();
    int column = ui->displayTable->columnCount();
    if(row == 0 || column == 0)
        return;

    Matrix mat(row, column);
    for(int r = 0; r < row; ++r)
    {
        for(int c = 0; c < column; ++c)
        {
            if(ui->displayTable->item(r, c) == NULL || ui->displayTable->item(r, c)->text().isEmpty())
            {
                QMessageBox::warning(this, QString("失败"), QString("矩阵不完整"));
                return;
            }
            mat[r][c] = ui->displayTable->item(r, c)->text().toDouble();
        }
    }

    if(mat.det() == 0)
    {
        QMessageBox::critical(this, QString("错误"), QString("不存在逆矩阵"));
        return;
    }
    Matrix res = mat.inverse();

    slot_display_output(res);
    return;
}

void DisplayWidget::on_applyButton_clicked() //slot_change_matrix
{
    QString name = ui->matrixNameLine->text();
    int row = ui->outputTable->rowCount();
    int column = ui->outputTable->columnCount();
    Matrix mat(row, column);
    for(int r = 0; r < row; ++r)
        for(int c = 0; c < column; ++c)
            mat[r][c] = ui->outputTable->item(r, c)->text().toDouble();

    emit apply_matrix(name, mat, name, "自定义矩阵");
}
