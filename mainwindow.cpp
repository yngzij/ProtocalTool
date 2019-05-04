#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <libnet.h>
#include <choosedlg.h>
#include <pthread.h>
#include <algorithm>

#include "nicmsg.h"
#include "singleton.h"
#include "netmodel.h"
#include "singleton.h"
#include "pcapstruct.h"
#include "showwid.h"
#include <sys/types.h>
#include <QItemSelectionModel>
#include <QDebug>
#include <QMessageBox>
#include "requestdia.h"

void *ToModel(void *args);
void print_hex_ascii_line(char *payload, int len, int offset);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_chooseDlg(new ChooseDlg),
    exit_(false)
{
    ui->setupUi(this);
    initUI();
    signalAndSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    setWindowTitle("网络协议学习系统");
    QStringList headList;

    headList << "No." << "源地址" << "目的地址" <<"协议类型";
    m_tableHandle = new QStandardItemModel(1, headList.count());
    m_selectItem = new QItemSelectionModel(m_tableHandle);
    m_tableHandle->setHorizontalHeaderLabels(headList);

    this->ui->main_tableView->setModel(m_tableHandle);
    this->ui->main_tableView->setSelectionModel(m_selectItem);
    this->ui->main_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->ui->main_tableView->verticalHeader()->setHidden(true);
    this->ui->main_tableView->verticalHeader()->setDefaultSectionSize(3);
    this->ui->main_tableView->verticalHeader()->setVisible(false);
    this->ui->main_tableView->setStyleSheet("selection-background-color:lightblue;");
    this->ui->main_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

//save interface and port
void MainWindow::signalAndSlot()
{
    connect(this->m_selectItem, SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));
}


void MainWindow::on_ac_choose_triggered()
{
    m_chooseDlg->exec();
}

void MainWindow::on_ac_start_triggered()
{
    Singleton<NetModel>::Instance().run();
    pthread_t pid;
    pthread_create(&pid, nullptr, ToModel, this);
    pthread_detach(pid);
}

void MainWindow::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if (current.row() >= pages.size()) {
        fprintf(stderr, "test");
        return;
    }

    if (current.isValid()) {
        ShowWid wid;
        bool ok = wid.init(this->pages.at(current.row()));

        if (!ok) {
            QMessageBox::warning(this, "错误", "无效的报文");
            return;
        }
        wid.run();
        wid.exec();
    }

}

void *ToModel(void *args) {
    auto *self = reinterpret_cast<MainWindow *>(args);
    static ssize_t number = 1;
    struct sniff_ethernet *ethernet;
    struct sniff_ip *ip;

    int size_ip;
    int size_tcp;

    QStandardItem *no;
    QStandardItem *s_ipItem;
    QStandardItem *d_ipItem;
    QStandardItem *protoItem;

    while (!self->exit_) {
        QList<QStandardItem *> itemList;
        auto *c = Singleton<NetModel>::Instance().getRow();
        auto *c_uchar = (u_char*) &*c->begin();

        std::string t;
        ethernet = (struct sniff_ethernet *)c_uchar;
        ip = (struct sniff_ip *) (c_uchar+ SIZE_ETHERNET);
        size_ip = IP_HL(ip) * 4;

        if (size_ip < 20) {
            fprintf(stderr,"   * Invalid IP header length: %u bytes\n", size_ip);
            continue;
        }

        switch (ip->ip_p) {
        case IPPROTO_TCP:
            protoItem = new QStandardItem("TCP");
            break;
        case IPPROTO_UDP:
            protoItem = new QStandardItem("UDP");
            break;
            case IPPROTO_ICMP:
            protoItem = new QStandardItem("ICMP");
            break;
            case IPPROTO_IP:
            protoItem = new QStandardItem("IP");
            break;
            default:
            protoItem = new QStandardItem("unknow");
        }
        self->pages.push_back(c);
        no =       new QStandardItem(QString::number(number,10));
        s_ipItem = new QStandardItem(QString::fromLocal8Bit(inet_ntoa(ip->ip_src)));
        d_ipItem = new QStandardItem (QString::fromLocal8Bit(inet_ntoa(ip->ip_dst)));

        no->setTextAlignment(Qt::AlignCenter);
        s_ipItem->setTextAlignment(Qt::AlignCenter);
        d_ipItem->setTextAlignment(Qt::AlignCenter);
        protoItem->setTextAlignment(Qt::AlignCenter);

        QModelIndex curIndex = self->m_tableHandle->index(self->m_tableHandle->rowCount()-1,0);

        //self->m_selectItem->clearSelection();
        itemList << no <<s_ipItem <<d_ipItem <<protoItem;
        self->m_tableHandle->insertRow(self->m_tableHandle->rowCount()-1,itemList);

        //self->m_selectItem->setCurrentIndex(curIndex, QItemSelectionModel::SelectCurrent);
        number++;
    }
}

void MainWindow::on_ac_stop_triggered()
{
    Singleton<NetModel>::Instance().stop();
}


void print_hex_ascii_line(char *payload, int len, int offset) {
    int i;
    int gap;
    char *ch;

    /* offset */
    printf("%05d   ", offset);

    /* hex */
    ch = payload;
    for (i = 0; i < len; i++) {
        printf("%02x ", *ch);
        ch++;
        /* print extra space after 8th byte for visual aid */
        if (i == 7)
            printf(" ");
    }
    /* print space to handle line less than 8 bytes */
    if (len < 8)
        printf(" ");

    /* fill hex gap with spaces if not full line */
    if (len < 16) {
        gap = 16 - len;
        for (i = 0; i < gap; i++) {
            printf("   ");
        }
    }
    printf("   ");
    /* ascii (if printable) */

    ch = payload;
    for (i = 0; i < len; i++) {
        if (isprint(*ch))
            printf("%c", *ch);
        else
            printf(".");
        ch++;
    }
    printf("\n");

    return;
}

void MainWindow::on_actionARP_triggered()
{
    RequestDia dia;
    dia.exec();
}
