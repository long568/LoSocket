#ifndef LOSOCKET_H
#define LOSOCKET_H

#include <QMainWindow>
#include <QSettings>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QVector>
#include <QTimer>
#include <QKeyEvent>
#include <QFileDialog>

#include "myconfig.h"
#include "ls_pageudp.h"
#include "ls_pagetcpclient.h"
#include "ls_pagetcpsrv.h"

namespace Ui {
class LoSocket;
}

class LoSocket : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoSocket(QWidget *parent = 0);
    ~LoSocket();

    virtual void keyPressEvent(QKeyEvent *event) override;

public slots:
    void onLanguageChanged();
    void onAboutShown();
    void onDataSent();
    void onAutSendChanged(bool isBegin);
    void onTxtSendConverted(bool isHex);
    void onFileOpened();

private:
    Ui::LoSocket      *ui;
    QTimer            *m_timer;
    LS_PageUdp        *m_udp;
    LS_PageTcpClient  *m_tcpClient;
    LS_PageTcpSrv     *m_tcpSrv;
    QVector<LS_Page*>  m_pageList;
};

#endif // LOSOCKET_H
