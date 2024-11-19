#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QMainWindow>

struct Point                                                                //k-means演算法所用的資料結構
{
    int R, G, B;                                                            //將RGB影像視為三維空間的點
    int cluster;                                                            //屬於哪一個簇，預設-1
    int min_dist;                                                           //最小距離，預設INT_MAX
    Point(int r = 0, int g = 0, int b = 0) :                                //全參數建構子
        R(r),
        G(g),
        B(b),
        cluster(-1),
        min_dist(INT_MAX) {}
    int dist(const Point& another)                                          //計算歐式距離，但不開方根便於計算
    {
        return (R - another.R) * (R - another.R) +
               (G - another.G) * (G - another.G) +
               (B - another.B) * (B - another.B);
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class ImageProcessing; }
QT_END_NAMESPACE

class ImageProcessing : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessing(QWidget *parent = nullptr);
    ~ImageProcessing();

private:
    Ui::ImageProcessing *ui;
    QImage image;
};
#endif // IMAGEPROCESSING_H
