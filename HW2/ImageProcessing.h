#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>

namespace cv {
    enum {
        COLOR_RGB2GRAY1,
        COLOR_RGB2GRAY2
    };
    QImage cvtColor(const QImage&, const int&);
    QImage diff(const QImage&, const QImage&);
    QImage threshold(const QImage&, const int&);
    QImage nearestNeighborInterpolation(const QImage&, const double&);
    QVector<int> calcHist(const QImage&);
};

#endif // IMAGEPROCESSING_H
