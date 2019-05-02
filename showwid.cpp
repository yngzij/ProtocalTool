#include "showwid.h"
#include "ui_showwid.h"

ShowWid::ShowWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowWid)
{
    ui->setupUi(this);
}

ShowWid::~ShowWid()
{
    delete ui;
}
