#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <libnet.h>
#include <choosedlg.h>
#include <pthread.h>

#include "nicmsg.h"
#include "singleton.h"
#include "netmodel.h"
#include "singleton.h"
#include "pcapstruct.h"
#include <QItemSelectionModel>

void *ToModel(void *args);

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

    while (!self->exit_) {
        QList<QStandardItem *> itemList;
        auto *c = Singleton<NetModel>::Instance().getRow();
        ethernet = (struct sniff_ethernet *) (c);
        ip = (struct sniff_ip *) (c+ SIZE_ETHERNET);
        size_ip = IP_HL(ip) * 4;

        if (size_ip < 20) {
            fprintf(stderr,"   * Invalid IP header length: %u bytes\n", size_ip);
            continue;
        }

        no =       new QStandardItem(QString::number(number,10));
        s_ipItem = new QStandardItem(QString::fromLocal8Bit(inet_ntoa(ip->ip_src)));
        d_ipItem = new QStandardItem (QString::fromLocal8Bit(inet_ntoa(ip->ip_dst)));

        no->setTextAlignment(Qt::AlignCenter);
        s_ipItem->setTextAlignment(Qt::AlignCenter);
        d_ipItem->setTextAlignment(Qt::AlignCenter);

        QModelIndex curIndex = self->m_tableHandle->index(self->m_tableHandle->rowCount()-1,0);
        //self->m_selectItem->clearSelection();
        itemList << no <<s_ipItem <<d_ipItem ;
        self->m_tableHandle->insertRow(self->m_tableHandle->rowCount()-1,itemList);
        //self->m_selectItem->setCurrentIndex(curIndex, QItemSelectionModel::Select);
        number++;
    }
}
