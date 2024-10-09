#ifndef HW3_H
#define HW3_H

#include <QMainWindow>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class HW3;
}
QT_END_NAMESPACE

class HW3 : public QMainWindow
{
    Q_OBJECT

public:
    HW3(QWidget *parent = nullptr);
    ~HW3();

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

private:
    Ui::HW3 *ui;

    /*原始影像*/
    QImage image;

    /*用來存放undo/redo的stack*/
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;
};
#endif // HW3_H
