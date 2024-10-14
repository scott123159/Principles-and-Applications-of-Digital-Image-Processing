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
            r = qBound<int>(0, r, 255);
            g = qBound<int>(0, g, 255);
            b = qBound<int>(0, b, 255);
            dst.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    return dst;
}

QImage cv::applyMedianBlur(const QImage &image, const int kernelSize)
{
    QImage result(image.size(), image.format());

    // Calculate the half of the kernel size
    int halfKernel = kernelSize / 2;

    // Loop through each pixel of the image
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            std::vector<int> reds, greens, blues;

            // Loop through the kernel (neighborhood)
            for (int ky = -halfKernel; ky <= halfKernel; ++ky) {
                for (int kx = -halfKernel; kx <= halfKernel; ++kx) {
                    int neighborX = x + kx;
                    int neighborY = y + ky;

                    // Ensure the pixel is within image bounds
                    if (neighborX >= 0 && neighborX < image.width() && neighborY >= 0 && neighborY < image.height()) {
                        QColor pixelColor = image.pixelColor(neighborX, neighborY);
                        reds.push_back(pixelColor.red());
                        greens.push_back(pixelColor.green());
                        blues.push_back(pixelColor.blue());
                    }
                }
            }

            // Sort the pixel values to find the median
            std::sort(reds.begin(), reds.end());
            std::sort(greens.begin(), greens.end());
            std::sort(blues.begin(), blues.end());

            // Get the median value (the middle value)
            int medianRed = reds[reds.size() / 2];
            int medianGreen = greens[greens.size() / 2];
            int medianBlue = blues[blues.size() / 2];

            // Set the new pixel value in the result image
            result.setPixelColor(x, y, QColor(medianRed, medianGreen, medianBlue));
        }
    }

    return result;
}

QImage cv::applySobel(const QImage &src)
{
    QImage dst(src);
    int rx, ry, gx, gy, bx, by;

    /*Use Sobel kernel such that [[-1, 0, 1],
                                  [-2, 0, 2],
                                  [-1, 0, 1]] for horizontal direction*/

    /*Use Sobel kernel such that [[-1, -2, -1],
                                  [ 0,  0,  0],
                                  [ 1,  2,  1]] for vertical direction*/
    for(int i = 1; i < src.height() - 1; i++) {
        for(int j = 1; j < src.width() -1; j++) {
            rx = qRed(src.pixel(j - 1, i - 1))
            + qRed(src.pixel(j - 1, i)) * 2
                + qRed(src.pixel(j - 1, i + 1))
                + qRed(src.pixel(j + 1, i - 1)) * (-1)
                + qRed(src.pixel(j + 1, i)) * (-2)
                + qRed(src.pixel(j + 1, i + 1)) * (-1);
            ry = qRed(src.pixel(j - 1, i - 1))
                 + qRed(src.pixel(j, i - 1)) * 2
                 + qRed(src.pixel(j + 1, i - 1))
                 + qRed(src.pixel(j - 1, i + 1)) * (-1)
                 + qRed(src.pixel(j, i + 1)) * (-2)
                 + qRed(src.pixel(j + 1, i + 1)) * (-1);
            int r = fmin(255, sqrt(rx * rx + ry * ry));

            gx = qGreen(src.pixel(j - 1, i - 1))
                 + qGreen(src.pixel(j - 1, i)) * 2
                 + qGreen(src.pixel(j - 1, i + 1))
                 + qGreen(src.pixel(j + 1, i - 1)) * (-1)
                 + qGreen(src.pixel(j + 1, i)) * (-2)
                 + qGreen(src.pixel(j + 1, i + 1)) * (-1);
            gy = qGreen(src.pixel(j - 1, i - 1))
                 + qGreen(src.pixel(j, i - 1)) * 2
                 + qGreen(src.pixel(j + 1, i - 1))
                 + qGreen(src.pixel(j - 1, i + 1)) * (-1)
                 + qGreen(src.pixel(j, i + 1)) * (-2)
                 + qGreen(src.pixel(j + 1, i + 1)) * (-1);
            int g = fmin(255, sqrt(gx * gx + gy * gy));

            bx = qBlue(src.pixel(j - 1, i - 1))
                 + qBlue(src.pixel(j - 1, i)) * 2
                 + qBlue(src.pixel(j - 1, i + 1))
                 + qBlue(src.pixel(j + 1, i - 1)) * (-1)
                 + qBlue(src.pixel(j + 1, i)) * (-2)
                 + qBlue(src.pixel(j + 1, i + 1)) * (-1);
            by = qBlue(src.pixel(j - 1, i - 1))
                 + qBlue(src.pixel(j, i - 1)) * 2
                 + qBlue(src.pixel(j + 1, i - 1))
                 + qBlue(src.pixel(j - 1, i + 1)) * (-1)
                 + qBlue(src.pixel(j, i + 1)) * (-2)
                 + qBlue(src.pixel(j + 1, i + 1)) * (-1);
            int b = fmin(255, sqrt(bx * bx + by * by));

            dst.setPixel(j, i, qRgb(r, g, b));
        }
    }
    return dst;
}

Kernel cv::getLaplacianOfGaussianKernel(const double sigma, const int size)
{
    /*Initialize kernel to an empty kernel*/
    Kernel kernel(size, QVector<double>(size, 0));

    /*For half size of kernel size*/
    const int halfSize = size / 2;

    /*σ ^ 2 and σ ^ 4*/
    double sigma2 = sigma * sigma;
    double sigma4 = sigma2 * sigma2;
    double normalization = -1.0 / (M_PI * sigma4);

    for (int y = -halfSize; y <= halfSize; ++y) {
        for (int x = -halfSize; x <= halfSize; ++x) {
            double r2 = x * x + y * y;
            double value = normalization * (1 - r2 / (2 * sigma2)) * exp(-r2 / (2 * sigma2));
            kernel[y + halfSize][x + halfSize] = value;
        }
    }
    return kernel;
}
