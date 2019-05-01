#ifndef NICMSG_H
#define NICMSG_H

const char IPV4_LEN = 32;

typedef struct NIC {
    struct NIC        *first;
    struct NIC        *last;
    struct NIC        *data;
    char              ip_v4[IPV4_LEN];
    char              *name;
    struct NIC        *next;
}NIC;

class NICMsg
{
public:
    NICMsg();
    NIC *GetNetMsg();
    void init();

private:
    void     push(NIC *);
    NIC      *pop();
    void     destory();
    void     done();

private:
    NIC     *m_netcard;
};

#endif // NICMSG_H
