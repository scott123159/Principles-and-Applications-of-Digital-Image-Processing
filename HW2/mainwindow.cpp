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

    /*Create dialog window object for threshold*/
    thresholdDialog = new ThresholdDialog(this);

    /*Create dialog window object for Brightness-Constrast*/
    bcDialog = new BrightnessContrastDialog(this);

    /*connect signals and slots for threshold dialog*/
    connect(thresholdDialog, &ThresholdDialog::adjustThreshhold, this, &MainWindow::adjustThreshold);
    connect(thresholdDialog, &ThresholdDialog::cancel, this, &MainWindow::cancel);
    connect(thresholdDialog, &ThresholdDialog::ok, this, &MainWindow::saveThreshold);

    /*connect signals and slots for brightness and contrast dialog*/
    connect(bcDialog, &BrightnessContrastDialog::adjustBrightness, this, &MainWindow::adjustBrightness);
    connect(bcDialog, &BrightnessContrastDialog::adjustContrast, this, &MainWindow::adjustContrast);
    connect(bcDialog, &BrightnessContrastDialog::cancel, this, &MainWindow::cancel);
    connect(bcDialog, &BrightnessContrastDialog::ok, this, &MainWindow::saveBrightnessAndContrast);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::adjustThreshold(int threshold)
{
    /*Slide the slider in threshold dialog, then change the value of current image*/
    ui->image->setPixmap(QPixmap::fromImage(cv::threshold(image, threshold)));
}

void MainWindow::saveThreshold(int threshold)
{
    undo.push(image);
    redo.clear();
    image = cv::threshold(image, threshold);
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::cancel()
{
    /*If the cancel button in dialog was clicked,*/
    /*then restore the previous image*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::saveBrightnessAndContrast(int b, int c)
{
    undo.push(image);
    redo.clear();
    image = cv::contrast(cv::brightness(image, b), c);
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::adjustBrightness(int value)
{
    //processedImage = cv::brightness(image, value);
    ui->image->setPixmap(QPixmap::fromImage(cv::brightness(image, value)));
}

void MainWindow::adjustContrast(int value)
{
    //processedImage = cv::contrast(processedImage, value);
    ui->image->setPixmap(QPixmap::fromImage(cv::contrast(image, value)));
}

void MainWindow::setUIGeometry() {
    /*Adjust the window size to fit the image size*/
    setGeometry(x(), y(), image.width(), image.height());
}

void MainWindow::on_exitAction_triggered()
{
    close();
}

void MainWindow::on_openFileAction_triggered()
{
    /*Open an image file that store in current folder*/
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    QCoreApplication::applicationDirPath(),
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
    setUIGeometry();
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
    setUIGeometry();
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

void MainWindow::on_thresholdAction_triggered()
{
    if (image.isNull()) return;
    thresholdDialog->show();
}


void MainWindow::on_zoomInAction_triggered()
{
    if (image.isNull()) return;

    undo.push(image);
    image = cv::nearestNeighborInterpolation(image, 1.1);

    redo.clear();

    ui->image->setPixmap(QPixmap::fromImage(image));
    setUIGeometry();
}


void MainWindow::on_zoomOutAction_triggered()
{
    if (image.isNull()) return;

    undo.push(image);
    image = cv::nearestNeighborInterpolation(image, 0.9);

    redo.clear();

    ui->image->setPixmap(QPixmap::fromImage(image));
    setUIGeometry();
}


void MainWindow::on_actionBrightness_Contrast_triggered()
{
    if (image.isNull()) return;
    bcDialog->show();
}

