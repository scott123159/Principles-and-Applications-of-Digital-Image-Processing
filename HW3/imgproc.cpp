#include "imgproc.h"

QVector<QVector<double>> cv::getGaussianKernel(const double sigma, const int kernelSize)
{
    const int halfSize = kernelSize / 2;
    double sum = 0.;
    QVector<QVector<double>> kernel(kernelSize, QVector<double>(kernelSize));

    for (int x = -halfSize; x <= halfSize; ++x) {
        for (int y = -halfSize; y <= halfSize; ++y) {
            double exponent = -(x * x + y * y) / (2 * sigma * sigma);
            kernel[x + halfSize][y + halfSize] = std::exp(exponent) / (2 * M_PI * sigma * sigma);
            sum += kernel[x + halfSize][y + halfSize];
        }
    }

    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}
