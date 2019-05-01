#include "netmodel.h"

#include <pcap.h>
#include <QMessageBox>
#include <QString>



void loop_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) ;

NetModel::NetModel()
{
}

NetModel::NetModel(char *nic, int port):m_nic(nic),m_port(port)
{
}

void NetModel::run()
{
    char  errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "ip";
    pcap_t *handle;
    bpf_u_int32 net;
    bpf_u_int32 mask;

    if (pcap_lookupnet(m_nic.c_str(), &net, &mask, errbuf) == -1) {
        net = 0;
        mask = 0;
        QMessageBox::critical(nullptr, "错误", "未识别的接口信息", QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    handle = pcap_open_live(m_nic.c_str(), SNAP_LEN, 1, 1000, errbuf);
    if (handle == nullptr) {
        QMessageBox::critical(nullptr, "错误", QString("不能打开设备%1").arg(QString::fromStdString(m_nic)), QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        exit(EXIT_FAILURE);
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        exit(EXIT_FAILURE);
    }

    pcap_loop(handle, 0, loop_packet, nullptr);
    pcap_freecode(&fp);
    pcap_close(handle);
}

std::string NetModel::nic() const
{
    return m_nic;
}

void NetModel::setNic(const std::string &nic)
{
    m_nic = nic;
}

int NetModel::port() const
{
    return m_port;
}

void NetModel::setPort(const int &port)
{
    m_port = port;
}


void loop_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    fprintf(stderr, "%s \n", packet);
}
