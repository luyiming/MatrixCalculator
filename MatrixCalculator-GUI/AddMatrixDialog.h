#ifndef ADDMATRIXDIALOG_H
#define ADDMATRIXDIALOG_H

#include <QDialog>
#include "Matrix.h"
#include <QStringList>
#include <QMap>

namespace Ui {
class AddMatrixDialog;
}

class AddMatrixDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMatrixDialog(QWidget *parent = 0, QMap<QString, Matrix> mats = QMap<QString, Matrix>());
    ~AddMatrixDialog();

private slots:
    void on_rowLine_textChanged(const QString &arg1);

    void on_columnLine_textChanged(const QString &arg1);

    void on_quitButton_clicked();

    void on_addButton_clicked();

    void on_matrixList_currentRowChanged(int currentRow);

signals:
    void matrix_updated(Matrix mat, QString name, QString info, bool isOverwritten);

private:
    Ui::AddMatrixDialog *ui;
    QMap<QString, Matrix> matrices;
    QStringList nameList;
};

#endif // ADDMATRIXDIALOG_H
