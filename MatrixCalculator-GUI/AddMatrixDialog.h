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
    void setModifyMode(Matrix mat, QString name, QString info);
    ~AddMatrixDialog();

private slots:
    void on_rowLine_textChanged(const QString &arg1);

    void on_columnLine_textChanged(const QString &arg1);

    void on_quitButton_clicked();

    void on_addButton_clicked();

    void on_matrixList_currentRowChanged(int currentRow);

    void on_matrixOrder_3_currentIndexChanged(const QString &arg1);

    void on_angleLine_textChanged(const QString &arg1);

    void on_radianLine_textChanged(const QString &arg1);

    void on_angleLine_textEdited(const QString &arg1);

    void on_radianLine_textEdited(const QString &arg1);

signals:
    void matrix_added(Matrix mat, QString name, QString info);
    void matrix_changed(QString prevName, Matrix mat, QString name, QString info);

private:
    Ui::AddMatrixDialog *ui;
    QMap<QString, Matrix> matrices;
    bool modifyMode = false;
    QString prevName;
};

#endif // ADDMATRIXDIALOG_H
