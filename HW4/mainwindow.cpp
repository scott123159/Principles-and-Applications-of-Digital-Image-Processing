#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <cmath>
#include <complex>
#include <fftw3.h>                //使用快速傅立葉轉換函式庫
#include <opencv2/opencv.hpp>     //使用離散傅立葉轉換函式庫
#include <random>                 //常態分布函式庫

using namespace cv;               //opencv命名空間
double n = 2.25;                  //巴特沃斯的n常數，課本給2.25
int c = 5;                        //巴特沃斯的c常數，課本給5
Mat shift(Mat img) {              //類似於matlab的fftshift
  int row = img.rows / 2;
  int col = img.cols / 2;
  Mat q1(img, Rect(0, 0, col, row));
  Mat q2(img, Rect(col, 0, col, row));
  Mat q3(img, Rect(0, row, col, row));
  Mat q4(img, Rect(col, row, col, row));
  Mat tmp;
  q1.copyTo(tmp);
  q4.copyTo(q1);
  tmp.copyTo(q4);

  q3.copyTo(tmp);
  q2.copyTo(q3);
  tmp.copyTo(q2);
  return img;
}
QImage inverse(const QString& file_name) {
  return QImage(file_name);
}
QImage motion_blur(const QString& file_name, int filterSize) {
  Mat srcImg = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  int size = filterSize;
    cv::Mat filter = cv::Mat::zeros(size, size, CV_8UC1);
    cv::Mat result = cv::Mat(srcImg.size(), srcImg.type());
    for (int i = 0; i < size; i++)filter.at<uchar>(i, i) = (uchar)1;

    int len = size / 2;

    for (int r = 0; r < srcImg.rows; r++) {
        for (int c = 0; c < srcImg.cols; c++) {
            //創造kernel並做convolution
            int color = 0;
            for (int i = r - len; i <= r + len; i++) {
                for (int j = c - len; j <= c + len; j++) {
                    if (i < 0 || j < 0 || i >= srcImg.rows || j >= srcImg.cols) continue;
                    color += ((int)srcImg.at<uchar>(i, j)) * ((int)filter.at<uchar>(i - (r - len), j - (c - len)));
                }
            }

            result.at<uchar>(r, c) = (uchar)(color / size);
        }
    }

    QImage r = QImage(result.cols, result.rows, QImage::Format_Grayscale8);
    for(int i = 0; i < result.rows; i++) for(int j = 0; j < result.cols; j++) {
        int color = result.at<uchar>(i, j);
        r.setPixel(j, i, qRgb(color, color, color));
      }
    return r;


}
QImage hp_butterworth_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted;        //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F);                                 //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);                    //進行 DFT
                                  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(1));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          double D = sqrt(pow((a - half_y), 2) + pow((b - half_x), 2));
          filter.at<float>(a, b) = 1 / (1 + pow((D0 / D), (2 * n)));
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;

}

QImage lp_butterworth_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT

  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(1));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          double D = sqrt(pow((a - half_y), 2) + pow((b - half_x), 2));
          filter.at<float>(a, b) = 1 / (1 + pow((D / D0), (2 * n)));
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;

}

QImage hp_gaussian_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT

  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(1));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          double D = sqrt(pow((a - half_y), 2) + pow((b - half_x), 2));
          filter.at<float>(a, b) = 1 - exp(-pow(D, 2) / (2 * D0 * D0));
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;

}
QImage lp_gaussian_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT

  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(1));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          double D = sqrt(pow((a - half_y), 2) + pow((b - half_x), 2));
          filter.at<float>(a, b) = exp(-pow(D, 2) / (2 * D0 * D0));
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;

}

QImage hp_ideal_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT

  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(1));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          if (pow((a - half_y), 2) + pow((b - half_x), 2) <= pow(D0,2)) {
              filter.at<float>(a, b) = 0;
            }
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;
}

QImage lp_ideal_filter(const QString& file_name, double D0) {
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器
  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT

  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(0));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  //int radius = 105; //遮罩半徑
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          if (pow((a - half_y), 2) + pow((b - half_x), 2) <= pow(D0,2)) {
              filter.at<float>(a, b) = 1;
            }
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = inverseDFTconverted.at<uchar>(i, j);
      result.setPixel(j, i, qRgb(color, color, color));
    }
  return result;
}

QImage image_spectrum(const QImage& image) {
  QImage temp(image);
  int nx = image.height(), ny = image.width();
  //另外使用fftw的快速傅立葉主換
  fftw_complex* in;
  fftw_complex* out;
  fftw_plan plan_forward;
  in = (fftw_complex*)fftw_malloc ( sizeof ( fftw_complex ) * nx * ny );
  out = (fftw_complex*)fftw_malloc ( sizeof ( fftw_complex ) * nx * ny );
  plan_forward = fftw_plan_dft_2d ( nx, ny, in, out, FFTW_FORWARD,
                                    FFTW_ESTIMATE );
  for (int i = 0; i < nx; i++ )
    {
      for (int j = 0; j < ny; j++ )
      {
        in[i*ny+j][0] = qRed(image.pixel(j, i));
        in[i*ny+j][1] = 0;
      }
    }
  //執行fft
  fftw_execute(plan_forward);
  //使用作業pdf的公式 對其normalization
  int Fmin = INT_MAX, Fmax = INT_MIN, FMIN, FMAX;
  for(int i = 0; i < image.height(); i++) {
      for(int j = 0; j < image.width(); j++) {
          int color = sqrt(pow(out[i*ny+j][0], 2) + pow(out[i*ny+j][1], 2));
          if(color > Fmax) Fmax = color;
          if(color < Fmin) Fmin = color;
        }
    }
  FMIN = log(1 + Fmin);
  FMAX = log(1 + Fmax);
  for(int i = 0; i < image.height(); i++) {
      for(int j = 0; j < image.width(); j++) {
          int color = 255 * (log(1 + sqrt(pow(out[i*ny+j][0], 2) + pow(out[i*ny+j][1], 2))) - FMIN) / (FMAX - FMIN);
          temp.setPixel(j, i, qRgb(color, color, color));
        }
    }

  fftw_destroy_plan ( plan_forward );
  fftw_free ( in );
  fftw_free ( out );
  return temp;
}

QImage image_inverse(const QImage& image) {
  QImage temp(image);
  int nx = image.height(), ny = image.width();
  //做逆向傅立葉轉換，將頻率圖像轉回原來圖片
  fftw_complex* in1;
  fftw_complex* in2;
  fftw_complex* out;
  fftw_plan plan_forward, plan_backward;
  in1 = (fftw_complex*)fftw_malloc ( sizeof ( fftw_complex ) * nx * ny );
  in2 = (fftw_complex*)fftw_malloc ( sizeof ( fftw_complex ) * nx * ny );
  out = (fftw_complex*)fftw_malloc ( sizeof ( fftw_complex ) * nx * ny );
  plan_forward = fftw_plan_dft_2d ( nx, ny, in1, out, FFTW_FORWARD,
                                    FFTW_ESTIMATE );
  for (int i = 0; i < nx; i++ )
    {
      for (int j = 0; j < ny; j++ )
      {
        in1[i*ny+j][0] = qRed(image.pixel(j, i));
        in1[i*ny+j][1] = 0;
      }
    }
  fftw_execute(plan_forward);
  plan_backward = fftw_plan_dft_2d ( nx, ny, out, in2, FFTW_BACKWARD,
                                     FFTW_ESTIMATE );
  fftw_execute ( plan_backward );
  for(int i = 0; i < image.height(); i++) {
      for(int j = 0; j < image.width(); j++) {
          int color = in2[i*ny+j][0] / ( double ) ( nx * ny );
          temp.setPixel(j, i, qRgb(color, color, color));
        }
    }
  fftw_destroy_plan ( plan_backward );
  fftw_destroy_plan ( plan_forward );
  fftw_free ( in1 );
  fftw_free ( in2 );
  fftw_free ( out );
  return temp;
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setWindowTitle("Image Processing");
  ui->cut_off_scrollbar->setMinimum(10);
  ui->cut_off_scrollbar->setMaximum(500);
  buttongroup = new QButtonGroup(this);
  buttongroup->addButton(ui->if_radiobutton, 0);
  buttongroup->addButton(ui->gf_radiobutton, 1);
  buttongroup->addButton(ui->bf_radiobutton, 2);
  connect(ui->load_image, &QAction::triggered, this, [=] {
      QString filter = "All files (*.*);; PNG files (*.png);; JPG files (*.jpg);; BMP files (*.bmp)";
      file_name = QFileDialog::getOpenFileName(this, "Load Image", QDir::currentPath(), filter);
      if(file_name.isEmpty())return;
      img  = QImage(file_name)
          .convertToFormat(QImage::Format_Grayscale8);

      ui->original_image->setPixmap(QPixmap::fromImage(img));
      ui->spectrum_image->setPixmap(QPixmap::fromImage(image_spectrum(img)));
      ui->ifft_image->setPixmap(QPixmap::fromImage(image_inverse(img)));
    });
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_cut_off_scrollbar_valueChanged(int value)
{
  ui->cut_off_label->setText("D0: " + QString::number(value));
  if(img.isNull()) return;
  switch(buttongroup->checkedId()) {
    case 0:
      ui->lowpass_image->setPixmap(QPixmap::fromImage(lp_ideal_filter(file_name, value)));
      ui->highpass_image->setPixmap(QPixmap::fromImage(hp_ideal_filter(file_name, value)));
      break;
    case 1:
      ui->lowpass_image->setPixmap(QPixmap::fromImage(lp_gaussian_filter(file_name, value)));
      ui->highpass_image->setPixmap(QPixmap::fromImage(hp_gaussian_filter(file_name, value)));
      break;
    case 2:
      ui->lowpass_image->setPixmap(QPixmap::fromImage(lp_butterworth_filter(file_name, value)));
      ui->highpass_image->setPixmap(QPixmap::fromImage(hp_butterworth_filter(file_name, value)));
      break;
    }
  //homomorphic filter
  double gamma_L = ui->gamma_L->text().toDouble();//取得gamma L
  double gamma_H = ui->gamma_H->text().toDouble();//取得gamma H
  Mat img = imread(file_name.toStdString(), IMREAD_GRAYSCALE);//讀取圖片
  Mat dftInput1, dftImage1, inverseDFT, inverseDFTconverted; //新建轉換所需的容器

  img.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣容器
  for(int i = 0; i < img.rows; i++) for(int j = 0; j < img.cols; j++) {//對影像取對數
      if(dftInput1.at<uchar>(i, j) == 0) dftInput1.at<uchar>(i, j) = 0;
      dftInput1.at<uchar>(i, j) = log(dftInput1.at<uchar>(i, j));
    }
  dft(dftInput1, dftImage1, DFT_COMPLEX_OUTPUT);    //進行 DFT
  //遮罩建立 (低通濾波)
  Mat filter(Size(img.cols, img.rows), CV_32F, Scalar::all(0));
  int half_x = filter.cols / 2;
  int half_y = filter.rows / 2;
  for (int a = 0; a < filter.rows; a++) {
      for (int b = 0; b < filter.cols; b++) {
          double D = sqrt(pow((a - half_y), 2) + pow((b - half_x), 2));
          filter.at<float>(a, b) = (gamma_H - gamma_L) * (1 - exp(-c * pow(D, 2) / (value * value))) + gamma_L;
        }
    }
  filter = shift(filter); //平移遮罩
  //進行低通濾波
  Mat dftImage1_vector(filter.size(), CV_32F, dftImage1.data);
  dftImage1_vector = dftImage1_vector.mul(filter);
  // 重建圖型
  idft(dftImage1, inverseDFT, DFT_SCALE | DFT_REAL_OUTPUT); //進行 IDFT
  inverseDFT.convertTo(inverseDFTconverted, CV_8U);
  QImage result = QImage(inverseDFTconverted.cols, inverseDFTconverted.rows, QImage::Format_Grayscale8);
  for(int i = 0; i < inverseDFTconverted.rows; i++) for(int j = 0; j < inverseDFTconverted.cols; j++) {
      int color = exp(inverseDFTconverted.at<uchar>(i, j));
      result.setPixel(j, i, qRgb(color, color, color));
    }
  ui->homo_image->setPixmap(QPixmap::fromImage(result));
}


void MainWindow::on_pushButton_clicked()
{
  if(img.isNull()) return;
  ui->degradation->setPixmap(QPixmap::fromImage(motion_blur(file_name, 41)));
  ui->restoration->setPixmap(QPixmap::fromImage(inverse(file_name)));
}


void MainWindow::on_pushButton_2_clicked()
{
    if(img.isNull()) return;
    QImage noise_image(motion_blur(file_name, 41));
    double noise = 0;
    double sigma = 20, mean = 0;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean, sigma);
    for(int i = 0; i < noise_image.height(); i++) for(int j = 0; j < noise_image.width(); j++) {
        noise = (int)distribution(generator);
        int color = qRed(noise_image.pixel(j, i)) + noise;
        noise_image.setPixel(j, i, qRgb(color, color, color));
      }
    ui->degradation->setPixmap(QPixmap::fromImage(noise_image));
    ui->restoration->setPixmap(QPixmap::fromImage(inverse(file_name)));

}

