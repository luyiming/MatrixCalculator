#include "InputFrame.h"
#include "ui_InputFrame.h"
#include "Highlighter.h"
#include <QDebug>
#include <QTextDocumentFragment>

InputFrame::InputFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InputFrame)
{
    ui->setupUi(this);
    ui->historyText->setEnabled(false);
    Highlighter *highlighter = new Highlighter(ui->inputLine->document());
}

InputFrame::~InputFrame()
{
    delete ui;
}

void InputFrame::on_calcButton_clicked()
{
    QString exp = ui->inputLine->toPlainText().trimmed();
    emit signal_calculate(exp);
    QTextCursor cursor = ui->inputLine->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    ui->historyText->textCursor().insertFragment(cursor.selection());
    //cursor.movePosition(QTextCursor::End);
    //ui->historyText->setTextCursor(cursor);
    //ui->historyText->setDocument(ui->inputLine->document());


//    QTextBlock::iterator it;
//    for (it = ui->inputLine->document()->begin().begin(); !(it.atEnd()); ++it) {
//        QTextFragment currentFragment = it.fragment();
//        if (currentFragment.isValid())
//            cursor.insertFragment(currentFragment);

       // QTextDocumentFragment frag(ui->inputLine->document());


}
