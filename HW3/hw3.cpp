#include "hw3.h"
#include "ui_hw3.h"
#include "imgproc.h"

#include <QFileDialog>

HW3::HW3(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HW3)
{
    ui->setupUi(this);

    /*Set fixed size 800x600*/
    setFixedSize(800, 600);

    /*Create dialog objects*/
    gaussianBlurDialog = new GaussianBlurDialog(this);
    medianBlurDialog = new MedianBlurDialog(this);
    marrHildrethEdgeDetectionDialog = new MarrHildrethEdgeDetectionDialog(this);
    localEnhancementDialog = new LocalEnhancementDialog(this);

    /*Connect signals and slots when user click apply button*/
    connect(gaussianBlurDialog, &GaussianBlurDialog::sendSigmaAndKernelSize, this, &HW3::applyGaussianBlur);
    connect(medianBlurDialog, &MedianBlurDialog::sendKernelSize, this, &HW3::applyMedianBlur);
    connect(marrHildrethEdgeDetectionDialog, &MarrHildrethEdgeDetectionDialog::sendThresholdAndKernelSize, this, &HW3::applyMarrHildreth);
    connect(localEnhancementDialog, &LocalEnhancementDialog::applyLocalEnhancemant, this, &HW3::applyLocalEnhancement);
}

HW3::~HW3()
{
    delete ui;
}

void HW3::applyGaussianBlur(double sigma, int kernelSize)
{
    /*Save previous state*/
    undoStack.push_back(image);

    /*Clear the redo stack*/
    redoStack.clear();

    /*Start the timer*/
    timer.start();

    /*1. Get the orginal Gaussian kernel with sigma and kernelSize parameters
      2. Flip the kernel
      3. Apply convolution with orginal image
    */
    image = cv::convolution(image, cv::flipKernel(cv::getGaussianKernel(sigma, kernelSize)));

    /*Stop the timer and calculate the elapsed time*/
    const double elapsedTime = timer.elapsed() / 1000.;

    /*Display the elapsed time on status bar*/
    ui->statusbar->showMessage(QString("CPU time: %1 (sec.)").arg(elapsedTime));

    /*Display new image on QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::applyMedianBlur(int kernelSize)
{
    /*Save previous state*/
    undoStack.push_back(image);

    /*Clear the redo stack*/
    redoStack.clear();

    /*Start the timer*/
    timer.start();

    image = cv::applyMedianBlur(image, kernelSize);

    /*Stop the timer and calculate the elapsed time*/
    const double elapsedTime = timer.elapsed() / 1000.;

    /*Display the elapsed time on status bar*/
    ui->statusbar->showMessage(QString("CPU time: %1 (sec.)").arg(elapsedTime));

    /*Display new image on QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::applyMarrHildreth(const int size, const double threshold)
{
    /*Save previous state*/
    undoStack.push_back(image);

    /*Clear the redo stack*/
    redoStack.clear();

    /*Start the timer*/
    timer.start();

    /*1. Get the orginal Gaussian kernel with sigma and kernelSize parameters
      2. Flip the kernel
      3. Apply convolution with orginal image
    */
    image = cv::applyMarrHildreth(image, size, threshold);

    /*Stop the timer and calculate the elapsed time*/
    const double elapsedTime = timer.elapsed() / 1000.;

    /*Display the elapsed time on status bar*/
    ui->statusbar->showMessage(QString("CPU time: %1 (sec.)").arg(elapsedTime));

    /*Display new image on QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::applyLocalEnhancement(int size, double c, double k0, double k1, double k2, double k3)
{
    /*Save previous state*/
    undoStack.push_back(image);

    /*Clear the redo stack*/
    redoStack.clear();

    /*Start the timer*/
    timer.start();

    image = cv::applyLocalHistogramEnhancement(image, size, k0, k1, k2, k3, c);

    /*Stop the timer and calculate the elapsed time*/
    const double elapsedTime = timer.elapsed() / 1000.;

    /*Display the elapsed time on status bar*/
    ui->statusbar->showMessage(QString("CPU time: %1 (sec.)").arg(elapsedTime));

    /*Display new image on QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::on_actionExit_triggered()
{
    /*Release and close this application when user clicked "Exit" or pressed Ctrl-Q*/
    close();
}

void HW3::on_actionOpen_triggered()
{
    /*讀取影像檔案名稱*/
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Image files (*.jpg *.jpeg *.bmp *.png);;All files (*.*)"));

    /*影像檔案名稱不可為空*/
    if (fileName.isEmpty())
        return;

    undoStack.clear();
    redoStack.clear();

    image = QImage(fileName);
    // image = cv::applyLocalHistogramEnhancement(image, 3, 0, 0.1, 0, 0.1, 22.8);

    /*顯示影像並調整視窗大小*/
    ui->image->setPixmap(QPixmap::fromImage(image));

    /*留一點空間*/
    const int offset = 40;
    setFixedSize(image.width() + offset, image.height() + offset * 2);
}

void HW3::on_actionUndo_triggered()
{
    /*The stack can not be empty*/
    if (undoStack.isEmpty())
        return;

    /*Save this state*/
    redoStack.push_back(image);

    /*Pop from stack*/
    image = undoStack.pop();
    ui->image->setPixmap(QPixmap::fromImage(image));
}
void HW3::on_actionRedo_triggered()
{
    /*The stack can not be empty*/
    if (redoStack.isEmpty())
        return;

    /*Save this state*/
    undoStack.push_back(image);

    /*Pop from stack*/
    image = redoStack.pop();
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::on_actionGaussian_Blur_triggered()
{
    /*Image can not be null or empty*/
    if (image.isNull())
        return;

    /*Display Gaussian blur dialog*/
    gaussianBlurDialog->show();
}

void HW3::on_actionMedian_Blur_triggered()
{
    /*Image can not be null or empty*/
    if (image.isNull())
        return;

    /*Display median blur dialog*/
    medianBlurDialog->show();
}

void HW3::on_actionSobel_triggered()
{
    if (image.isNull())
        return;

    /*Save previous state*/
    undoStack.push_back(image);

    /*Clear the redo stack*/
    redoStack.clear();

    /*Start the timer*/
    timer.start();

    /*Apply Sobel operator on image*/
    image = cv::applySobel(image);

    /*Stop the timer and calculate the elapsed time*/
    const double elapsedTime = timer.elapsed() / 1000.;

    /*Display the elapsed time on status bar*/
    ui->statusbar->showMessage(QString("CPU time: %1 (sec.)").arg(elapsedTime));

    /*Display new image on QLabel*/
    ui->image->setPixmap(QPixmap::fromImage(image));
}

void HW3::on_actionMarr_Hildreth_triggered()
{
    /*Image can not be null or empty*/
    // if (image.isNull())
    //     return;

    /*Display edge detection dialog*/
    marrHildrethEdgeDetectionDialog->show();
}

void HW3::on_actionLocal_triggered()
{
    if (image.isNull())
        return;

    localEnhancementDialog->show();
}
