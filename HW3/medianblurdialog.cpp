#include "medianblurdialog.h"
#include "ui_medianblurdialog.h"

MedianBlurDialog::MedianBlurDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MedianBlurDialog)
{
    ui->setupUi(this);
}

MedianBlurDialog::~MedianBlurDialog()
{
    delete ui;
}

void MedianBlurDialog::on_pushButton_clicked()
{
    /*Check if the conversion of QString is successful*/
    bool chk;

    const int kernelSize = ui->kernelSizeLineEdit->text().toInt(&chk);

    if (!chk || (kernelSize % 2 != 1))
        return;

    emit sendKernelSize(kernelSize);
}
