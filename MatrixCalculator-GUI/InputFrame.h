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

    void on_importButton_clicked();

    void on_exportButton_clicked();

    void on_quitButton_clicked();

signals:
    void signal_calculate(QString exp);
    void signal_export_matrix();
    void signal_import_matrix();
    void signal_close();
    void signal_error(int index, int size);

private:
    Ui::InputFrame *ui;
};

#endif // INPUTFRAME_H
