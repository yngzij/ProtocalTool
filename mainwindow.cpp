#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <libnet.h>
#include <choosedlg.h>
#include <QDebug>
#include "nicmsg.h"
#include "singleton.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    chooseDlg(new ChooseDlg)
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
    m_tableHandle->setHorizontalHeaderLabels(headList);
    this->ui->main_tableView->setModel(m_tableHandle);
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
    chooseDlg->exec();
}
