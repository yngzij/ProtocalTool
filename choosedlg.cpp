#include "choosedlg.h"
#include "ui_choosedlg.h"
#include "singleton.h"
#include "nicmsg.h"

#include <QDebug>

ChooseDlg::ChooseDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDlg)
{
    ui->setupUi(this);
    init();
    setWindowTitle("接口选择");
}

ChooseDlg::~ChooseDlg()
{
    delete ui;
}

void ChooseDlg::init()
{
    for (auto *i = Singleton<NICMsg>::Instance().GetNetMsg();
         i != nullptr; i = i->next) {
        auto item = QString::fromLocal8Bit(i->name);
        this->ui->interface_comboBox->addItem(item);
    }
    this->ui->port_spinBox->setValue(80);
}



void ChooseDlg::on_cacel_btn_clicked()
{
    this->hide();
}

void ChooseDlg::on_ok_btn_clicked()
{
    changeData();
    this->hide();
}

QString ChooseDlg::getInterface() const
{
    return m_interface;
}

int ChooseDlg::getPort() const
{
    return m_port;
}


void ChooseDlg::changeData()
{
    m_port= this->ui->port_spinBox->value();
    m_interface= this->ui->interface_comboBox->currentText();
    qDebug() << m_port << m_interface <<'\n';
}


