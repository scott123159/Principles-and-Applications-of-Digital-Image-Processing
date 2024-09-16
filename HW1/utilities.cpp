#include "utilities.h"

/*This is the implementation of utilities.h*/

QImage file_to_QImage() {
    /*Use Qt file system utilities to read .64 file from disk*/
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Open File",
        QApplication::applicationDirPath(),
        ".64 file (*.64)");

    /*If no file is selected, return empty QImage*/
    if (fileName.isEmpty())
        return QImage();

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    /*Chaining programming style*/
    QString content = in.readAll().
                      remove("\r").
                      remove("\n");
    file.close();

    const int N = 64;
    QVector<uchar> src;

    /*std::transform is like reduce/fold from other functional programming languages such as haskell, TS*/
    std::transform(content.begin(),
                  content.end(),
                  std::back_inserter(src),
                  [](const QChar& c) { return c.toLatin1(); });

    QImage dst(N, N, QImage::Format_RGB32);

    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            /*Set pixel of image f(x, y)*/
            const uchar pixel = src[y * N + x];
            /*Map 0, 1, ..., a, b, ..., z to 0-31 graylevel*/
            const uchar newPixel = isdigit(pixel) ? pixel - '0' : pixel - 'A' + 10;
            dst.setPixel(x, y, qRgb(newPixel, newPixel, newPixel));
        }
    }

    return dst;
}

QVector<uchar> QImage_to_QVector(const QImage& src) {
    QVector<uchar> dst;
    const int h = src.height(), w = src.width();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            dst.push_back(qGray(src.pixel(x, y)));
        }
    }
    return dst;
}

QVector<int> calc_hist(const QVector<uchar>& src) {
    const int level = *std::max_element(src.begin(), src.end()) + 1;
    QVector<int> histogram(level);
    std::for_each(src.begin(),
                  src.end(),
                  [&](const int& pixel) { histogram[pixel] += 1; });
    return histogram;
}

QChartView *create_bar_chart(const QVector<int>& histogram) {
    QBarSet *set = new QBarSet("Histogram");
    std::for_each(histogram.begin(),
                  histogram.end(),
                  [&](const int& count) { set->append(count); });
    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

QImage QVector_to_QImage(const QVector<uchar>& src, const int& width, const int& height) {
    QImage dst(width, height, QImage::Format_RGB32);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const uchar pixel = src.at(y * height + x);
            dst.setPixel(x, y, qRgb(pixel, pixel, pixel));
        }
    }
    return dst;
}

/*add and sub operation*/
QVector<uchar> add_or_sub(const QVector<uchar>& src, const int& value) {
    QVector<uchar> dst;
    std::transform(src.begin(),
                   src.end(),
                   std::back_inserter(dst),
                   [&](const int& pixel) { return pixel + value > 255 ? \
                                                       255 : pixel + value < 0 ? 0 : pixel + value; });
    return dst;
}

/*multiplication operation*/
QVector<uchar> mul(const QVector<uchar>& src, const int& value) {
    QVector<uchar> dst;
    std::transform(src.begin(),
                   src.end(),
                   std::back_inserter(dst),
                   [&](const int& pixel) { return pixel * value > 255 ? \
                                                        255 : pixel * value < 1 ? 1 : pixel * value; });
    return dst;
}
