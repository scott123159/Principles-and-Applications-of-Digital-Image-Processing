#ifndef IMGPROC_H
#define IMGPROC_H

#include <QVector>
#include <QImage>

namespace cv {
QVector<QVector<double>> getGaussianKernel(const double sigma, const int kernelSize);
}

#endif // IMGPROC_H
