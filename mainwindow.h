#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QStandardItemModel>
#include <choosedlg.h>
#include "netmodel.h"
#include <vector>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initUI();
    void signalAndSlot();

public:
    bool                            exit_;
    QStandardItemModel              *m_tableHandle;
    QItemSelectionModel             *m_selectItem;
    std::vector<NetModel::UCHARS *>        pages;
private slots:
    void on_ac_choose_triggered();
    void on_ac_start_triggered();
    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_ac_stop_triggered();

    void on_actionARP_triggered();

private:
    Ui::MainWindow         *ui;
    ChooseDlg              *m_chooseDlg;

};

#endif // MAINWINDOW_H
