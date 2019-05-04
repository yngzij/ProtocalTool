#include "requestdia.h"
#include "ui_requestdia.h"
#include <libnet.h>
#include <libgen.h>
#include "singleton.h"
#include "nicmsg.h"
#include <QMessageBox>

RequestDia::RequestDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RequestDia)
{
    ui->setupUi(this);
    init();
}


RequestDia::~RequestDia()
{
    delete ui;
}

void RequestDia::init()
{

    this->setWindowTitle("协议构造");

    for (auto *i = Singleton<NICMsg>::Instance().GetNetMsg();
         i != nullptr; i = i->next) {
        auto item = QString::fromLocal8Bit(i->name);
        this->ui->interface_comboBox->addItem(item);
    }

}




void RequestDia::sendARP(std:: string dev,
                         u_int8_t *src_ip,
                         u_int8_t *dst_ip,
                         u_int8_t *src_mac,
                         u_int8_t *dst_mac)
{
    libnet_t *handler;
    int packet_size;
    u_int32_t dst_n;
    u_int32_t src_n;
    char error[LIBNET_ERRBUF_SIZE];
    libnet_ptag_t arp_proto;
    libnet_ptag_t eth_proto;

    dst_n = libnet_name2addr4(handler, (char *)dst_ip, LIBNET_RESOLVE);
    src_n = libnet_name2addr4(handler, (char *)src_ip, LIBNET_RESOLVE);

    if (dst_n == -1 || src_n == -1) {
        QMessageBox::warning(this, "错误", "地址解析错误", QMessageBox::Yes);
        return;
    }
    if ((handler = libnet_init(LIBNET_LINK_ADV, dev.c_str(), error))  == nullptr) {
        QMessageBox::warning(this, "错误", QString("%1").arg(QString::fromLocal8Bit(error)), QMessageBox::Yes);
        return;
    }
    arp_proto = libnet_build_arp(
                ARPHRD_ETHER,
                ETHERTYPE_IP,
                6,
                4,
                ARPOP_REQUEST,
                src_mac,
                (u_int8_t *)&src_n,
                dst_mac,
                (u_int8_t *)&dst_n,
                nullptr,
                0,
                handler,
                0
                );
    if (arp_proto == -1) {
        QMessageBox::warning(this, "错误", "构造失败", QMessageBox::Yes);
        return;
    }

    eth_proto = libnet_build_ethernet(
                dst_mac,
                src_mac,
                ETHERTYPE_ARP,
                nullptr,
                0,
                handler,
                0
                );
    if (eth_proto == -1) {
        QMessageBox::warning(this, "错误", "构造失败", QMessageBox::Yes);
        return;
    }
    packet_size = libnet_write(handler);
    QMessageBox::about(this, "提示", "发送成功");
}

void RequestDia::on_pushButton_2_clicked()
{
    QString src_host = this->ui->src_lineEdit->text();
    QString dst_host = this->ui->dst_lineEdit->text();
    QString dev      = this->ui->interface_comboBox->currentText();

    for (auto *i = Singleton<NICMsg>::Instance().GetNetMsg();
         i != nullptr; i = i->next) {
        auto item = QString::fromLocal8Bit(i->name);
        this->ui->interface_comboBox->addItem(item);
    }
}

void RequestDia::on_pushButton_clicked()
{
    ui->src_lineEdit->setText("");
    ui->dst_lineEdit->setText("");
}
