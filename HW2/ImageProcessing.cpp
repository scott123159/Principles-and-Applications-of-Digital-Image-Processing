#include "ImageProcessing.h"

QImage cv::cvtColor(const QImage& src, const int& code) {
    /*Create new image from copy constructor*/
    QImage dst(src);

    for (int y = 0; y < dst.height(); y++) {
        for (int x = 0; x < dst.width(); x++) {

            /*Get the RGB three colors and set them const value*/
            const int red = qRed(src.pixel(x, y));
            const int green = qGreen(src.pixel(x, y));
            const int blue = qBlue(src.pixel(x, y));

            /*Use switch case statement to determine which color mode shall we use*/
            switch (code) {
                case COLOR_RGB2GRAY1: {
                    /*Use "GRAY = (R + G + B) / 3.0" method to transform image into grayscale image*/
                    const int color = (red + green + blue) / 3.0;
                    dst.setPixelColor(x, y, QColor(color, color, color));
                } break;

                case COLOR_RGB2GRAY2: {
                    /*Use "GRAY = 0.299 * R + 0.587 * G + 0.114 * B" method to transform image into grayscale image*/
                    const int color = (0.299 * red + 0.587 * green + 0.114 * blue);
                    dst.setPixelColor(x, y, QColor(color, color, color));
                } break;
            }
        }
    }
    return dst;
}

QImage cv::diff(const QImage& src1, const QImage& src2) {
    /*Create new image from copy constructor*/
    QImage dst(src1);

    for (int y = 0; y < dst.height(); y++) {
        for (int x = 0; x < dst.width(); x++) {

            /*Use the absolute value of two images to determine the new pixel value*/
            const int color = abs(qRed(src1.pixel(x, y)) - qRed(src2.pixel(x, y)));
            dst.setPixelColor(x, y, QColor(color, color, color));
        }
    }
    return dst;
}

QVector<int> cv::calcHist(const QImage& src) {
    /*For calculating the histogram*/
    QVector<int> hist(256);

    for (int y = 0; y < src.height(); y++) {
        for (int x = 0; x < src.width(); x++) {
            hist[qGray(src.pixel(x, y))] += 1;
        }
    }

    return hist;
}

QImage cv::threshold(const QImage &src, const int &threshold)
{
    QImage dst(src);

    /*Use threshold to determine the binary image*/
    for (int y = 0; y < src.height(); y++) {
        for (int x = 0; x < src.width(); x++) {

            /*If current grayscale level is greater than threshold*/
            /*then return 255 otherwise return 0*/
            const int grayColor = qGray(src.pixel(x, y));
            dst.setPixelColor(x, y, grayColor > threshold ? QColor(255, 255, 255) : QColor(0, 0, 0));
        }
    }

    return dst;
}

QImage cv::nearestNeighborInterpolation(const QImage& src, const double& ratio)
{
    QImage dst(src.width() * ratio, src.height() * ratio, src.format());

    /*Nearest neightbor interpolation algorithm*/
    for (int y = 0; y < dst.height(); y++) {
        for (int x = 0; x < dst.width(); x++) {
            const int newX = x / ratio;
            const int newY = y / ratio;
            const QColor color = src.pixelColor(newX, newY);
            dst.setPixelColor(x, y, color);
        }
    }

    return dst;
}
