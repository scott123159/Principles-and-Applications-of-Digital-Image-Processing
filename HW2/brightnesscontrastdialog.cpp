#include "brightnesscontrastdialog.h"
#include "ui_brightnesscontrastdialog.h"

BrightnessContrastDialog::BrightnessContrastDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BrightnessContrastDialog)
{
    ui->setupUi(this);
    ui->brightnessLabel->setText(QString::number(ui->brightnessSlider->value()));
    ui->contrastLabel->setText(QString::number(ui->constrastSlider->value()));
}

BrightnessContrastDialog::~BrightnessContrastDialog()
{
    delete ui;
}

void BrightnessContrastDialog::on_cancelButton_clicked()
{
    emit cancel();
    close();
}


void BrightnessContrastDialog::on_okButton_clicked()
{
    emit ok(brightness, contrast);
    close();
}


void BrightnessContrastDialog::on_brightnessSlider_valueChanged(int value)
{
    emit adjustBrightness(value);
    brightness = value;
    ui->brightnessLabel->setText(QString::number(ui->brightnessSlider->value()));
}


void BrightnessContrastDialog::on_constrastSlider_valueChanged(int value)
{
    emit adjustContrast(value);
    contrast = value;
    ui->contrastLabel->setText(QString::number(ui->constrastSlider->value()));
}


void BrightnessContrastDialog::on_BrightnessContrastDialog_rejected()
{
    emit cancel();
}

