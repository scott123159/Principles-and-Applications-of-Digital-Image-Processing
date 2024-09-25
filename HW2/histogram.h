#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QDialog>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>

#include "ImageProcessing.h"

namespace Ui {
class Histogram;
}

class Histogram : public QDialog
{
    Q_OBJECT

public:
    explicit Histogram(const QImage& src, QWidget *parent = nullptr);
    ~Histogram();

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
