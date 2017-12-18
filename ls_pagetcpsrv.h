#ifndef LS_PAGETCPSRV_H
#define LS_PAGETCPSRV_H

#include "ls_page.h"

namespace Ui {
class LS_PageTcpSrv;
}

class LS_PageTcpSrv : public LS_Page
{
    Q_OBJECT

public:
    explicit LS_PageTcpSrv(QWidget *parent = 0);
    ~LS_PageTcpSrv();

    virtual void sendData(QByteArray a) override;
    virtual void changeLanguage(int id) override;

private:
    Ui::LS_PageTcpSrv *ui;
};

#endif // LS_PAGETCPSRV_H
