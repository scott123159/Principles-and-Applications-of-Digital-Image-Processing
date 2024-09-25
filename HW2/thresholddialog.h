#ifndef THRESHOLDDIALOG_H
#define THRESHOLDDIALOG_H

#include <QDialog>

namespace Ui {
class ThresholdDialog;
}

class ThresholdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThresholdDialog(QWidget *parent = nullptr);
    ~ThresholdDialog();

signals:
    void adjustThreshhold(int);
    void ok(int);
    void cancel();

private slots:
    void on_okButton_clicked();

    void on_thresholdSlider_valueChanged(int);

    void on_cancelButton_clicked();

    void on_ThresholdDialog_finished(int result);

private:
    Ui::ThresholdDialog *ui;
    int threshold;
};

#endif // THRESHOLDDIALOG_H
