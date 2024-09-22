#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->undoAction->setDisabled(true);
    ui->redoAction->setDisabled(true);
    this->setWindowTitle("HW2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}

void MainWindow::on_openFileAction_triggered()
{
    /*Open image file that store in current folder*/
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    ".\\",
                                                    tr("Images (*.JPEG, *.BMP, *.PNG);;All Files (*.*)"));

    /*If file is not selected, then return program*/
    if (fileName.isNull()) return;
    /*Set the program title with file that you selected*/
    this->setWindowTitle(this->windowTitle() + " " + fileName.mid(fileName.lastIndexOf('/') + 1));
    /*Set current state*/
    this->image = QImage(fileName);
    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_undoAction_triggered()
{
    if (undo.isEmpty()) return;
}

void MainWindow::on_redoAction_triggered()
{
    if (redo.isEmpty()) return;
}
