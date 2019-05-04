#ifndef SHOWWID_H
#define SHOWWID_H

#include <QDialog>
#include <sys/types.h>
#include <QStandardItemModel>
#include <string>
#include "netmodel.h"

namespace Ui {
class ShowWid;
}

class ShowWid : public QDialog
{
    Q_OBJECT

public:
    explicit ShowWid(QDialog *parent = nullptr);
    bool init(NetModel::UCHARS *msg);
    ~ShowWid();

    void print_hex_ascii_line(char *data,const u_char *payload, int len, int offset);
    void run();
private:
    Ui::ShowWid         *ui;
    NetModel::UCHARS    *m_msg;

    QStandardItemModel  *m_tableHandle;
};


#endif // SHOWWID_H


