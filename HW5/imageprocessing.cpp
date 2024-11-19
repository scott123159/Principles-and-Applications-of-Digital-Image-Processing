#include "imageprocessing.h"
#include "ui_imageprocessing.h"
#include <QFileDialog>
#include <QButtonGroup>

float f(float n)                                                     //RGB轉換L*a*b*時所用的f(x)
{
    float value;
    if (n > 0.008856) {
        value = pow (n, 1 / 3.0);
        return value;
    }
    else {
        value = 7.787 * n + (16 / 116.0);
        return value;
    }
}

QImage kmeans(const QImage& src, int k, int iter=10)                              //K-means 演算法
{
    QImage dst(src.width(), src.height(), src.format());                            //複製影像
    QVector<Point> pixels, centroids;
    for(int y = 0; y < src.height(); y++) for(int x = 0; x < src.width(); x++)
    {
        QColor color = src.pixelColor(x, y);
        pixels << Point(color.red(), color.green(), color.blue());                      //用一維vector儲存
    }
    srand(time(nullptr));                                                               //隨機數種子
    for(int i = 0; i < k; i++) centroids << pixels[rand() % (src.height() * src.width())];  //隨機挑選質心
    for(int i = 0; i < iter; i++)                                                       //幾次迭代
    {
        for(QVector<Point>::iterator c = centroids.begin(); c != centroids.end(); c++)  //開始吸收派系
        {
            int cluster_id = c - centroids.begin();                                     //計算簇
            for(QVector<Point>::iterator it = pixels.begin(); it != pixels.end(); it++)
            {
                int c_dist = it->dist(*c);
                if(c_dist < it->min_dist)                                               //以距離作為度量衡
                {
                    it->min_dist = c_dist;                                              //更新最小距離
                    it->cluster = cluster_id;                                           //更新簇
                }
            }
        }
        QVector<int> num;                                                               //找新的質心用
        QVector<double> sum_R, sum_G, sum_B;                                            //找新的質心用
        for(int i = 0; i < k; i++)
        {
            num.push_back(0);
            sum_R.push_back(.0);
            sum_G.push_back(.0);
            sum_B.push_back(.0);
        }
        for(QVector<Point>::iterator it = pixels.begin(); it != pixels.end(); it++)
        {
            int cluster_id = it->cluster;
            num[cluster_id] += 1;
            sum_R[cluster_id] += it->R;
            sum_G[cluster_id] += it->G;
            sum_B[cluster_id] += it->B;
            it->min_dist = INT_MAX;
        }
        for(QVector<Point>::iterator c = centroids.begin(); c != centroids.end(); c++)  //更新質心
        {                                                                               //用上個迴圈所疊代的結果再以平均法來求出新的質心
            int cluster_id = c - centroids.begin();
            c->R = sum_R[cluster_id] / num[cluster_id];
            c->G = sum_G[cluster_id] / num[cluster_id];
            c->B = sum_B[cluster_id] / num[cluster_id];
        }
    }
    for(int y = 0; y < src.height(); y++) for(int x = 0; x < src.width(); x++)          //將最後分群的結果寫入影像中
    {
        int cluster_id = pixels[y * src.width() + x].cluster;
        QColor color(centroids[cluster_id].R, centroids[cluster_id].G, centroids[cluster_id].B);
        dst.setPixelColor(x, y, color);
    }
    return dst;
}

QImage RGB2GRAY(const QImage& src) {                                 //將全彩影像轉灰階影像
    QImage dst(src.width(), src.height(), QImage::Format_Grayscale8);
    QColor colour;
    int R, G, B, gray;
    for(int y = 0; y < src.height(); y++) {
        for(int x = 0; x < src.width(); x++) {
            colour = src.pixelColor(x, y);
            R = colour.red();
            G = colour.green();
            B = colour.blue();
            gray = 0.299 * R + 0.587 * G + 0.114 * B;                //使用心理學上的轉換方式
            dst.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    return dst;
}

QImage RGB2CMY(const QImage& src) {                                  //將RGB影像轉換成CMY影像
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor color;
    int C, M, Y;
    for(int y = 0; y < src.height(); y++) {
        for(int x = 0; x < src.width(); x++) {
            color = src.pixelColor(x, y);
            C = 255 - color.red();                                  //取紅補色
            M = 255 - color.green();                                //取綠補色
            Y = 255 - color.blue();                                 //取藍補色
            dst.setPixelColor(x, y, QColor(C, M, Y));
        }
    }
    return dst;                                                     //輸出影像
}

QImage RGB2HSI(const QImage& src) {                                  //將RGB影像轉換成HSI影像
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor color;
    float R, G, B, H, S, I, num, den, theta, min_RGB, PI = acos(-1);
    int new_H, new_S, new_I;
    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {
            color = src.pixelColor(x, y);
            R = color.red() / 255.0;                                //將紅色正規化至[0, 1]
            G = color.green() / 255.0;                              //將綠色正規化至[0, 1]
            B = color.blue() / 255.0;                               //將藍色正規化至[0, 1]
            I = (R + G + B) / 3;
            num = 0.5 * ((R - G) + (R - B));
            den = sqrt((R - G) * (R - G) + (R - B) * (G - B));
            if(den == 0) {
                H = 0;
            } else {
                theta = acos(num / den);
                H = B <= G ? theta : 2 * PI - theta;
            }
            H = H / (2 * PI);
            min_RGB = std::min(R, std::min(G, B));
            S = (R + G + B) > 0 ? 1 - (3 * min_RGB / (R + G + B)) : 0;
            new_H = qBound(0, int(H * 255), 255);
            new_S = qBound(0, int(S * 255), 255);
            new_I = qBound(0, int(I * 255), 255);
            dst.setPixelColor(x, y, QColor(new_H, new_S, new_I));
        }
    }
    return dst;
}

QImage RGB2XYZ(const QImage& src) {                                  //將RGB轉換成XYZ
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor color;
    float R, G, B, X, Y, Z, new_X, new_Y, new_Z;
    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {
            color = src.pixelColor(x, y);
            R = color.red() / 255.0;                                //將red正規化
            G = color.green() / 255.0;                              //將green正規化
            B = color.blue() / 255.0;                               //將blue正規化
            //XYZ轉換的係數矩陣
            // __                          __    _   _
            //|  0.412453 0.357580 0.184023  |  |  R  |
            //|                              |  |     |
            //|  0.212671 0.715160 0.072169  |  |  G  |
            //|                              |  |     |
            //|  0.019334 0.119193 0.959227  |  |  B  |
            // --                          --    -   -
            X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
            Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
            Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
            new_X = qBound(0, int(X * 255), 255);
            new_Y = qBound(0, int(Y * 255), 255);
            new_Z = qBound(0, int(Z * 255), 255);
            dst.setPixelColor(x, y, QColor(new_X, new_Y, new_Z));
        }
    }
    return dst;
}

QImage RGB2Lab(const QImage& src) {                                  //將RGB轉換成L*a*b*
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor color;
    float R, G, B, X, Y, Z, L, a, b;
    int new_L, new_a, new_b;
    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {
            color = src.pixelColor(x, y);
            R = color.red() / 255.0;                                //將red正規化
            G = color.green() / 255.0;                              //將green正規化
            B = color.blue() / 255.0;                               //將blue正規化
            X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
            Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
            Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
            L = Y > 0.008856 ? 116 * pow(Y, 1 / 3.0) - 16 : 903.3 * Y;
            a = 500 * (f(X / 0.950467) - f(Y));
            b = 200 * (f(Y) - f(Z / 1.088969));
            new_L = qBound(0, int(L * 255 / 100), 255);
            new_a = qBound(0, int(a + 128), 255);
            new_b = qBound(0, int(b + 128), 255);
            dst.setPixelColor(x, y, QColor(new_L, new_a, new_b));
        }
    }
    return dst;
}

QImage RGB2YUV(const QImage& src) {                                  //將RGB轉換成L*a*b*
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor color;
    float R, G, B, Y, U, V;
    int new_Y, new_U, new_V;
    for (int y = 0; y < src.height(); y++)
    {
        for (int x = 0; x < src.width(); x++)
        {
            color = src.pixelColor(x, y);
            R = color.red();
            G = color.green();
            B = color.blue();
            //YUV轉換的係數矩陣
            // __                   __    _   _
            //|  0.257  0.504  0.098  |  |  R  |
            //|                       |  |     |
            //| -0.148 -0.291  0.439  |  |  G  |
            //|                       |  |     |
            //|  0.439 -0.368 -0.071  |  |  B  |
            // --                   --    -   -
            Y = 0.257 * R + 0.504 * G + 0.098 * B + 16;
            U = -0.148 * R + -0.291 * G + 0.439 * B + 128;
            V = 0.439 * R + -0.368 * G + -0.071 * B + 128;
            new_Y = qBound(0, (int)Y, 255);
            new_U = qBound(0, (int)U, 255);
            new_V = qBound(0, (int)V, 255);
            dst.setPixelColor(x, y, QColor(new_Y, new_U, new_V));
        }
    }
    return dst;
}

ImageProcessing::ImageProcessing(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageProcessing)
{
    ui->setupUi(this);
    QButtonGroup* color_space = new QButtonGroup(this);
    QButtonGroup* pseudo_color = new QButtonGroup(this);
    QButtonGroup* kmeans_buttons = new QButtonGroup(this);
    color_space->addButton(ui->cmy_rbutton, 0);
    color_space->addButton(ui->hsi_rbutton, 1);
    color_space->addButton(ui->xyz_rbutton, 2);
    color_space->addButton(ui->CIELab_rbutton, 3);
    color_space->addButton(ui->yuv_rbutton, 4);
    pseudo_color->addButton(ui->winter_rbutton, 0);
    pseudo_color->addButton(ui->jet_rbutton, 1);
    pseudo_color->addButton(ui->random_rbutton, 2);
    pseudo_color->addButton(ui->autumn_rbutton, 3);
    pseudo_color->addButton(ui->hot_rbutton, 4);
    kmeans_buttons->addButton(ui->kmeansrgb_rbutton, 0);
    kmeans_buttons->addButton(ui->kmeanshsi_rbutton, 1);
    kmeans_buttons->addButton(ui->kmeansLab_rbutton, 2);
    ui->kmeans_slider->setMinimum(2);
    ui->kmeans_slider->setMaximum(20);
    connect(ui->kmeans_slider, &QSlider::valueChanged, this, [=]
    {
        if(image.isNull()) return;
        ui->kmeans_label->setText("K : " + QString::number(ui->kmeans_slider->value()));
        int k = ui->kmeans_slider->value();
        QImage processed_image;
        switch(kmeans_buttons->checkedId())
        {
        case 0:
            processed_image = QImage(image);
            break;
        case 1:
            processed_image = RGB2HSI(image);
            break;
        case 2:
            processed_image = RGB2Lab(image);
            break;
        }
        ui->processed_image->setPixmap(QPixmap::fromImage(kmeans(processed_image, k, 10)));
    });
    connect(ui->load_image, &QAction::triggered, this, [=] {
        QString file_name = QFileDialog::getOpenFileName(this,
                                                         "Load Image",
                                                         QDir::currentPath(),
                                                         "Image(*.bmp *.png *.jpg);;All(*.*)");
        if(file_name.isEmpty()) return;
        image = QImage(file_name);
        ui->original_image->setPixmap(QPixmap::fromImage(image));
    });
    connect(ui->apply_button, &QPushButton::clicked, this, [=] {
        if(image.isNull()) return;
        switch(color_space->checkedId()) {
        case 0:
            ui->processed_image->setPixmap(QPixmap::fromImage(RGB2CMY(image)));
            break;
        case 1:
            ui->processed_image->setPixmap(QPixmap::fromImage(RGB2HSI(image)));
            break;
        case 2:
            ui->processed_image->setPixmap(QPixmap::fromImage(RGB2XYZ(image)));
            break;
        case 3:
            ui->processed_image->setPixmap(QPixmap::fromImage(RGB2Lab(image)));
            break;
        case 4:
            ui->processed_image->setPixmap(QPixmap::fromImage(RGB2YUV(image)));
            break;
        }
        QImage grayscale_image = RGB2GRAY((image));                                     //轉灰階影像
        QImage pseudo_color_image(image.width(), image.height(), QImage::Format_RGB888);//偽色彩影像
        ui->grayscale_image->setPixmap(QPixmap::fromImage(grayscale_image));
        QImage color_map(256, 1, QImage::Format_RGB888);                                //256色的colormap
        for(int x = 0; x < 256; x++) color_map.setPixelColor(x, 0, QColor(x, x, x));
        ui->gray_colortable->setPixmap(QPixmap::fromImage(color_map));
        switch(pseudo_color->checkedId()) {                                             //偽色彩選擇用的按鈕集合
        case 0:                                                                         //冬天colormap
            for(int x = 0; x < 256; x++) color_map.setPixelColor(x, 0, QColor(0, x, 255));
            for(int y = 0; y < image.height(); y++) {
                for(int x = 0; x < image.width(); x++) {
                    QColor gray_color = grayscale_image.pixelColor(x, y);
                    QColor psreudo_color = color_map.pixelColor(gray_color.red(), 0);
                    pseudo_color_image.setPixelColor(x, y, psreudo_color);
                }
            }
            ui->pseudo_colortable->setPixmap(QPixmap::fromImage(color_map));
            ui->pseudo_color_image->setPixmap(QPixmap::fromImage(pseudo_color_image));
            break;
        case 1:                                                                         //Jet colormap
            for(int x = 0; x < 32; x++) color_map.setPixelColor(x, 0, QColor(0, 0, 128 + 4 * x));
            color_map.setPixelColor(32, 0, QColor(0, 0, 255));
            for(int x = 0; x < 63; x++) color_map.setPixelColor(33 + x, 0, QColor(0, 4 + 4 * x, 255));
            color_map.setPixelColor(96, 0, QColor(2, 255, 254));
            for(int x = 0; x < 62; x++) color_map.setPixelColor(97 + x, 0, QColor(6 + 4 * x, 255, 250 - 4 * x));
            color_map.setPixelColor(159, 0, QColor(254, 255, 1));
            for(int x = 0; x < 64; x++) color_map.setPixelColor(160 + x, 0, QColor(255, 252 - (4 * x), 0));
            for(int x = 0; x < 32; x++) color_map.setPixelColor(224 + x, 0, QColor(252 - (4 * x), 0, 0));
            for(int y = 0; y < image.height(); y++) {
                for(int x = 0; x < image.width(); x++) {
                    QColor gray_color = grayscale_image.pixelColor(x, y);
                    QColor psreudo_color = color_map.pixelColor(gray_color.red(), 0);
                    pseudo_color_image.setPixelColor(x, y, psreudo_color);
                }
            }
            ui->pseudo_colortable->setPixmap(QPixmap::fromImage(color_map));
            ui->pseudo_color_image->setPixmap(QPixmap::fromImage(pseudo_color_image));
            break;
        case 2:{                                                                        //隨機顏色colormap
            int n = ui->random_edit->text().toInt();
            if(n <= 0) return;
            int gap;
            srand(time(nullptr));
            color_map = QImage(n, 1, QImage::Format_RGB888);
            gap = 256 / n;
            for(int x = 0; x < n; x++) color_map.setPixelColor(x, 0, QColor(rand() % 256, rand() % 256, rand() % 256));
            for(int y = 0; y < image.height(); y++) {
                for(int x = 0; x < image.width(); x++) {
                    QColor gray_color = grayscale_image.pixelColor(x, y);
                    int choose = gray_color.red() / gap >= n ? n - 1 : gray_color.red() / gap;
                    QColor psreudo_color = color_map.pixelColor(choose, 0);
                    pseudo_color_image.setPixelColor(x, y, psreudo_color);
                }
            }
            ui->pseudo_colortable->setPixmap(QPixmap::fromImage(color_map));
            ui->pseudo_color_image->setPixmap(QPixmap::fromImage(pseudo_color_image));

        }
            break;
        case 3:                                                                         //秋天colormap
            for(int x = 0; x < 256; x++) color_map.setPixelColor(x, 0, QColor(255, x, 0));
            for(int y = 0; y < image.height(); y++) {
                for(int x = 0; x < image.width(); x++) {
                    QColor gray_color = grayscale_image.pixelColor(x, y);
                    QColor psreudo_color = color_map.pixelColor(gray_color.red(), 0);
                    pseudo_color_image.setPixelColor(x, y, psreudo_color);
                }
            }
            ui->pseudo_colortable->setPixmap(QPixmap::fromImage(color_map));
            ui->pseudo_color_image->setPixmap(QPixmap::fromImage(pseudo_color_image));
            break;
        case 4:                                                                         //產生很熱的感覺的色譜
            for(int x = 0; x <= 85; x++) color_map.setPixelColor(x, 0, QColor(x * 3, 0, 0));
            for(int x = 86; x <=170; x++) color_map.setPixelColor(x, 0, QColor(255, (x - 86) * 3, 0));
            for(int x = 171; x <= 255; x++) color_map.setPixelColor(x, 0, QColor(255, 255, (x - 171) * 3));
            for(int y = 0; y < image.height(); y++) {
                for(int x = 0; x < image.width(); x++) {
                    QColor gray_color = grayscale_image.pixelColor(x, y);
                    QColor psreudo_color = color_map.pixelColor(gray_color.red(), 0);
                    pseudo_color_image.setPixelColor(x, y, psreudo_color);
                }
            }
            ui->pseudo_colortable->setPixmap(QPixmap::fromImage(color_map));
            ui->pseudo_color_image->setPixmap(QPixmap::fromImage(pseudo_color_image));
            break;
        }
    });
    connect(ui->exit, &QAction::triggered, this, [=] {
        this->close();
    });
}

ImageProcessing::~ImageProcessing()
{
    delete ui;
}

