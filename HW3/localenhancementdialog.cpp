#include "localenhancementdialog.h"
#include "ui_localenhancementdialog.h"

LocalEnhancementDialog::LocalEnhancementDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LocalEnhancementDialog)
{
    ui->setupUi(this);
}

LocalEnhancementDialog::~LocalEnhancementDialog()
{
    delete ui;
}

void LocalEnhancementDialog::on_pushButton_clicked()
{
    bool chk;

    const int size = ui->size->text().toInt(&chk);

    if (!chk || size % 2 != 1)
        return;

    const double c = ui->c->text().toDouble(&chk);

    if (!chk)
        return;

    const double k0 = ui->k0->text().toDouble(&chk);

    if (!chk)
        return;

    const double k1 = ui->k1->text().toDouble(&chk);

    if (!chk)
        return;

    const double k2 = ui->k2->text().toDouble(&chk);

    if (!chk)
        return;

    const double k3 = ui->k3->text().toDouble(&chk);

    if (!chk)
        return;

    emit applyLocalEnhancemant(size, c, k0, k1, k2, k3);
}
