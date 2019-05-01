#ifndef CHOOSEDLG_H
#define CHOOSEDLG_H

#include <QDialog>

namespace Ui {
class ChooseDlg;
}

class ChooseDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseDlg(QWidget *parent = nullptr);
    ~ChooseDlg();


    int getPort() const;
    QString getInterface() const;

public slots:
    void init();


    void changeData();

private slots:
    void on_cacel_btn_clicked();
    void on_ok_btn_clicked();

private:
    Ui::ChooseDlg *ui;
    int                  m_port;
    QString              m_interface;
};

#endif // CHOOSEDLG_H
