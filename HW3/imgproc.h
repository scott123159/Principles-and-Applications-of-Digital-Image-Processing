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

/*Laplacian of Gaussian kernel*/
Kernel getLaplacianOfGaussianKernel(const double sigma, const int size);

/*Flip kernel*/
Kernel flipKernel(const Kernel& kernel);

/*Use Sobel operator on image directly*/
QImage applySobel(const QImage& src);

/*Convolution operation*/
QImage convolution(const QImage& src, const Kernel& kernel);

/*Use median blur on image directly*/
QImage applyMedianBlur(const QImage& src, const int kernelSize);

QImage applyMarrHildreth(const QImage& src, double sigma, int kernelSize, double threshold);
}

#endif // IMGPROC_H
