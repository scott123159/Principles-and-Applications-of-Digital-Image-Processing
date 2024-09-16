#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Initialize the value of slider label*/
    ui->value_label->setText(QString::number(ui->value_slider->value()));

    /*Set the title of this program*/
    this->setWindowTitle("HW1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_file_action_triggered()
{
    /*Clean the histogram up*/
    if (!ui->raw_histogram_vertical_layout->isEmpty())
        ui->raw_histogram_vertical_layout->removeItem(ui->raw_histogram_vertical_layout->itemAt(0));

    /*Read the .64 file from the disk then turn it into QImage object*/
    this->raw_image = file_to_QImage();

    /*The QImage can't be emtpy
     * If it is, do nothing*/
    if (this->raw_image.isNull())
        return;

    /*Clear raw image lebel*/
    ui->raw_image->clear();

    /*Get height and width of raw image*/
    const int h = this->raw_image.height(), w = this->raw_image.width();

    /*Obtain the image with gray level enhancement by 8*/
    const QVector<uchar> raw_image_vector = QImage_to_QVector(this->raw_image);
    QImage new_image = QVector_to_QImage(mul(raw_image_vector, 8), w, h).
                       /*Zoom the image in*/
                       scaled(ui->raw_image->width(), ui->raw_image->height(),Qt::KeepAspectRatio);

    /*Show the image*/
    ui->raw_image->setPixmap(QPixmap::fromImage(new_image));

    /*Show the histogram which processed by create_bar_chart, calc_hist and QImage_to_QVector functions*/
    ui->raw_histogram_vertical_layout->addWidget(create_bar_chart(calc_hist(QImage_to_QVector(this->raw_image))));
}

void MainWindow::on_exit_app_action_triggered()
{
    /*Close the application*/
    this->close();
}

void MainWindow::on_average_button_clicked()
{
    /*Check if the raw image is empty*/
    if (this->raw_image.isNull())
        return;

    /*Open the file that will be operated average value on raw image*/
    QImage another_image = file_to_QImage();
    /*If there is no image then just return it*/
    if (another_image.isNull())
        return;

    /*Clear the histogram*/
    if (!ui->processed_histogram_vertical_layout->isEmpty())
        ui->processed_histogram_vertical_layout->removeItem(ui->processed_histogram_vertical_layout->itemAt(0));

    ui->processed_image->clear();

    /*Get the size from origin image*/
    const int h = this->raw_image.height(), w = this->raw_image.width();

    /*Use two QVectors to store origin image and another image*/
    const QVector<uchar> raw_image_vector = QImage_to_QVector(this->raw_image);
    const QVector<uchar> another_image_vector = QImage_to_QVector(another_image);

    /*Calculate the new image by averaging two images above*/
    QVector<uchar> new_image_vector;
    for (int i = 0; i < h * w; i++)
        new_image_vector.push_back((raw_image_vector.at(i) + another_image_vector.at(i)) / 2);
    QImage new_image = QVector_to_QImage(mul(new_image_vector, 8), w, h).
                       scaled(ui->processed_image->width(),ui->processed_image->height(),Qt::KeepAspectRatio);

    ui->processed_image->setPixmap(QPixmap::fromImage(new_image));
    ui->processed_histogram_vertical_layout->addWidget(create_bar_chart(calc_hist(new_image_vector)));
}

void MainWindow::on_value_slider_valueChanged(int value)
{
    ui->value_label->setText(QString::number(value));

    /*Check if the raw image is empty*/
    if (this->raw_image.isNull())
        return;

    /*Clear the histogram*/
    if (!ui->processed_histogram_vertical_layout->isEmpty())
        ui->processed_histogram_vertical_layout->removeItem(ui->processed_histogram_vertical_layout->itemAt(0));

    /*Use two QVectors to store origin image and another image*/
    const QVector<uchar> raw_image_vector = QImage_to_QVector(this->raw_image);
    QVector<uchar> new_image_vector;

    /*Select the radio button that performs addition or subtraction operation*/
    if (ui->add_or_sub_radio_button->isChecked())
        new_image_vector = add_or_sub(raw_image_vector, value);

    if (ui->mul_radio_button->isChecked())
        new_image_vector = mul(raw_image_vector, value);

    QImage new_image = QVector_to_QImage(new_image_vector, this->raw_image.width(), this->raw_image.height()).
                       scaled(ui->processed_image->width(),ui->processed_image->height(),Qt::KeepAspectRatio);
    ui->processed_image->setPixmap(QPixmap::fromImage(new_image));
    ui->processed_histogram_vertical_layout->addWidget(create_bar_chart(calc_hist(new_image_vector)));
}

void MainWindow::on_function_button_clicked()
{
    /*Check if the raw image is empty*/
    if (this->raw_image.isNull())
        return;

    /*Clear the histogram*/
    if (!ui->processed_histogram_vertical_layout->isEmpty())
        ui->processed_histogram_vertical_layout->removeItem(ui->processed_histogram_vertical_layout->itemAt(0));

    const int h = this->raw_image.height(), w = this->raw_image.width();
    QImage new_image(w, h, QImage::Format_RGB32);
    /*g(x, y) = f(x, y) - f(x - 1, y)*/
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (x - 1 >= 0) {
                int pixel = qGray(this->raw_image.pixel(x, y)) - qGray(this->raw_image.pixel(x - 1, y));
                pixel = pixel < 0 ? 0 : pixel;
                new_image.setPixel(x, y, qRgb(pixel, pixel, pixel));
            }
            else {
                int pixel = qGray(this->raw_image.pixel(x, y));
                new_image.setPixel(x, y, qRgb(pixel, pixel, pixel));
            }
        }
    }

    ui->processed_histogram_vertical_layout->addWidget(create_bar_chart(calc_hist(QImage_to_QVector(new_image))));

    new_image = QVector_to_QImage(mul(QImage_to_QVector(new_image), 8), this->raw_image.width(), this->raw_image.height()).
                       scaled(ui->processed_image->width(),ui->processed_image->height(),Qt::KeepAspectRatio);
    ui->processed_image->setPixmap(QPixmap::fromImage(new_image));
}

