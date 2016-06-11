#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddMatrixDialog.h"
#include "ui_AddMatrixDialog.h"
#include "MatrixTableWidget.h"
#include "ui_MatrixTableWidget.h"
#include "DisplayWidget.h"
#include "ui_DisplayWidget.h"
#include "InputFrame.h"
#include "ui_InputFrame.h"
#include <QIcon>
#include <QStringList>
#include <QTableWidget>
#include <QString>
#include <QDebug>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <cstring>
#include <QPixmap>
#include <QBitmap>

class Matrix;
class Calculator;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    matrixTableWidget = new MatrixTableWidget(this);
    displayWidget = new DisplayWidget(this);
    inputFrame = new InputFrame(this);

    connect(matrixTableWidget, &MatrixTableWidget::matrix_changed, displayWidget, &DisplayWidget::slot_display_matrix);
    connect(inputFrame, &InputFrame::signal_calculate, matrixTableWidget, &MatrixTableWidget::slot_calculate);
    connect(matrixTableWidget, &MatrixTableWidget::output_updated, displayWidget, &DisplayWidget::slot_display_output);
    connect(displayWidget, &DisplayWidget::apply_matrix, matrixTableWidget, &MatrixTableWidget::slot_change_matrix);
    connect(inputFrame, &InputFrame::signal_export_matrix, matrixTableWidget, &MatrixTableWidget::slot_export_matrix);
    connect(inputFrame, &InputFrame::signal_import_matrix, matrixTableWidget, &MatrixTableWidget::slot_import_matrix);
    connect(inputFrame, &InputFrame::signal_close, this, &MainWindow::close);
    connect(matrixTableWidget, &MatrixTableWidget::signal_setError, inputFrame, &InputFrame::slot_setError);

    mainSplitter = new QSplitter(Qt::Horizontal, this);
    rightSplitter = new QSplitter(Qt::Vertical, this);

    slideButton= new QPushButton(this);
    slideButton->setFocusPolicy(Qt::NoFocus);

    rightSplitter->addWidget(displayWidget);
    rightSplitter->addWidget(inputFrame);
    mainSplitter->addWidget(matrixTableWidget);
    mainSplitter->addWidget(rightSplitter);
    //mainSplitter->setStyleSheet("QmainSplitter::handle { background-color: black }"); //设置分界线的样式

    connect(mainSplitter,SIGNAL(splitterMoved(int,int)),this,SLOT(slot_splitterMoved(int,int)));
    connect(slideButton,SIGNAL(clicked()),this,SLOT(slot_slideButtonClicked()));
    setCentralWidget(mainSplitter);
    //setWindowFlags(Qt::FramelessWindowHint);

    matrixTableWidget->setMouseTracking(true);
    displayWidget->setMouseTracking(true);
    mainSplitter->setMouseTracking(true);

    slideButton->setMouseTracking(true);
    setMouseTracking(true);


}

void MainWindow::setBtnPos()
{
    int width = matrixTableWidget->width() + 7;
    //int height = (matrixTableWidget->height() - slideButton->height()) / 2;
    int height = matrixTableWidget->height() - slideButton->height();
    slideButton->move(width ,height);
}

void MainWindow::setBtnIcon()
{

    if(matrixTableWidget->width()!=0)
    {
        slideButton->setIcon(QIcon(":/arrow-left"));
    }
    else
    {
        slideButton->setIcon(QIcon(":/arrow-right"));
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    static bool m_bInitShow = false;
    if(!m_bInitShow)
    {
        setBtnPos();
        setBtnIcon();
        m_bInitShow = true;

        QPixmap pixmap(":/arrow-left");
        pixmap = pixmap.scaled(24, 24);
        slideButton->resize(pixmap.size());
        //slideButton->setFixedSize(pixmap.size());
        slideButton->setMask(pixmap.mask());
        slideButton->setIconSize(pixmap.size());
        slideButton->setStyleSheet("border:none;");
    }
    QMainWindow::showEvent(event);

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    setBtnPos();
    QMainWindow::resizeEvent(event);
}

void MainWindow::slot_splitterMoved(int pos, int index)
{
    Q_UNUSED(pos)
    Q_UNUSED(index)
    setBtnPos();
    if(matrixTableWidget->width()==0)
    {
        setBtnIcon();
    }
 }

void MainWindow::slot_slideButtonClicked()
{
    static  QList<qint32> base = mainSplitter->sizes(); //keep the initial pos
    QList<qint32>  newSize;

    if(matrixTableWidget->width() != 0)
    {
        newSize << 0 << base.at(1);
    mainSplitter->setSizes(newSize);  //move the SPLITTER
        QRect rec = this->geometry();
        int x, y, w, h;
        rec.getRect(&x, &y, &w, &h);
        this->setGeometry(x + base.at(0), y, w - base.at(0), h);
    }
    else
    {
        newSize << base.at(0) << base.at(1);
mainSplitter->setSizes(newSize);  //move the SPLITTER
        QRect rec = this->geometry();
        int x, y, w, h;
        rec.getRect(&x, &y, &w, &h);
        this->setGeometry(x - base.at(0), y, w + base.at(0), h);
    }
    //mainSplitter->setSizes(newSize);  //move the SPLITTER

    setBtnPos();
    setBtnIcon();

}



MainWindow::~MainWindow()
{
    delete ui;
}


