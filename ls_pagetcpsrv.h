#ifndef LS_PAGETCPSRV_H
#define LS_PAGETCPSRV_H

#include "ls_page.h"
#include "ls_pagetcpsrvitem.h"

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

protected:
    virtual void loadConfig() override;
    virtual void saveConfig() override;

private:
    void closeServer();

private slots:
    void onCreateServer();
    void onCloseClient();
    void onCloseAll();
    void onClientSelected();

    void onAcceptError();
    void onNewConnection();

    void onClientReadyRead(LS_PageTcpSrvItem* item);
    void onClientDisconnected(LS_PageTcpSrvItem* item);
    void onClientError(LS_PageTcpSrvItem* item, QAbstractSocket::SocketError error);

private:
    Ui::LS_PageTcpSrv *ui;
    bool        m_isCreated;
    QTcpServer *m_srv;
};

#endif // LS_PAGETCPSRV_H
