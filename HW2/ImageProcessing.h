#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>

namespace cv {
    enum {
        COLOR_RGB2GRAY1,
        COLOR_RGB2GRAY2
    };
    QImage cvtColor(const QImage& src, const int& code);
    QImage diff(const QImage& src1, const QImage& src2);
    QVector<int> calcHist(const QImage& src);
};

#endif // IMAGEPROCESSING_H
