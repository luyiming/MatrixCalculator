#ifndef ADDMATRIXDIALOG_H
#define ADDMATRIXDIALOG_H

#include <QDialog>

namespace Ui {
class AddMatrixDialog;
}

class AddMatrixDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMatrixDialog(QWidget *parent = 0);
    ~AddMatrixDialog();

private:
    Ui::AddMatrixDialog *ui;
};

#endif // ADDMATRIXDIALOG_H
