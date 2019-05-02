#ifndef NETMODEL_H
#define NETMODEL_H

#include <string>
#include <queue>
#include <sys/types.h>
#include <pthread.h>
#include <pcap.h>


class NetModel
{
public:
    NetModel();
    NetModel(char *, int);

    void              run();
    void              stop() ;
    std::string       nic() const;
    void              setNic(const std::string &nic);
    int               port() const;
    void              setPort(const int &port);
    u_char            *getRow();
    void              init();

public:
    std::queue<const u_char *>   q_;
    pthread_mutex_t              mux_;
    pthread_cond_t               cond_;
    pcap_t                      *m_handle;
    bool                         exit_;
private:
    std::string                  m_nic;
    int                          m_port;
    struct bpf_program           fp;
    pthread_t                    pid;
    bool                         run_;

};

#endif // NETMODEL_H
