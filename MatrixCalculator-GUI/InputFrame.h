#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QFrame>
#include <QString>

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

signals:
    void signal_calculate(QString exp);

private:
    Ui::InputFrame *ui;
};

#endif // INPUTFRAME_H
