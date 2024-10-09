#include "hw3.h"
#include "ui_hw3.h"

#include <QFileDialog>

HW3::HW3(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HW3)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
}

HW3::~HW3()
{
    delete ui;
}

void HW3::on_actionExit_triggered()
{
    close();
}

void HW3::on_actionOpen_triggered()
{
    /*讀取影像檔案名稱*/
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    QCoreApplication::applicationDirPath(),
                                                    tr("Image files (*.jpg *.jpeg *.bmp *.png);;All files (*.*)"));

    /*影像檔案名稱不可為空*/
    if (fileName.isEmpty())
        return;

    image = QImage(fileName);

    /*顯示影像並調整視窗大小*/
    ui->image->setPixmap(QPixmap::fromImage(image));

    /*留一點空間*/
    const int offset = 40;
    setFixedSize(image.width() + offset, image.height() + offset * 2);
}

void HW3::on_actionUndo_triggered()
{
    /*影像不可為空*/
    if (image.isNull())
        return;
}
void HW3::on_actionRedo_triggered()
{
    /*影像不可為空*/
    if (image.isNull())
        return;
}
