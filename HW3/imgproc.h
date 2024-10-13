#ifndef IMGPROC_H
#define IMGPROC_H

#include <QVector>
#include <QImage>
#include <QtGlobal>

/*定義新的核data type*/
typedef QVector<QVector<double>> Kernel;

namespace cv {

/*Gaussian kernel*/
Kernel getGaussianKernel(const double sigma, const int kernelSize);

/*Convolution operation*/
QImage convolution(const QImage& src, const Kernel& kernel);

/*翻轉kernel*/
Kernel flipKernel(const Kernel& kernel);

QImage applyMedianBlur(const QImage& src, const int kernelSize);
}

#endif // IMGPROC_H
