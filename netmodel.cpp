#include "netmodel.h"

#include <QMessageBox>
#include <QString>
#include <unistd.h>

#include "pcapstruct.h"

void loop_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) ;
void *loop_event(void *args);

NetModel::NetModel()
{

}

NetModel::NetModel(char *nic, int port):m_nic(nic),m_port(port)
{
}


void NetModel::run()
{
    if (exit_ && run_ ) {
        exit_ = false;
        return;
    }

    run_ = true;
    char  errbuf[PCAP_ERRBUF_SIZE];
    char filter_exp[] = "ip";
    bpf_u_int32 net;
    bpf_u_int32 mask;


    m_handle = pcap_open_live(m_nic.c_str(), SNAP_LEN, 1, 1000, errbuf);
    if (m_handle == nullptr) {
        QMessageBox::critical(nullptr, "错误", QString("不能打开设备%1").arg(QString::fromStdString(m_nic)), QMessageBox::Yes,QMessageBox::Yes);
        return;
    }

    if (pcap_compile(m_handle, &fp, filter_exp, false, net) == -1) {
        exit(EXIT_FAILURE);
    }

    if (pcap_setfilter(m_handle, &fp) == -1) {
        exit(EXIT_FAILURE);
    }

    pthread_create(&pid, nullptr, loop_event, this);
    pthread_detach(pid);
}

void NetModel::stop()
{
    exit_ = true;
    //pthread_exit(pid);
    //pcap_freecode(&fp);
    //pcap_close(m_handle);
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

NetModel::UCHARS *NetModel::getRow()
{
    pthread_mutex_lock(&mux_);
    if (q_.empty()) {
        pthread_cond_wait(&cond_, &mux_);
    }
    auto t = q_.front();
    pthread_mutex_unlock(&mux_);
    q_.pop();
    return t;
}

void NetModel::init()
{
    mux_  = PTHREAD_MUTEX_INITIALIZER;
    cond_ = PTHREAD_COND_INITIALIZER;
    exit_ = false;
    run_  = false;
}


void loop_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    auto *self = reinterpret_cast<NetModel*>(args);
    NetModel::UCHARS *t = new NetModel::UCHARS;
    t->resize(header->caplen);

    if (!self->exit_) {
        pthread_mutex_lock(&self->mux_);
        memcpy(&*t->begin(), (char *)packet, header->caplen);
        self->q_.push(t);
        pthread_cond_signal(&self->cond_);
        pthread_mutex_unlock(&self->mux_);
    }
}

void *loop_event(void *args) {
    auto *self = reinterpret_cast<NetModel *>(args);
    pcap_loop(self->m_handle, -1, loop_packet, (u_char *) self);

}
