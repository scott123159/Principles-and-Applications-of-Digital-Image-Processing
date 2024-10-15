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

QImage cv::applyMarrHildreth(const QImage &src, const int size, const double threshold)
{
    QImage dst(src);

    /*Calculate sigma*/
    const double sigma = size / 6.0;

    /*Get the LoG kernel*/
    const Kernel kernel = cv::getLaplacianOfGaussianKernel(sigma, size);

    QVector<QVector<double>> values(src.height(), QVector<double>(src.width(), 0));

    /*Convolution for numeric image vector*/
    const int dx = kernel[0].size() / 2;
    const int dy = kernel.size() / 2;

    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {
            dst.setPixelColor(x, y, QColor(0, 0, 0));
            double gray = 0;
            for (int z = 0; z < kernel.size(); z++)
            {
                for (int w = 0; w < kernel[0].size(); w++)
                {
                    const int newX = x - dx + w;
                    const int newY = y - dy + z;

                    if (newX >= 0 && newX < src.width() && newY >= 0 && newY < src.height()) {
                        const QColor color = src.pixelColor(newX, newY);
                        const double coff = kernel[z][w];

                        /*Use grayscale color*/
                        gray += qGray(color.red(), color.green(), color.blue()) * coff;
                    }
                }
            }

            /*Store the value of convolution of LoG*/
            values[y][x] = gray;
        }
    }

    for (int y = 1; y < values.size() - 1; y++)
        for (int x = 1; x < values[0].size() - 1; x++) {

            /*Current value*/
            const double center = values[y][x];

            /*Scan the eight neighboring points around the pixel*/
            for (int z = -1; z <= 1; z++)
                for (int w = -1; w <= 1; w++) {
                    if (z == 0 && w == 0)
                        continue;

                    const double neighbor = values[y + z][x + w];

                    if ((center > 0 && neighbor < 0) || (center < 0 && neighbor > 0))

                        /*If the |P(x, y)| is greater than threshold then mark it as edge*/
                        if (qAbs(center) > threshold)
                            dst.setPixelColor(x, y, QColor(255, 255, 255));
                }
        }
    return dst;
}

QImage cv::applyLocalHistogramEnhancement(const QImage &image, const int size, const double k0, const double k1, const double k2, const double k3, const double c)
{
    QImage gray_image(image);
    QImage he_image = QImage(image);
    QImage lhe_image = QImage(image);
    QImage contrast_image = QImage(image);
    double global_avg = 0.0;//全域平均
    double global_dev = 0.0;//全域標準差
    for(int i = 0; i < image.height(); i++) {
        for(int j = 0; j < image.width(); j++) {
            int r = (qRed(image.pixel(j, i)) + qGreen(image.pixel(j, i)) + qBlue(image.pixel(j, i))) / 3;
            int g = r, b = r;
            gray_image.setPixel(j, i, qRgb(r, g, b));
        }
    }
    int nk[256] = {};                                           //計算出數量
    for(int i = 0; i < image.height(); i++) {
        for(int j = 0; j < image.width(); j++) {
            nk[qRed(gray_image.pixel(j, i))] += 1;
        }
    }
    double pdf[256] = {};                                       //計算出機率密度函數P.D.F
    for(int i = 0; i < 256; i++) pdf[i] = (nk[i] * 1.0) / (image.height() * image.width());
    double cdf[256] = {pdf[0]};
    for(int i = 1; i < 256; i++) cdf[i] = cdf[i - 1] + pdf[i];  //計算出累計密度函數C.D.F
    int histogramEqualization[256] = {};                        //計算出灰階值方圖平均所對應的灰階值
    for(int i = 0; i < 256; i++)histogramEqualization[i] = cdf[i] * 255;
    for(int i = 0; i < 256; i++) global_avg += (i * ((nk[i] * 1.0) / (image.height() * image.width())));
    for(int i = 0; i < 256; i++) global_dev += ((i - global_avg) * ((i - global_avg))) * ((nk[i] * 1.0) / (image.height() * image.width()));
    global_dev = sqrt(global_dev);
    //計算區域平均以及區域標準差
    for(int i = 0; i < image.height(); i++) {
        for(int j = 0; j < image.width(); j++) {
            double local_avg = 0.0;
            double local_dev = 0.0;
            int lnk[256] = {};
            for(int y = 0; y < size; y++) {
                for(int x = 0; x < size; x++) {
                    int di = i + y - (size / 2);
                    int dj = j + x - (size / 2);
                    if (di >= 0 && di < image.height() && dj >= 0 && dj < image.width()) {
                        lnk[qRed(gray_image.pixel(dj, di))]++;
                    }
                }
            }
            //計算區域平均以及區域標準差
            for(int i = 0; i < 256; i++) local_avg += (i * ((lnk[i] * 1.0) / (size * size)));
            //計算區域平均以及區域標準差
            for(int i = 0; i < 256; i++) local_dev += ((i - local_avg) * (i - local_avg)) * (lnk[i] * 1.0 / size * size);
            local_dev = sqrt(local_dev);
            bool condition1 = k0 * global_avg <= local_avg && local_avg <= k1 * global_avg;
            bool condition2 = k2 * global_dev <= local_dev && local_dev <= k3 * global_dev;
            int r1 = qRed(gray_image.pixel(j, i));
            int r2 = histogramEqualization[r1];
            he_image.setPixel(j, i, qRgb(r2, r2, r2));
            if(condition1 && condition2) {
                r1 = r1 * c >= 255 ? 255 : (r1 * c);
                lhe_image.setPixel(j, i, qRgb(r1, r1, r1));
            } else {
                lhe_image.setPixel(j, i, qRgb(r1, r1, r1));
            }
            //unsigned int gray = histogramEqualization[qRed(gray_image.pixel(j, i))];

        }
    }
    return lhe_image;
}
