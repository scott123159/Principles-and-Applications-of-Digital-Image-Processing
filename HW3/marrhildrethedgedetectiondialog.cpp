#include "marrhildrethedgedetectiondialog.h"
#include "ui_marrhildrethedgedetectiondialog.h"

MarrHildrethEdgeDetectionDialog::MarrHildrethEdgeDetectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MarrHildrethEdgeDetectionDialog)
{
    ui->setupUi(this);
}

MarrHildrethEdgeDetectionDialog::~MarrHildrethEdgeDetectionDialog()
{
    delete ui;
}

void MarrHildrethEdgeDetectionDialog::on_pushButton_clicked()
{
    bool chk;

    const int size = ui->kernelSizeEditLine->text().toInt(&chk);

    /*The conversion must be successful*/
    if (!chk)
        return;

    const double threshold = ui->thresholdEditLine->text().toDouble(&chk);

    /*The conversion must be successful*/
    if (!chk)
        return;

    /*The kernel size must be odd number*/
    if (size % 2 == 0 || size < 3)
        return;

    emit sendThresholdAndKernelSize(size, threshold);
}
