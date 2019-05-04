#include "showwid.h"
#include "ui_showwid.h"
#include "pcapstruct.h"
#include <QStandardItemModel>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QStringListModel>
#include <QSplitter>

ShowWid::ShowWid(QDialog*parent) :
    QDialog(parent),
    ui(new Ui::ShowWid)
{
    ui->setupUi(this);
}


/*
 *
 *   源端口    内容    备注说明
 *   目的端口
 *   序号
 *   确认序号
 *   头部序号
 *   flags.PSH
 *   ACK
 *   SYN
 *   UGR
 *   FIN
 *   RST
 *   窗口大小
 *   校验和
 *   紧急指针
 *
 */


bool ShowWid::init(NetModel::UCHARS* msg)
{
    m_msg = msg;
    auto  *c_msg = (u_char *)&*m_msg->begin();
    struct sniff_ethernet *ethernet;
    struct sniff_ip *ip;
    struct sniff_tcp *tcp;

    int size_ip;
    int size_tcp;
    QString proto;

    ip = (struct sniff_ip *) (c_msg+ SIZE_ETHERNET);  //ip src and dest  and proto
    size_ip = IP_HL(ip) * 4;
    fprintf(stderr, "ip len = %u ", size_ip);
    fprintf(stderr, "%d \n", ip->ip_p);
    switch (ip->ip_p) {
    case IPPROTO_TCP:
         proto = "TCP";
        break;
    case IPPROTO_UDP:
        proto = "UDP";
        //break;
        return false;
    case IPPROTO_ICMP:
        proto = "ICMP";
        return false;
        break;
    case IPPROTO_IP:
        proto = "IP";
        return false;
        break;
    default:
        proto = "unknow";
        return false;
    }

    setWindowTitle(QString("%1协议分析").arg(proto));
    QStringList headList;
    headList << QString("%1协议分析").arg(proto) << "内容" << "备注说明";
    QStringList li;
    m_tableHandle = new QStandardItemModel(1, headList.count());
    m_tableHandle->setHorizontalHeaderLabels(headList);
    ui->show_tableView->setModel(m_tableHandle);
    ui->show_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->show_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    tcp = (struct sniff_tcp *) (c_msg + SIZE_ETHERNET + size_ip);

    size_tcp = TH_OFF(tcp) * 4;

    if (size_tcp < 20) {
        //QMessageBox::warning(nullptr, "错误", "无效的TCP报文");
        return false;
    }


    return true;
}


ShowWid::~ShowWid()
{
    delete ui;
}

void ShowWid::print_hex_ascii_line(char *data, const u_char *payload, int len, int offset)
{

    int i;
    int gap;
    const u_char *ch;
    sprintf(data, "%05d   ", offset);
    ch = payload;
    for(i = 0; i < len; i++) {
        sprintf(data + strlen(data), "%02x ", *ch);
        ch++;

        if (i == 7)
            sprintf(data + strlen(data), " ");
    }

    if (len < 8)
        sprintf(data + strlen(data), " ");


    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            sprintf(data + strlen(data), "   ");
        }
    }
    sprintf(data + strlen(data), "   ");


    ch = payload;
    for(i = 0; i < len; i++) {
        if (isprint(*ch))
            sprintf(data + strlen(data), "%c", *ch);
        else
            sprintf(data + strlen(data), ".");
        ch++;
    }

    sprintf(data + strlen(data),"\n");

}

void ShowWid::run()
{
    struct sniff_tcp *tcp;
    struct sniff_ip *ip;

    auto  *c_msg = (u_char *)&*m_msg->begin();

    int size_ip;
    int size_tcp;
    ip = (struct sniff_ip *) (c_msg+ SIZE_ETHERNET);  //ip src and dest  and proto
    size_ip = IP_HL(ip) * 4;
    tcp = (struct sniff_tcp *) (c_msg + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp) * 4;

    QList<QStandardItem *> sport_itemList;
    QStandardItem *s_port_1 = new QStandardItem("源端口");
    QStandardItem *s_port_2= new QStandardItem(QString::number(ntohs(tcp->th_sport)));
    QStandardItem *s_port_3= new QStandardItem("16位源端口");
    sport_itemList << s_port_1 <<s_port_2 << s_port_3;


    QList<QStandardItem *> dport_itemList;
    QStandardItem *d_port_1 = new QStandardItem("目的端口");
    QStandardItem *d_port_2= new QStandardItem(QString::number(ntohs(tcp->th_dport)));
    QStandardItem *d_port_3= new QStandardItem("16位源端口");
    dport_itemList << d_port_1 <<d_port_2 << d_port_3;

    QList<QStandardItem *> no_itemList;
    QStandardItem *no_1 = new QStandardItem("序号");
    QStandardItem *no_2= new QStandardItem(QString::number(ntohs(tcp->th_seq)));
    QStandardItem *no_3= new QStandardItem("32位序号");
    no_itemList<< no_1 << no_2 <<no_3;

    QList<QStandardItem *> ack_itemList;
    QStandardItem *ack_1 = new QStandardItem("确认序号");
    QStandardItem *ack_2= new QStandardItem(QString::number(ntohs(tcp->th_ack)));
    QStandardItem *ack_3= new QStandardItem("ACK设置为0,32位序号无效");
    ack_itemList << ack_1 << ack_2 << ack_3;

    QList<QStandardItem *> len_itemList;
    QStandardItem *len_1 = new QStandardItem("头部长度");
    QStandardItem *len_2 = new QStandardItem(QString::number((size_tcp)));
    QStandardItem *len_3 = new QStandardItem("头部长度");
    len_itemList << len_1 << len_2<< len_3;

    QList<QStandardItem *> psh_itemList;
    QStandardItem *psh_1 = new QStandardItem("fsags-PSH");
    QStandardItem *psh_2 = new QStandardItem("PSH设置为0");
    QStandardItem *psh_3 = new QStandardItem("说明接受方不需要尽快将这个报文交给应用层");
    psh_itemList<< psh_1 << psh_2 << psh_3;

    QList<QStandardItem *> fack_itemList;
    QStandardItem *fack_1= new QStandardItem("fsags-ACK");
    QStandardItem *fack_2 = new QStandardItem("ACK设置为0");
    QStandardItem *fack_3 = new QStandardItem("说明确认报文无效");
    fack_itemList << fack_1 << fack_2 <<fack_3;


    QList<QStandardItem *> ugr_itemList;
    QStandardItem *ugr_1= new QStandardItem("fsags-UGR");
    QStandardItem *ugr_2 = new QStandardItem("UGR设置为0");
    QStandardItem *ugr_3 = new QStandardItem("保留的六位");
    ugr_itemList << ugr_1 <<ugr_2 << ugr_3;

    QList<QStandardItem *> fin_itemList;
    QStandardItem *fin_1= new QStandardItem("fsags-Fin");
    QStandardItem *fin_2 = new QStandardItem("FIN设置为0");
    QStandardItem *fin_3 = new QStandardItem("没有完成发送任务");
    fin_itemList << fin_1 <<fin_2 <<fin_3;

    QList<QStandardItem *> rst_itemList;
    QStandardItem *rst_1= new QStandardItem("fsags-RST");
    QStandardItem *rst_2 = new QStandardItem("RST设置为0");
    QStandardItem *rst_3 = new QStandardItem("不需要重新建立连接");
    rst_itemList << rst_1 << rst_2 << rst_3;

    QList<QStandardItem *> wndsize_itemList;
    QStandardItem *wnd_1= new QStandardItem("窗口大小");
    QStandardItem *wnd_2 = new QStandardItem(QString::number(ntohs(tcp->th_win)));
    QStandardItem *wnd_3 = new QStandardItem("16位窗口大小");
    wndsize_itemList << wnd_1 << wnd_2 << wnd_3;

    QList<QStandardItem *> sum_itemList;
    QStandardItem *sum_1 = new QStandardItem("校验和");
    QStandardItem *sum_2 = new QStandardItem(QString::number(ntohs(tcp->th_sum)));
    QStandardItem *sum_3 = new QStandardItem("16位校验和,根据首部计算的校验和码");
    sum_itemList << sum_1 << sum_2 << sum_3;

    QList<QStandardItem *> urp_itemList;
    QStandardItem *urp_1 = new QStandardItem("紧急指针");
    QStandardItem *urp_2 = new QStandardItem(QString::number(ntohs(tcp->th_urp)));
    QStandardItem *urp_3 = new QStandardItem("URG为0,紧急指针无效");
    urp_itemList << urp_1 << urp_2 << urp_3;

    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, no_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, sport_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, dport_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, len_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, wndsize_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, ack_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, sum_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, urp_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, rst_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, fin_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, ugr_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, fack_itemList);
    m_tableHandle->insertRow(m_tableHandle->rowCount() - 1, psh_itemList);



    fprintf(stderr, "len %d \n", m_msg->size());

    char data[m_msg->size()];
    char *data_ch = data;

    int len_rem = m_msg->size();
    int line_width = 16;
    int line_len;
    int offset = 0;

    u_char *ch = (u_char *)&*m_msg->begin();
    ch += SIZE_ETHERNET;
    if (m_msg->size()<= 0)
        return;


    if (m_msg->size() <= line_width) {
        print_hex_ascii_line(data,ch, m_msg->size(), offset);
        return;
    }
    QStringList data_li;
    for ( ;; ) {
        memset(data, '\0', sizeof(data));
        line_len = line_width % len_rem;
        fprintf(stderr, "line : %d \n", line_len);
        print_hex_ascii_line(data, ch, line_len, offset);
        data_li << QString::fromLocal8Bit(data);

        len_rem = len_rem - line_len;
        ch = ch + line_len;
        data_ch += line_len;
        offset = offset + line_width;
        if (len_rem <= line_width) {
            print_hex_ascii_line(data, ch, len_rem, offset);
            data_li << QString::fromLocal8Bit(data);
            break;
        }
    }

    QStringListModel *listModel = new QStringListModel(this);
    ui->sec_listView->setModel(listModel);
    listModel->setStringList(data_li);

}


