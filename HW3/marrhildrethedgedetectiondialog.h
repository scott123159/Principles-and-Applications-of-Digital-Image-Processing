#ifndef MARRHILDRETHEDGEDETECTIONDIALOG_H
#define MARRHILDRETHEDGEDETECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class MarrHildrethEdgeDetectionDialog;
}

class MarrHildrethEdgeDetectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MarrHildrethEdgeDetectionDialog(QWidget *parent = nullptr);
    ~MarrHildrethEdgeDetectionDialog();

signals:
    void sendThresholdAndKernelSize(const int size, const double threshold);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MarrHildrethEdgeDetectionDialog *ui;
};

#endif // MARRHILDRETHEDGEDETECTIONDIALOG_H
