#ifndef LOCALENHANCEMENTDIALOG_H
#define LOCALENHANCEMENTDIALOG_H

#include <QDialog>

namespace Ui {
class LocalEnhancementDialog;
}

class LocalEnhancementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LocalEnhancementDialog(QWidget *parent = nullptr);
    ~LocalEnhancementDialog();

signals:
    void applyLocalEnhancemant(int size, double c, double k0, double k1, double k2, double k3);

private slots:
    void on_pushButton_clicked();

private:
    Ui::LocalEnhancementDialog *ui;
};

#endif // LOCALENHANCEMENTDIALOG_H
