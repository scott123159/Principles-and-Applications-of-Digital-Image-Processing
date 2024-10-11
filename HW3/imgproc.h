#ifndef IMGPROC_H
#define IMGPROC_H

#include <QVector>
#include <QImage>

/*定義新的核data type*/
typedef QVector<QVector<double>> Kernel;

namespace cv {

/*Gaussian kernel*/
Kernel getGaussianKernel(const double sigma, const int kernelSize);

/*Convolution operation*/
QImage convolution(const QImage& src, const Kernel& kernel);

/*翻轉kernel*/
Kernel flipKernel(const Kernel& kernel);
}

#endif // IMGPROC_H
