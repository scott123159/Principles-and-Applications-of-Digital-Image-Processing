#include "gaussianblurdialog.h"
#include "ui_gaussianblurdialog.h"

GaussianBlurDialog::GaussianBlurDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GaussianBlurDialog)
{
    ui->setupUi(this);
}

GaussianBlurDialog::~GaussianBlurDialog()
{
    delete ui;
}

void GaussianBlurDialog::on_pushButton_clicked()
{
    /*Check if sigma and kernel size are numbers*/
    bool sigmaChk, kernelSizeChk;
    double sigma = ui->sigmaLineEdit->text().toDouble(&sigmaChk);
    int kernelSize = ui->kernelSizeLineEdit->text().toInt(&kernelSizeChk);

    if (!(sigmaChk && kernelSizeChk))
        return;

    /*The kernel size must be odd number*/
    if (kernelSize % 2 != 1)
        return;

    emit sendSigmaAndKernelSize(sigma, kernelSize);
}
