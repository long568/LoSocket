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
#include <QTranslator>

#include "myconfig.h"
#include "ls_pageudp.h"
#include "ls_pagetcpclient.h"
#include "ls_pagetcpsrv.h"
#include "about.h"

#if defined(Q_OS_WIN32)
#   define CONF_FORMAT QSettings::IniFormat
#elif defined(Q_OS_MACOS)
#   define CONF_FORMAT QSettings::NativeFormat
#endif

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
    void onShowAbout();

private:
    Ui::LoSocket      *ui;
    QTimer            *m_timer;
    LS_PageUdp        *m_udp;
    LS_PageTcpClient  *m_tcpClient;
    LS_PageTcpSrv     *m_tcpSrv;
    QVector<LS_Page*>  m_pageList;
    QTranslator       *m_Translator;
};

#endif // LOSOCKET_H
