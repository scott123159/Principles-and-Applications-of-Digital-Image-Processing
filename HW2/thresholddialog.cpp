#include "thresholddialog.h"
#include "ui_thresholddialog.h"

ThresholdDialog::ThresholdDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ThresholdDialog)
{
    ui->setupUi(this);
    ui->thresholdValueLable->setText(QString::number(ui->thresholdSlider->value()));
}

ThresholdDialog::~ThresholdDialog()
{
    delete ui;
}

void ThresholdDialog::on_okButton_clicked()
{
    emit ok(threshold);
    close();
}

void ThresholdDialog::on_thresholdSlider_valueChanged(int threshold)
{
    ui->thresholdValueLable->setText(QString::number(threshold));
    this->threshold = threshold;
    emit adjustThreshhold(threshold);
}


void ThresholdDialog::on_cancelButton_clicked()
{
    emit cancel();
    close();
}


void ThresholdDialog::on_ThresholdDialog_finished(int result)
{
    qDebug() << result;
    emit cancel();
}

