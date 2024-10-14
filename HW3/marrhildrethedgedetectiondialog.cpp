#include "marrhildrethedgedetectiondialog.h"
#include "ui_marrhildrethedgedetectiondialog.h"

MarrHildrethEdgeDetectionDialog::MarrHildrethEdgeDetectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MarrHildrethEdgeDetectionDialog)
{
    ui->setupUi(this);
}

MarrHildrethEdgeDetectionDialog::~MarrHildrethEdgeDetectionDialog()
{
    delete ui;
}
