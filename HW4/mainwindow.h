#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:

  void on_cut_off_scrollbar_valueChanged(int value);

  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  Ui::MainWindow *ui;
  QImage img;
  QString file_name;
  QButtonGroup* buttongroup;
};
#endif // MAINWINDOW_H
