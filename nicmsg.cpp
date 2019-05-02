#include "nicmsg.h"
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string.h>


NICMsg::NICMsg():m_netcard(nullptr)
{

}

NIC *NICMsg::GetNetMsg()
{
    return m_netcard->first;

}

void NICMsg::init()
{
    pcap_if_t *alldev = {};
    pcap_if_t *dev;
    char      errbuf[PCAP_ERRBUF_SIZE];
    pcap_findalldevs(&alldev, errbuf);

    for (dev = alldev; dev != nullptr; dev = dev->next) {
        for (auto *p = dev->addresses; p != nullptr; p = p->next) {
            if (nullptr != p){
                auto toAddrIn = reinterpret_cast<struct sockaddr_in*>(p->addr);
                if (toAddrIn->sin_family == AF_INET) {
                    NIC *v = reinterpret_cast<struct NIC*>(calloc(1,sizeof (NIC)));
                    v->name = dev->name;
                    //inet_ntao不可重入
                    memcpy(v->ip_v4, inet_ntoa(toAddrIn->sin_addr),strlen(inet_ntoa(toAddrIn->sin_addr)));
                    push(v);
                }
            }
        }
    }
    done();
}


void NICMsg::push(NIC *node)
{
    if (m_netcard == nullptr) {
        m_netcard = node;
        m_netcard->first = node;
        m_netcard->last = node;
        m_netcard->next = nullptr;
        return;
    }
    m_netcard->last->next = node;
    m_netcard->last = m_netcard->last->next;
}


void NICMsg::destory()
{
    for (auto *p = m_netcard->first;p != nullptr; ) {
        auto *t = p->next;
        free(p);
        p = t;
    }
}


void NICMsg::done()
{
    m_netcard->last->next = nullptr;
}




