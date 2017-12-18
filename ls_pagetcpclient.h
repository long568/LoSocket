#ifndef LS_PAGETCPCLIENT_H
#define LS_PAGETCPCLIENT_H

#include "ls_page.h"

namespace Ui {
class LS_PageTcpClient;
}

class LS_PageTcpClient : public LS_Page
{
    Q_OBJECT

    enum {
        UNCONNECT = 0,
        CONNECTING,
        CONNECTED
    } CONNECT_STATE;

public:
    explicit LS_PageTcpClient(QWidget *parent = 0);
    ~LS_PageTcpClient();

    virtual void sendData(QByteArray a) override;
    virtual void changeLanguage(int id) override;

private:
    void closeTCP();

private slots:
    void onConnectClicked();
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);

private:
    Ui::LS_PageTcpClient *ui;
    QTcpSocket *m_tcp;
    int         m_connState;
};

#endif // LS_PAGETCPCLIENT_H
