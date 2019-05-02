#ifndef SHOWWID_H
#define SHOWWID_H

#include <QWidget>

namespace Ui {
class ShowWid;
}

class ShowWid : public QWidget
{
    Q_OBJECT

public:
    explicit ShowWid(QWidget *parent = nullptr);
    ~ShowWid();

private:
    Ui::ShowWid *ui;
};

#endif // SHOWWID_H
