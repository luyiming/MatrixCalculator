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

private slots:


    void slot_slideButtonClicked();
    void slot_splitterMoved(int pos, int index);

protected:
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;

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
