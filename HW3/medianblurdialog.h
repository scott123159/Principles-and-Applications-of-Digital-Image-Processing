#ifndef MEDIANBLURDIALOG_H
#define MEDIANBLURDIALOG_H

#include <QDialog>

namespace Ui {
class MedianBlurDialog;
}

class MedianBlurDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MedianBlurDialog(QWidget *parent = nullptr);
    ~MedianBlurDialog();

signals:
    void sendKernelSize(int kernelSize);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MedianBlurDialog *ui;
};

#endif // MEDIANBLURDIALOG_H
