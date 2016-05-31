#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Matrix.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include "InputFrame.h"
#include "ui_InputFrame.h"
#include "MatrixTableWidget.h"
#include "ui_MatrixTableWidget.h"
#include "DisplayWidget.h"
#include "ui_DisplayWidget.h"

#include <QMap>
#include <QSplitter>
#include <QEvent>
#include <QMouseEvent>
#include <QShowEvent>


#include "Calculator.h"
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

    void on_inverseButton_clicked();

    void on_detButton_clicked();

    void on_deleteButton_clicked();

    void on_transposeButton_clicked();

    void on_exportButton_clicked();

    void on_importButton_clicked();

    void slot_slideButtonClicked();
    void slot_splitterMoved(int pos, int index);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;

    int getMatrix(Matrix &result);

    void setBtnIcon();
    void setBtnPos();

    MatrixTableWidget* matrixTableWidget;
    QMap<QString, Matrix> matrices;
    DisplayWidget* displayWidget;
    InputFrame* inputFrame;
    QSplitter* mainSplitter;
    QSplitter* rightSplitter;
    QPushButton* slideButton;
};

#endif // MAINWINDOW_H
