#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"

AddMatrixDialog::AddMatrixDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMatrixDialog)
{
    ui->setupUi(this);
}

AddMatrixDialog::~AddMatrixDialog()
{
    delete ui;
}
