#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_exitAction_triggered();

    void on_openFileAction_triggered();

    void on_undoAction_triggered();

    void on_redoAction_triggered();

    void on_grayScaleAction1_triggered();

    void on_grayScaleAction2_triggered();

    void on_diffAction_triggered();

private:
    Ui::MainWindow *ui;

    /*For undo and redo stack*/
    QStack<QImage> undo;
    QStack<QImage> redo;

    /*Original image*/
    QImage image;

    /*For difference of two grayscale methods*/
    QImage diffImage;

    /*Set UI geometry when opening image*/
    void setUIGeometry();
};
#endif // MAINWINDOW_H
