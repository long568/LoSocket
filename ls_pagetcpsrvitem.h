#ifndef LS_PAGETCPSRVITEM_H
#define LS_PAGETCPSRVITEM_H

#include <QtNetwork>
#include <QListWidgetItem>
#include "myconfig.h"

class LS_PageTcpSrvItem;

class LS_PageTcpSrvItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

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

    void sendData(const QByteArray &a);

signals:
    void sigReadyRead(LS_PageTcpSrvItem* item);
    void sigDisconnected(LS_PageTcpSrvItem* item);
    void sigError(LS_PageTcpSrvItem* item, QAbstractSocket::SocketError err);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError err);
};

#endif // LS_PAGETCPSRVITEM_H
