#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(const QImage& src, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Histogram)
{
    ui->setupUi(this);
    qDebug() << src;
}

Histogram::~Histogram()
{
    delete ui;
}
