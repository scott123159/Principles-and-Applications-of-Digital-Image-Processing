#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVector<int> x;
    x.append(0);
    x.push_back(1);
    auto ptr = x.data();
    qDebug() << *(ptr + 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
