#ifndef HW3_H
#define HW3_H

#include <QMainWindow>
#include <QStack>
#include <QElapsedTimer>

#include "gaussianblurdialog.h"
#include "medianblurdialog.h"
#include "marrhildrethedgedetectiondialog.h"
#include "localenhancementdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class HW3;
}
QT_END_NAMESPACE

class HW3 : public QMainWindow
{
    Q_OBJECT

public:
    HW3(QWidget *parent = nullptr);
    ~HW3();

public slots:
    void applyGaussianBlur(double sigma, int kernelSize);

    void applyMedianBlur(int kernelSize);

    void applyMarrHildreth(const int size, const double threshold);

    void applyLocalEnhancement(int size, double c, double k0, double k1, double k2, double k3);

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionGaussian_Blur_triggered();

    void on_actionMedian_Blur_triggered();

    void on_actionSobel_triggered();

    void on_actionMarr_Hildreth_triggered();

    void on_actionLocal_triggered();

private:
    Ui::HW3 *ui;

    /*Original image*/
    QImage image;

    /*The stack for undo/redo operation*/
    QStack<QImage> undoStack;
    QStack<QImage> redoStack;

    GaussianBlurDialog* gaussianBlurDialog;
    MedianBlurDialog* medianBlurDialog;
    MarrHildrethEdgeDetectionDialog* marrHildrethEdgeDetectionDialog;
    LocalEnhancementDialog* localEnhancementDialog;

    /*Create a timer to measure execution time*/
    QElapsedTimer timer;
};
#endif // HW3_H
