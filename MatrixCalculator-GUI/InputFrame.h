#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QFrame>
#include <QString>
#include "Calculator.h"
#include "Highlighter.h"
#include <QDebug>
#include <QTextDocumentFragment>

namespace Ui {
class InputFrame;
}

class InputFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InputFrame(QWidget *parent = 0);
    ~InputFrame();

private slots:
    void on_calcButton_clicked();

    void on_importButton_clicked();

    void on_exportButton_clicked();

    void on_quitButton_clicked();

public slots:
    void slot_setError(int position, int offset);

signals:
    void signal_calculate(QString exp);
    void signal_export_matrix();
    void signal_import_matrix();
    void signal_close();
//    void signal_error(int index, int size);

private:
    Ui::InputFrame *ui;
    Highlighter *highlighter;
};

#endif // INPUTFRAME_H
