#include "ls_pagetcpsrvitem.h"

LS_PageTcpSrvItem::LS_PageTcpSrvItem(QTcpSocket *tcp, QListWidget *view) :
    QObject(view),
    QListWidgetItem(view, UserType)
{
    tcp->setParent(this);
    setTcp          (tcp);
    setLocalIP      (tcp->localAddress());
    setLocalPort    (tcp->localPort());
    setLocalAddress (LocalIP().toString() + ":" + QString("%1").arg(LocalPort()));
    setRemoteIP     (tcp->peerAddress());
    setRemotePort   (tcp->peerPort());
    setRemoteAddress(RemoteIP().toString() + ":" + QString("%1").arg(RemotePort()));
    setText("["+RemoteAddress()+"]|("+LocalAddress()+")");
    connect(Tcp(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(Tcp(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(Tcp(), SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));
}

LS_PageTcpSrvItem::~LS_PageTcpSrvItem()
{
    disconnect(Tcp(), SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    disconnect(Tcp(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    disconnect(Tcp(), SIGNAL(error(QAbstractSocket::SocketError)),
               this, SLOT(onError(QAbstractSocket::SocketError)));
    Tcp()->close();
}

void LS_PageTcpSrvItem::sendData(const QByteArray &a)
{ Tcp()->write(a); }

void LS_PageTcpSrvItem::onReadyRead()
{ emit sigReadyRead(this); }

void LS_PageTcpSrvItem::onDisconnected()
{ emit sigDisconnected(this); }

void LS_PageTcpSrvItem::onError(QAbstractSocket::SocketError err)
{ emit sigError(this, err); }
