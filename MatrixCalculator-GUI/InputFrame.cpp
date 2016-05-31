#include "InputFrame.h"
#include "ui_InputFrame.h"

InputFrame::InputFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InputFrame)
{
    ui->setupUi(this);
}

InputFrame::~InputFrame()
{
    delete ui;
}
