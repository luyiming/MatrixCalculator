#include "InputFrame.h"
#include "ui_InputFrame.h"
#include "Highlighter.h"
#include <QDebug>
#include <QTextDocumentFragment>
#include <QMessageBox>

InputFrame::InputFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InputFrame)
{
    ui->setupUi(this);
    ui->historyText->setEnabled(false);
    highlighter = new Highlighter(ui->inputLine->document());
}

InputFrame::~InputFrame()
{
    delete ui;
}

void InputFrame::on_calcButton_clicked()
{
    QString exp = ui->inputLine->toPlainText().trimmed();
    emit signal_calculate(exp);
    ui->historyText->append(exp);
}


void InputFrame::on_importButton_clicked()
{
    emit signal_import_matrix();
}

void InputFrame::on_exportButton_clicked()
{
    emit signal_export_matrix();
}

void InputFrame::on_quitButton_clicked()
{
    emit signal_close();
}

void InputFrame::slot_setError(int position, int offset)
{
    highlighter->setError(position, offset);
}
