#ifndef INPUTFRAME_H
#define INPUTFRAME_H

#include <QFrame>

namespace Ui {
class InputFrame;
}

class InputFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InputFrame(QWidget *parent = 0);
    ~InputFrame();

private:
    Ui::InputFrame *ui;
};

#endif // INPUTFRAME_H
