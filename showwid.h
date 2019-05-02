#ifndef SHOWWID_H
#define SHOWWID_H

#include <QDialog>
#include <sys/types.h>
#include <QStandardItemModel>
#include <string>

namespace Ui {
class ShowWid;
}

class ShowWid : public QDialog
{
    Q_OBJECT

public:
    explicit ShowWid(QDialog *parent = nullptr);
    void init(u_char *msg);
    ~ShowWid();

private:
    Ui::ShowWid         *ui;
    u_char              *m_msg;

    QStandardItemModel  *m_tableHandle;
};

#endif // SHOWWID_H
