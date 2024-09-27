#ifndef BRIGHTNESSCONTRASTDIALOG_H
#define BRIGHTNESSCONTRASTDIALOG_H

#include <QDialog>

namespace Ui {
class BrightnessContrastDialog;
}

class BrightnessContrastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrightnessContrastDialog(QWidget *parent = nullptr);
    ~BrightnessContrastDialog();

signals:
    void ok(int, int);
    void cancel();
    void adjustBrightness(int);
    void adjustContrast(int);

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_brightnessSlider_valueChanged(int value);

    void on_constrastSlider_valueChanged(int value);

    void on_BrightnessContrastDialog_rejected();

private:
    Ui::BrightnessContrastDialog *ui;
    int brightness, contrast;
};

#endif // BRIGHTNESSCONTRASTDIALOG_H
