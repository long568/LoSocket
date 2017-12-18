#include "ls_pagetcpsrv.h"
#include "ui_ls_pagetcpsrv.h"

LS_PageTcpSrv::LS_PageTcpSrv(QWidget *parent) :
    LS_Page(parent),
    ui(new Ui::LS_PageTcpSrv)
{
    ui->setupUi(this);
}

LS_PageTcpSrv::~LS_PageTcpSrv()
{
    delete ui;
}

void LS_PageTcpSrv::sendData(QByteArray a)
{
}

void LS_PageTcpSrv::changeLanguage(int id)
{
}
