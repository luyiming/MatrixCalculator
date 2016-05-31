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

public slots:
    void slot_update_matrix(Matrix mat, QString name, QString info, bool isOverwritten);

private:
    Ui::MatrixTableWidget *ui;
    QMap<QString, Matrix> matrices;

};

#endif // MATRIXTABLEWIDGET_H
