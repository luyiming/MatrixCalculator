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

    mainSplitter = new QSplitter(Qt::Horizontal, this);
    rightSplitter = new QSplitter(Qt::Vertical, this);

    slideButton= new QPushButton(this); // after Splitter
    slideButton->setFocusPolicy(Qt::NoFocus);

    rightSplitter->addWidget(displayWidget);
    rightSplitter->addWidget(inputFrame);
    mainSplitter->addWidget(matrixTableWidget);
    mainSplitter->addWidget(rightSplitter);
    //mainSplitter->setStretchFactor(1, 1);//left not strenchable
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

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

   int isWidth  = slideButton->pos().x() + slideButton->width();
   int isHeight = slideButton->pos().y() + slideButton->height();

   int x = event->pos().x();
   int y = event->pos().y();

//   if(x > slideButton->pos().x() && x < isWidth && y > slideButton->y() && y < isHeight)
//   {
//       slideButton->setVisible(true);
//   }

//   else
//   {
//       slideButton->setVisible(false);
//   }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::on_addButton_2_clicked()
{
    AddMatrixDialog *dlg = new AddMatrixDialog(this);
    dlg->show();
}

void MainWindow::on_addButton_clicked()
{
    int row, coloum;
    QString matName = ui->matrixNameLine->text().trimmed();
    if(matrices.contains(matName))
    {
        qDebug() << "already have matrix" << matName;
        return;
    }

    QStringList list = ui->matrixText->toPlainText().trimmed().split('\n');
    row = list.size();
    if(row == 0)
        return;
    coloum = list.at(0).trimmed().split(QRegExp("[ ,|]")).size();
    Matrix mat(row, coloum);
    for(int r = 0; r < row; r++)
    {
        QStringList list2 = list.at(r).trimmed().split(QRegExp("[ ,|]"));
        if(coloum != list2.size())
        {
            qDebug() << "coloum not equal";
            return;
        }
        for(int c = 0; c < coloum; c++)
            mat[r][c] = list2.at(c).toDouble();
    }
    matrices.insert(matName, mat);
    mat.print();

    int curRow = ui->matrixTable->rowCount();
    ui->matrixTable->insertRow(curRow);
    ui->matrixTable->setItem(curRow, 0 , new QTableWidgetItem(matName));
    ui->matrixTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(row)));
    ui->matrixTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(coloum)));
}

void MainWindow::on_calcButton_clicked()
{
    char *exp = new char[ui->expLine->toPlainText().trimmed().size() + 1];
    strcpy(exp, ui->expLine->toPlainText().trimmed().toStdString().data());
    qDebug() << exp;
    Calculator calc(matrices);
    Matrix res = calc.calculate(exp);
    ui->outputText->clear();
    for(int r = 0; r < res.row; ++r)
    {
        QString str;
        for(int c = 0; c < res.coloum; ++c)
            str += (QString::number(res[r][c]) + QString("\t"));
        ui->outputText->append(str);
    }
    delete exp;
}

int MainWindow::getMatrix(Matrix &result)
{
    //return:
    // 1 success
    // -1 no matrix
    // -2 coloum not equal
    int row, coloum;
    QStringList list = ui->matrixText->toPlainText().trimmed().split('\n');
    row = list.size();
    if(row == 0)
        return -1;
    coloum = list.at(0).trimmed().split(QRegExp("[ ,|]")).size();
    result.resize(row, coloum);
    for(int r = 0; r < row; r++)
    {
        QStringList list2 = list.at(r).trimmed().split(QRegExp("[ ,|]"));
        if(coloum != list2.size())
        {
            qDebug() << "coloum not equal";
            return -2;
        }
        for(int c = 0; c < coloum; c++)
            result[r][c] = list2.at(c).toDouble();
    }
    return 1;
}

void MainWindow::on_inverseButton_clicked()
{
    Matrix mat;
    int rtn = getMatrix(mat);
    switch(rtn)
    {
        case 1 :
        {
            Matrix res = mat.inverse();
            ui->outputText->clear();
            for(int r = 0; r < res.row; ++r)
            {
                QString str;
                for(int c = 0; c < res.coloum; ++c)
                    str += (QString::number(res[r][c]) + QString("\t"));
                ui->outputText->append(str);
            }
            //res.print();
            return;
        }
        case -1 : {qDebug() << "no matrix"; return;}
        case -2 : {qDebug() << "coloum not equal"; return;}
    }
}

void MainWindow::on_detButton_clicked()
{
    Matrix mat;
    int rtn = getMatrix(mat);

    switch(rtn)
    {
        case 1  : {ui->outputText->clear(); ui->outputText->setText(QString::number(mat.det())); return; }
        case -1 : {qDebug() << "no matrix"; return;}
        case -2 : {qDebug() << "coloum not equal"; return;}
    }
}

void MainWindow::on_deleteButton_clicked()
{
    int row = ui->matrixTable->currentRow();
    QString matName = ui->matrixTable->item(row, 0)->text();
    if(!matrices.contains(matName))
    {
        qDebug() << "delete error, no matrix";
        return;
    }
    matrices.remove(matName);
    ui->matrixTable->removeRow(row);
}

void MainWindow::on_transposeButton_clicked()
{
    Matrix mat;
    int rtn = getMatrix(mat);
    switch(rtn)
    {
        case 1 :
        {
            Matrix res = mat.transpose();
            ui->outputText->clear();
            for(int r = 0; r < res.row; ++r)
            {
                QString str;
                for(int c = 0; c < res.coloum; ++c)
                    str += (QString::number(res[r][c]) + QString("\t"));
                ui->outputText->append(str);
            }
            //res.print();
            return;
        }
        case -1 : {qDebug() << "no matrix"; return;}
        case -2 : {qDebug() << "coloum not equal"; return;}
    }
}

void MainWindow::on_exportButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("请选择导出文件"), ".");
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    QFile file(path);
    file.open(QFile::WriteOnly);
    QTextStream fout(&file);
    for(auto it = matrices.begin(); it != matrices.end(); ++it)
    {
        fout << it.key() << " " << it->row << " " << it->coloum << endl;
        for(int r = 0; r < it->row; ++r)
        {
            for(int c = 0; c < it->coloum; ++c)
            {
                fout << it.value()[r][c] << " ";
            }
            fout << endl;
        }
    }
    QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导出文件成功"));
}

void MainWindow::on_importButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择导入文件"), ".");
    if(path.isEmpty() == true)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("失败"), QString::fromLocal8Bit("未指定路径"));
        return;
    }
    QFile file(path);
    file.open(QFile::ReadOnly);
    QTextStream fin(&file);
    int row, coloum;
    QString matName;
    while(!fin.atEnd())
    {
        fin >> matName >> row >> coloum;
qDebug() << matName << " " << row << " " << coloum;
        Matrix mat(row, coloum);
        for(int r = 0; r < row; r++)
        {
            for(int c = 0; c < coloum; c++)
            {
                fin >> mat[r][c];
            }
        }

        matrices.insert(matName, mat);
        mat.print();

        int curRow = ui->matrixTable->rowCount();
        ui->matrixTable->insertRow(curRow);
        ui->matrixTable->setItem(curRow, 0 , new QTableWidgetItem(matName));
        ui->matrixTable->setItem(curRow, 1 , new QTableWidgetItem(QString::number(row)));
        ui->matrixTable->setItem(curRow, 2 , new QTableWidgetItem(QString::number(coloum)));
    }
    QMessageBox::information(this, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("导入文件成功"));
}
