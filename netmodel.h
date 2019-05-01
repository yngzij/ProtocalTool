#ifndef NETMODEL_H
#define NETMODEL_H
#include <string>
const int SNAP_LEN = 1518;
/* ethernet headers are always exactly 14 bytes [1] */
const int SIZE_ETHERNET = 14;
/* Ethernet addresses are 6 bytes */
const int ETHER_ADDR_LEN = 6;

class NetModel
{
public:
    NetModel();
    NetModel(char *, int);
    void  run();

    std::string nic() const;
    void setNic(const std::string &nic);

    int port() const;
    void setPort(const int &port);

private:
    std::string  m_nic;
    int          m_port;
};

#endif // NETMODEL_H
