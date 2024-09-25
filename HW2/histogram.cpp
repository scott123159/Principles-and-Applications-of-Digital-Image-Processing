#include "histogram.h"
#include "ui_histogram.h"
#include "ImageProcessing.h"

#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QVBoxLayout>

Histogram::Histogram(const QImage& src, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Histogram)
{
    ui->setupUi(this);
    //setGeometry(coordinate);

    QVector<int> hist = cv::calcHist(src);                  //Prepare the bin with size 256

    QBarSet* barSet = new QBarSet("Calculate the pixel");

    for (const int& i : hist)                               //Add count of pixel value of the image in chart
        barSet->append(i);

    QBarSeries* barBeries = new QBarSeries();
    barBeries->append(barSet);

    QChart* chart = new QChart();
    chart->addSeries(barBeries);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(chartView);
    setLayout(vboxLayout);                                  //Show the histogram with QCharts library
}

Histogram::~Histogram()
{
    delete ui;
}
