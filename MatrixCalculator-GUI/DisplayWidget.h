#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QFrame>

namespace Ui {
class DisplayWidget;
}

class DisplayWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

private:
    Ui::DisplayWidget *ui;
};

#endif // DISPLAYWIDGET_H
