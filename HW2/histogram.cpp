#include "histogram.h"
#include "ui_histogram.h"

Histogram::Histogram(const QImage& src, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Histogram)
{
    ui->setupUi(this);

    QVector<int> hist = cv::calcHist(src);

    QBarSet* barSet = new QBarSet("");

    for (const int& i : hist) {
        barSet->append(i);
    }

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
    setLayout(vboxLayout);
}

Histogram::~Histogram()
{
    delete ui;
}
