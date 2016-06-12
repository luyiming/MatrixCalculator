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

    ui->matrixOrder_3->setCurrentIndex(1);

    ui->matrixOrder_1->setMinimum(1);
    ui->matrixOrder_2->setMinimum(1);
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
            ui->matrixTable_0->setRowCount(row);
        }
    }
}

void AddMatrixDialog::on_columnLine_textChanged(const QString &arg1)
{
    int column = arg1.toInt();
    if(column < 0)
        return;
    switch(ui->stack->currentIndex())
    {
        case 0 :
        {
            ui->matrixTable_0->setColumnCount(column);
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
                    mat[r][c] = ui->matrixTable_0->item(r, c)->text().toDouble();
                }
            }
            if(modifyMode)
            {
                emit matrix_changed(prevName, mat, name, "自定义矩阵");
            }
            else
            {
                if(matrices.contains(name))
                {
                    if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
                        return;
                }
                emit matrix_added(mat, name, "自定义矩阵");
            }
            break;
        }
        case 1 :
        {
            int order = ui->matrixOrder_1->value();
            Matrix mat(order, order);
            for(int i = 0; i < order; ++i)
                mat[i][i] = 1;
            if(modifyMode)
            {
                emit matrix_changed(prevName, mat, name, "单位矩阵");
            }
            else
            {
                if(matrices.contains(name))
                {
                    if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
                        return;
                }
                emit matrix_added(mat, name, "单位矩阵");
            }
            break;
        }
        case 2 :
        {
            int order = ui->matrixOrder_2->value();
            Matrix mat(order, order);
            for(int r = 0; r < order; ++r)
                for(int c = 0; c < order; ++c)
                    mat[r][c] = 1;
            if(modifyMode)
            {
                emit matrix_changed(prevName, mat, name, "1矩阵");
            }
            else
            {
                if(matrices.contains(name))
                {
                    if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
                        return;
                }
                emit matrix_added(mat, name, "1矩阵");
            }
            break;
        }
        case 3 :
        {
            int row = ui->matrixTable_3->rowCount();
            int column = ui->matrixTable_3->columnCount();
            Matrix mat(row, column);
            for(int r = 0; r < row; ++r)
            {
                for(int c = 0; c < column; ++c)
                {
                    if(ui->matrixTable_3->item(r, c) == NULL || ui->matrixTable_3->item(r, c)->text().isEmpty())
                    {
                        QMessageBox::warning(this, QString("失败"), QString("矩阵不完整"));
                        return;
                    }
                    mat[r][c] = ui->matrixTable_3->item(r, c)->text().toDouble();
                }
            }
            if(modifyMode)
            {
                emit matrix_changed(prevName, mat, name, "旋转矩阵");
            }
            else
            {
                if(matrices.contains(name))
                {
                    if(QMessageBox::No == QMessageBox::question(this, QString("失败"), QString("已有同名矩阵,是否覆盖?")))
                        return;
                }
                emit matrix_added(mat, name, "旋转矩阵");
            }
            break;
        }
    }

    this->close();
}

void AddMatrixDialog::on_matrixList_currentRowChanged(int currentRow)
{
    if(currentRow == 1 || currentRow == 2 || currentRow == 3)
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

void AddMatrixDialog::setModifyMode(Matrix mat, QString name, QString info)
{
    modifyMode = true;
    prevName = name;
    ui->nameLine->setText(name);
    ui->rowLine->setText(QString::number(mat.row));
    ui->columnLine->setText(QString::number(mat.column));
    if(info == "单位矩阵")
    {
        ui->matrixList->setCurrentRow(1);
        ui->matrixOrder_1->setValue(mat.row);
    }
    else if(info == "1矩阵")
    {
        ui->matrixList->setCurrentRow(2);
        ui->matrixOrder_2->setValue(mat.row);
    }
    else if(info == "旋转矩阵")
    {

    }
    else
    {
        ui->matrixList->setCurrentRow(0);
        ui->matrixTable_0->setRowCount(mat.row);
        ui->matrixTable_0->setColumnCount(mat.column);
        for(int r = 0; r < mat.row; ++r)
        {
            for(int c = 0; c < mat.column; ++c)
            {
                ui->matrixTable_0->setItem(r, c , new QTableWidgetItem(QString::number(mat[r][c])));
            }
        }
    }
}

void AddMatrixDialog::on_matrixOrder_3_currentIndexChanged(const QString &arg1)
{
    ui->matrixTable_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if(arg1 == "2")
    {
        ui->label_3->setVisible(false);
        ui->rotationBox->setVisible(false);
        ui->matrixTable_3->setRowCount(2);
        ui->matrixTable_3->setColumnCount(2);
    }
    else if(arg1 == "3")
    {
        ui->label_3->setVisible(true);
        ui->rotationBox->setVisible(true);
        ui->matrixTable_3->setRowCount(3);
        ui->matrixTable_3->setColumnCount(3);
    }
    else if(arg1 == "4")
    {
        ui->label_3->setVisible(true);
        ui->rotationBox->setVisible(true);
        ui->matrixTable_3->setRowCount(4);
        ui->matrixTable_3->setColumnCount(4);
    }

    //change matrix
    on_angleLine_textEdited(ui->angleLine->text());

}

void AddMatrixDialog::on_angleLine_textEdited(const QString &arg1)
{
    double angle = arg1.toDouble();
    double radian = angle * 3.1415926 / 180.0;
    ui->radianLine->setText(QString::number(angle * 3.1415926 / 180.0, 'g', 5));
    for(int r = 0; r < ui->matrixTable_3->rowCount(); ++r)
        for(int c = 0; c < ui->matrixTable_3->columnCount(); ++c)
            ui->matrixTable_3->setItem(r, c, new QTableWidgetItem(QString("0")));

    QString a = QString::number(cos(radian), 'g', 2);
    QString b = QString::number(-sin(radian), 'g', 2);
    QString c = QString::number(sin(radian), 'g', 2);

    if(ui->matrixOrder_3->currentText() == "2")
    {
        ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
        ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
        ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
        ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
    }
    else if(ui->matrixOrder_3->currentText() == "3" || ui->matrixOrder_3->currentText() == "4")
    {
        if(ui->matrixOrder_3->currentText() == "4")
            ui->matrixTable_3->setItem(3, 3, new QTableWidgetItem(QString("1")));
        if(ui->rotationBox->currentText() == "x")
        {
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(1, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 1, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(ui->rotationBox->currentText() == "y")
        {
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(ui->rotationBox->currentText() == "z")
        {
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
        }
    }
}

void AddMatrixDialog::on_radianLine_textEdited(const QString &arg1)
{
    double radian = arg1.toDouble();
    ui->angleLine->setText(QString::number(radian / 3.1415926 * 180.0, 'g', 4));

    for(int r = 0; r < ui->matrixTable_3->rowCount(); ++r)
        for(int c = 0; c < ui->matrixTable_3->columnCount(); ++c)
            ui->matrixTable_3->setItem(r, c, new QTableWidgetItem(QString("0")));

    QString a = QString::number(cos(radian), 'g', 2);
    QString b = QString::number(-sin(radian), 'g', 2);
    QString c = QString::number(sin(radian), 'g', 2);

    if(ui->matrixOrder_3->currentText() == "2")
    {
        ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
        ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
        ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
        ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
    }
    else if(ui->matrixOrder_3->currentText() == "3" || ui->matrixOrder_3->currentText() == "4")
    {
        if(ui->matrixOrder_3->currentText() == "4")
            ui->matrixTable_3->setItem(3, 3, new QTableWidgetItem(QString("1")));
        if(ui->rotationBox->currentText() == "x")
        {
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(1, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 1, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(ui->rotationBox->currentText() == "y")
        {
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(ui->rotationBox->currentText() == "z")
        {
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
        }
    }
}

void AddMatrixDialog::on_rotationBox_currentIndexChanged(const QString &arg1)
{
    //if ui->radianLine->text() is empty, display 0
    double radian = ui->radianLine->text().toDouble();
    for(int r = 0; r < ui->matrixTable_3->rowCount(); ++r)
        for(int c = 0; c < ui->matrixTable_3->columnCount(); ++c)
            ui->matrixTable_3->setItem(r, c, new QTableWidgetItem(QString("0")));

    QString a = QString::number(cos(radian), 'g', 2);
    QString b = QString::number(-sin(radian), 'g', 2);
    QString c = QString::number(sin(radian), 'g', 2);

    if(ui->matrixOrder_3->currentText() == "2")
    {
        ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
        ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
        ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
        ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
    }
    else if(ui->matrixOrder_3->currentText() == "3" || ui->matrixOrder_3->currentText() == "4")
    {
        if(ui->matrixOrder_3->currentText() == "4")
            ui->matrixTable_3->setItem(3, 3, new QTableWidgetItem(QString("1")));
        if(arg1 == "x")
        {
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(1, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 1, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(arg1 == "y")
        {
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 2, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(2, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(a));
        }
        else if(arg1 == "z")
        {
            ui->matrixTable_3->setItem(2, 2, new QTableWidgetItem(QString("1")));
            ui->matrixTable_3->setItem(0, 0, new QTableWidgetItem(a));
            ui->matrixTable_3->setItem(0, 1, new QTableWidgetItem(b));
            ui->matrixTable_3->setItem(1, 0, new QTableWidgetItem(c));
            ui->matrixTable_3->setItem(1, 1, new QTableWidgetItem(a));
        }
    }
}
