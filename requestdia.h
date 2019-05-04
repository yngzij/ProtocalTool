#ifndef REQUESTDIA_H
#define REQUESTDIA_H

#include <QDialog>

namespace Ui {
class RequestDia;
}

class RequestDia : public QDialog
{
    Q_OBJECT

public:
    explicit RequestDia(QWidget *parent = nullptr);
    ~RequestDia();

    void init();
    void sendARP(std:: string dev,
                 u_int8_t *src_ip,
                 u_int8_t *dst_ip,
                 u_int8_t *src_mac,
                 u_int8_t *dst_mac);


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::RequestDia *ui;
};

#endif // REQUESTDIA_H
