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

private:
    Ui::MainWindow *ui;
    QStack<QImage> undo;
    QStack<QImage> redo;
    QImage image;
};
#endif // MAINWINDOW_H
