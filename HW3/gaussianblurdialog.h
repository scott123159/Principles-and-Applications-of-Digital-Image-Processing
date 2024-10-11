#ifndef GAUSSIANBLURDIALOG_H
#define GAUSSIANBLURDIALOG_H

#include <QDialog>

namespace Ui {
class GaussianBlurDialog;
}

class GaussianBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianBlurDialog(QWidget *parent = nullptr);
    ~GaussianBlurDialog();

signals:
    void sendSigmaAndKernelSize(double, int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::GaussianBlurDialog *ui;
};

#endif // GAUSSIANBLURDIALOG_H
