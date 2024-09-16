#ifndef UTILITIES_H
#define UTILITIES_H

#include <QImage>
#include <QFileDialog>
#include <QApplication>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>

/*Open the file then save it as QImage*/
QImage file_to_QImage();

/*Convert QImage to 1-D QVector */
QVector<uchar> QImage_to_QVector(const QImage& src);

/*Convert QVector to QImage*/
QImage QVector_to_QImage(const QVector<uchar>& src, const int& width, const int& height);

/*Calculate histogram*/
QVector<int> calc_hist(const QVector<uchar>& src);

/*Show QChart*/
QChartView *create_bar_chart(const QVector<int>& histogram);

/*The algorithm that add/sub value on image */
QVector<uchar> add_or_sub(const QVector<uchar>& src, const int& value);

/*The algorithm that mul value on image */
QVector<uchar> mul(const QVector<uchar>& src, const int& value);

#endif // UTILITIES_H
