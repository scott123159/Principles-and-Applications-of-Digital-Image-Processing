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

private:
    Ui::MarrHildrethEdgeDetectionDialog *ui;
};

#endif // MARRHILDRETHEDGEDETECTIONDIALOG_H
