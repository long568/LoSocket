#ifndef LS_PAGEUDP_H
#define LS_PAGEUDP_H

#include "ls_page.h"

namespace Ui {
class LS_PageUdp;
}

class LS_PageUdp : public LS_Page
{
    Q_OBJECT

public:
    explicit LS_PageUdp(QWidget *parent = 0);
    ~LS_PageUdp();

    virtual void sendData(QByteArray a) override;
    virtual void changeLanguage(int id) override;

private slots:
    void onBindUdp();
    void onReadyRead();

private:
    Ui::LS_PageUdp *ui;
    QUdpSocket *m_udp;
    bool        m_isBind;
};

#endif // LS_PAGEUDP_H
