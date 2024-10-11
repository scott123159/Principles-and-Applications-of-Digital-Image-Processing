#include "imgproc.h"

Kernel cv::getGaussianKernel(const double sigma, const int kernelSize)
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

Kernel cv::flipKernel(const Kernel &kernel)
{
    Kernel flippedKernel = kernel;

    /*翻轉上下*/
    std::reverse(flippedKernel.begin(), flippedKernel.end());

    /*翻轉左右*/
    for (auto& row : flippedKernel)
        std::reverse(row.begin(), row.end());

    return flippedKernel;
}

QImage cv::convolution(const QImage &src, const Kernel &kernel)
{

    /*Copy from original image with copy constructor*/
    QImage dst(src);

    /*Calculate x and y offsets of kernel*/
    const int dx = kernel[0].size() / 2;
    const int dy = kernel.size() / 2;

    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {

            /*The values we need to accumulate*/
            double r = 0, g = 0, b = 0;
            for (int z = 0; z < kernel.size(); z++)
            {
                for (int w = 0; w < kernel[0].size(); w++)
                {
                    const int newX = x - dx + w;
                    const int newY = y - dy + z;

                    if (newX >= 0 && newX < src.width() && newY >= 0 && newY < src.height()) {
                        const QColor color = src.pixelColor(newX, newY);
                        const double coff = kernel[z][w];
                        r += color.red() * coff;
                        g += color.green() * coff;
                        b += color.blue() * coff;
                    }
                }
            }
            dst.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return dst;
}
