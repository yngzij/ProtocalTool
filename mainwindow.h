#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QStandardItemModel>
#include <choosedlg.h>

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


private slots:
    void on_ac_choose_triggered();

private:
    Ui::MainWindow       *ui;
    QStandardItemModel   *m_tableHandle;
    ChooseDlg            *chooseDlg;

};

#endif // MAINWINDOW_H
