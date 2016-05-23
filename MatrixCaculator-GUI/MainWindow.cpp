#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->addButton->setIcon(QIcon(":/src/add.png"));
    ui->deleteButton->setIcon(QIcon(":/src/delete.png"));
    ui->modifyButton->setIcon(QIcon(":/src/modify.png"));
    ui->inputButton->setIcon(QIcon(":/src/in.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
