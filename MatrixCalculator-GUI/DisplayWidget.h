#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QFrame>
#include "Matrix.h"

namespace Ui {
class DisplayWidget;
}

class DisplayWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DisplayWidget(QWidget *parent = 0);
    ~DisplayWidget();

public slots:
    void slot_display_matrix(Matrix mat);
    void slot_display_output(Matrix mat);

private slots:
    void on_transposeButton_clicked();

    void on_detButton_clicked();

    void on_inverseButton_clicked();

    void on_applyButton_clicked();

signals:
    void apply_matrix(QString prevName, Matrix mat, QString name, QString info);

private:
    Ui::DisplayWidget *ui;
};

#endif // DISPLAYWIDGET_H
