#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*Set program title*/
    this->setWindowTitle("HW2");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUIGeometry() {
    /*Set window and label size*/
    // ui->image->setGeometry(ui->image->x() + 20,
    //                        ui->image->y() + 20,
    //                        this->image.width(),
    //                        this->image.height());
    this->setGeometry(this->x(),
                      this->y(),
                      this->image.width() + 20,
                      this->image.height() + 70);
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
                                                    tr("Image files (*.jpg *.jpeg *.bmp *.png);;All files (*.*)"));

    /*If file is not selected, then return program*/
    if (fileName.isNull()) return;

    /*Clear the undo/redo stack*/
    if (!this->undo.isEmpty()) this->undo.clear();
    if (!this->redo.isEmpty()) this->redo.clear();
    /*Set the program title with file that you selected*/
    this->setWindowTitle("HW2 " + fileName.mid(fileName.lastIndexOf('/') + 1));
    /*Set current state*/
    this->image = QImage(fileName);
    this->setUIGeometry();
    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_undoAction_triggered()
{
    /*If the stack is empty then return it*/
    if (undo.isEmpty()) return;

    /*Push current image into the redo stack*/
    redo.push(this->image);
    this->image = undo.pop();

    /*Display new image on QLable*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_redoAction_triggered()
{
    /*If the stack is empty then return it*/
    if (redo.isEmpty()) return;

    /*Push current image into the redo stack*/
    undo.push(this->image);
    this->image = redo.pop();

    /*Display new image on QLable*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_grayScaleAction1_triggered()
{
    /*If there is no image was selected then return it*/
    if (image.isNull()) return;

    /*Push current image into undo stack*/
    this->undo.push(image);

    /*Use "GRAY = (R + G + B) / 3.0" method to transform image into grayscale image*/
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QRgb pixel = image.pixel(x, y);
            const uchar newPixel = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3.0;
            image.setPixelColor(x, y, QColor(newPixel, newPixel, newPixel));
        }
    }
    /*Display the image that you selected on the QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}
