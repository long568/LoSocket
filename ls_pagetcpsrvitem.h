#ifndef LS_PAGETCPSRVITEM_H
#define LS_PAGETCPSRVITEM_H

#include <QtNetwork>
#include <QListWidgetItem>
#include "myconfig.h"

class LS_PageTcpSrvItem : public QListWidgetItem
{
    LoProperty0(QTcpSocket*,  Tcp)
    LoProperty1(QHostAddress, LocalIP)
    LoProperty1(QHostAddress, RemoteIP)
    LoProperty0(quint16,      LocalPort)
    LoProperty0(quint16,      RemotePort)
    LoProperty1(QString,      LocalAddress)
    LoProperty1(QString,      RemoteAddress)

public:
    LS_PageTcpSrvItem(QTcpSocket *tcp, QListWidget *view = Q_NULLPTR);
    ~LS_PageTcpSrvItem();

private:
    void closeMe();

public:
//    void closeTcp();
    void sendData(const QByteArray &a);

private:
    bool m_isDel;
};

#endif // LS_PAGETCPSRVITEM_H
