#include "ls_pagetcpsrvitem.h"

LS_PageTcpSrvItem::LS_PageTcpSrvItem(QTcpSocket *tcp, QListWidget *view) :
    QListWidgetItem(view, UserType),
    m_isDel(false)
{
    setTcp          (tcp);
    setLocalIP      (tcp->localAddress());
    setLocalPort    (tcp->localPort());
    setLocalAddress (LocalIP().toString() + ":" + QString("%1").arg(LocalPort()));
    setRemoteIP     (tcp->peerAddress());
    setRemotePort   (tcp->peerPort());
    setRemoteAddress(RemoteIP().toString() + ":" + QString("%1").arg(RemotePort()));
    setText("["+RemoteAddress()+"]|("+LocalAddress()+")");
}

LS_PageTcpSrvItem::~LS_PageTcpSrvItem() {
    closeMe();
}

void LS_PageTcpSrvItem::closeMe()
{
    if(!m_isDel) {
        Tcp()->close();
        Tcp()->deleteLater();
        m_isDel = true;
    }
}

//void LS_PageTcpSrvItem::closeTcp()
//{
//    closeMe();
//}

void LS_PageTcpSrvItem::sendData(const QByteArray &a)
{
    Tcp()->write(a);
}
