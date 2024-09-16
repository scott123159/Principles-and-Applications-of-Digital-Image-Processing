#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_open_file_action_triggered();

    void on_exit_app_action_triggered();

    void on_average_button_clicked();

    void on_value_slider_valueChanged(int value);

    void on_function_button_clicked();

private:
    Ui::MainWindow *ui;
    QImage raw_image;
    QImage processed_image;
};
#endif // MAINWINDOW_H
