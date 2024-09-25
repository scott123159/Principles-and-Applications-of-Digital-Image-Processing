#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImageProcessing.h"
#include "histogram.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUIGeometry() {
    this->setGeometry(this->x(),
                      this->y(),
                      this->image.width() + 20,
                      this->image.height() + 70);
}

void MainWindow::on_exitAction_triggered()
{
    close();
}

void MainWindow::on_openFileAction_triggered()
{
    /*Open an image file that store in current folder*/
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    "",
                                                    tr("Image files (*.jpg *.jpeg *.bmp *.png);;All files (*.*)"));

    /*If the image file is not selected, then do nothing*/
    if (fileName.isNull()) return;

    /*Clear the undo/redo stack*/
    if (!undo.isEmpty()) undo.clear();
    if (!redo.isEmpty()) redo.clear();

    /*Set the program title with file that you selected*/
    setWindowTitle("HW2 " + fileName.mid(fileName.lastIndexOf('/') + 1));

    /*Set current state*/
    image = QImage(fileName);
    diffImage = cv::diff(cv::cvtColor(image, cv::COLOR_RGB2GRAY1),
                         cv::cvtColor(image, cv::COLOR_RGB2GRAY2));

    /*Display the image on QLabel*/
    setUIGeometry();
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_undoAction_triggered()
{
    /*If the stack is empty then return it*/
    if (undo.isEmpty()) return;

    /*Push current image into the redo stack*/
    redo.push(image);
    image = undo.pop();

    /*Display new image on QLable*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_redoAction_triggered()
{
    /*If the stack is empty then return it*/
    if (redo.isEmpty()) return;

    /*Push current image into the redo stack*/
    undo.push(image);
    image = redo.pop();

    /*Display new image on QLable*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_grayScaleAction1_triggered()
{
    /*If there is no image was selected then return it*/
    if (image.isNull()) return;

    /*Push current image into undo stack*/
    undo.push(image);
    image = cv::cvtColor(image, cv::COLOR_RGB2GRAY1);
    redo.clear();

    /*Show the histogram*/
    Histogram* hist = new Histogram(image, this);
    hist->show();

    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_grayScaleAction2_triggered()
{
    /*If there is no image was selected then return it*/
    if (image.isNull()) return;

    /*Push current image into undo stack*/
    undo.push(image);
    image = cv::cvtColor(image, cv::COLOR_RGB2GRAY2);
    redo.clear();

    /*Show the histogram*/
    Histogram* hist = new Histogram(image, this);
    hist->show();

    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_diffAction_triggered()
{
    /*If there is no image was selected then return it*/
    if (image.isNull()) return;

    /*Push current image into undo stack*/
    undo.push(image);
    image = diffImage;
    redo.clear();

    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}
