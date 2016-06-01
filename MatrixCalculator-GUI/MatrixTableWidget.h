#ifndef MATRIXTABLEWIDGET_H
#define MATRIXTABLEWIDGET_H

#include <QFrame>
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include "Matrix.h"

#include <QString>

namespace Ui {
class MatrixTableWidget;
}

class MatrixTableWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MatrixTableWidget(QWidget *parent = 0);
    ~MatrixTableWidget();

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_modifyButton_clicked();

    void on_matrixTable_cellClicked(int row, int column);

    void on_matrixTable_cellDoubleClicked(int row, int column);

public slots:
    void slot_update_matrix(Matrix mat, QString name, QString info);
    void slot_change_matrix(QString prevName, Matrix mat, QString name, QString info);
    void slot_calculate(QString expression);

signals:
    void matrix_changed(Matrix mat);
    void output_updated(Matrix mat);
    void info_updated(QString info);

private:
    Ui::MatrixTableWidget *ui;
    QMap<QString, Matrix> matrices;

};

#endif // MATRIXTABLEWIDGET_H
