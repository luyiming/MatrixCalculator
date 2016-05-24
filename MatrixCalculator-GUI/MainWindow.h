#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Matrix.h"
#include "Calculator.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    friend class Calculator;

private slots:
    void on_quitButton_clicked();

    void on_addButton_2_clicked();

    void on_addButton_clicked();

    void on_calcButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QString, Matrix> matrices;
};

#endif // MAINWINDOW_H
