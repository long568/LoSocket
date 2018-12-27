#include "ls_pagetcpsrv.h"
#include "ui_ls_pagetcpsrv.h"

LS_PageTcpSrv::LS_PageTcpSrv(QWidget *parent) :
    LS_Page(parent),
    ui(new Ui::LS_PageTcpSrv),
    m_isCreated(false),
    m_srv(0)
{
    ui->setupUi(this);

    m_log = new QFile(CFG_TCPSRV_LOG_FILE);
    CFG_LOCAL_IP   = CFG_TCPSRV_LOCAL_IP;
    CFG_LOCAL_PO   = CFG_TCPSRV_LOCAL_PO;
    CFG_TARGET_IP  = CFG_TCPSRV_TARGET_IP;
    CFG_TARGET_PO  = CFG_TCPSRV_TARGET_PO;
    CFG_AUT_CLEAR  = CFG_TCPSRV_AUT_CLEAR;
    CFG_AUT_CLRVAL = CFG_TCPSRV_AUT_CLRVAL;
    CFG_HEX        = CFG_TCPSRV_HEX;
    CFG_SAVE_LOG   = CFG_TCPSRV_SAVE_LOG;
    ui_IPLocal     = ui->IPLocal;
    ui_PortLocal   = ui->PortLocal;
    ui_IPTarget    = 0;
    ui_PortTarget  = 0;
    ui_cbAutClear  = ui->cbAutClear;
    ui_sbAutClear  = ui->sbAutClear;
    ui_cbHex       = ui->cbHex;
    ui_cbSaveLog   = ui->cbSaveLog;
    ui_RxData      = ui->RxData;
    loadConfig();

    connect(ui->Clients, SIGNAL(currentRowChanged(int)), this, SLOT(onClientSelected()));
}

LS_PageTcpSrv::~LS_PageTcpSrv()
{
    if(m_isCreated) {
        ui->Clients->clear();
        m_srv->close();
        m_isCreated = false;
    }
    saveConfig();
    delete ui;
}

void LS_PageTcpSrv::sendData(QByteArray a)
{
    int cnt = ui->Clients->count();
    if(0 == cnt) return;
    if(m_isCreated) {
        QString s;
        if(ui->cbHex->isChecked()) {
            s = LoTools::BArray2Hex(a);
        } else {
            s = QString::fromUtf8(a);
        }
        LS_PageTcpSrvItem *item;
        if(ui->cbTxAll->isChecked()) {
            for(int i=0; i<cnt; i++) {
                item = static_cast<LS_PageTcpSrvItem*>(ui->Clients->item(i));
                ui->RxData->append("[" + item->RemoteAddress() + "] " + tr("TX"));
                LoTools::append(ui->RxData, s);
                item->sendData(a);
            }
        } else {
            item = static_cast<LS_PageTcpSrvItem*>(ui->Clients->currentItem());
            if(item) {
                ui->RxData->append("[" + item->RemoteAddress() + "] " + tr("TX"));
                LoTools::append(ui->RxData, s);
                item->sendData(a);
            }
        }
    }
}

void LS_PageTcpSrv::loadConfig()
{
    LS_Page::loadConfig();
    QSettings settings("long", "LoSocket");
    if(settings.value(CFG_TCPSRV_RX_ALL).isValid())
        ui->cbRxAll->setChecked(settings.value(CFG_TCPSRV_RX_ALL).toBool());
    if(settings.value(CFG_TCPSRV_TX_ALL).isValid())
        ui->cbTxAll->setChecked(settings.value(CFG_TCPSRV_TX_ALL).toBool());
    if(settings.value(CFG_TCPSRV_MSGLAY).isValid())
        ui->MsgLayout->restoreState(settings.value(CFG_TCPSRV_MSGLAY).toByteArray());
}

void LS_PageTcpSrv::saveConfig()
{
    LS_Page::saveConfig();
    QSettings settings("long", "LoSocket");
    settings.setValue(CFG_TCPSRV_RX_ALL, QVariant(ui->cbRxAll->isChecked()));
    settings.setValue(CFG_TCPSRV_TX_ALL, QVariant(ui->cbTxAll->isChecked()));
    settings.setValue(CFG_TCPSRV_MSGLAY, QVariant(ui->MsgLayout->saveState()));
}

void LS_PageTcpSrv::changeLanguage(int id)
{
    (void)id;
    ui->retranslateUi(this);
    if(m_isCreated)
        ui->bnCreate->setText(tr("Close"));
}

void LS_PageTcpSrv::closeServer()
{
    ui->Clients->clear();
    m_srv->close();
    m_srv->deleteLater();
    LoTools::append(ui->RxData, tr("[Local]") + " " + tr("TCP Server Closed"));
    ui->PortLocal->setEnabled(true);
    ui->IPLocal->setEnabled(true);
    ui->bnCreate->setText(tr("Create"));
    m_isCreated = false;
}

void LS_PageTcpSrv::onCreateServer()
{
    if(m_isCreated) {
        closeServer();
    } else {
        QHostAddress ip;
        QString ip_str = ui->IPLocal->currentText();
        if(ip_str.isEmpty()) {
            ip = QHostAddress::Any;
        } else {
            ip = QHostAddress(ip_str);
            if(ip.isNull()) {
                QMessageBox::critical(this, tr("Error"), tr("Invalid Local IP !"));
                return;
            }
        }
        m_srv = new QTcpServer(this);
        if(!m_srv->listen(ip, ui->PortLocal->value())) {
            QMessageBox::critical(this, tr("Error"), tr("TCP Server Listen Failed !"));
            return;
        }
        connect(m_srv, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        connect(m_srv, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onAcceptError()));
        LoTools::append(ui->RxData, tr("[Local]") + " " + tr("Listening ..."));
        ui->PortLocal->setEnabled(false);
        ui->IPLocal->setEnabled(false);
        ui->bnCreate->setText(tr("Close"));
        m_isCreated = true;
    }
}

void LS_PageTcpSrv::onCloseClient()
{
    if(!ui->IPTarget->text().isEmpty()) {
        LS_PageTcpSrvItem *item = static_cast<LS_PageTcpSrvItem*>(ui->Clients->currentItem());
        onClientDisconnected(item);
    }
}

void LS_PageTcpSrv::onCloseAll()
{
    if(m_isCreated) {
        LoTools::append(ui->RxData, tr("[Local]") + " " + tr("All Clients Closed"));
    }
}

void LS_PageTcpSrv::onClientSelected()
{
    LS_PageTcpSrvItem* item = static_cast<LS_PageTcpSrvItem*>(ui->Clients->currentItem());
    if(item) {
        ui->IPTarget->setText(item->RemoteIP().toString());
        ui->PortTarget->setValue(item->RemotePort());
    } else {
        ui->IPTarget->clear();
        ui->PortTarget->setValue(0);
    }
}

void LS_PageTcpSrv::onAcceptError()
{
    ui->RxData->append(tr("[Local]") + " " + tr("Client Accept Error"));
    LoTools::append(ui->RxData, m_srv->errorString());
}

void LS_PageTcpSrv::onNewConnection()
{
    while(m_srv->hasPendingConnections()) {
        QTcpSocket* clt = m_srv->nextPendingConnection();
        LS_PageTcpSrvItem* item = new LS_PageTcpSrvItem(clt);
        ui->Clients->addItem(item);
        connect(item, SIGNAL(sigReadyRead(LS_PageTcpSrvItem*)), this, SLOT(onClientReadyRead(LS_PageTcpSrvItem*)));
        connect(item, SIGNAL(sigDisconnected(LS_PageTcpSrvItem*)), this, SLOT(onClientDisconnected(LS_PageTcpSrvItem*)));
        connect(item, SIGNAL(sigError(LS_PageTcpSrvItem*,QAbstractSocket::SocketError)),
                this, SLOT(onClientError(LS_PageTcpSrvItem*,QAbstractSocket::SocketError)));
        LoTools::append(ui->RxData, "[" + item->RemoteAddress() + "] " + tr("Connected"));
    }
}

void LS_PageTcpSrv::onClientReadyRead(LS_PageTcpSrvItem *item)
{
    QByteArray a = item->Tcp()->readAll();
    if(ui->cbRxAll->isChecked() || (item == static_cast<LS_PageTcpSrvItem*>(ui->Clients->currentItem()))) {
        ui->RxData->append("[" + item->RemoteAddress() + "] " + tr("RX"));
        QString s;
        if(ui->cbHex->isChecked()) {
            s = LoTools::BArray2Hex(a);
        } else {
            s = QString::fromUtf8(a);
        }
        LoTools::append(ui->RxData, s);
    }
}

void LS_PageTcpSrv::onClientDisconnected(LS_PageTcpSrvItem *item)
{
    LoTools::append(ui->RxData, "[" + item->RemoteAddress() + "] " + tr("Disconnected"));
    item->deleteLater();
}

void LS_PageTcpSrv::onClientError(LS_PageTcpSrvItem *item, QAbstractSocket::SocketError error)
{
    (void)error;
    ui->RxData->append("[" + item->RemoteAddress() + "] " + tr("Error/Disconnected"));
    LoTools::append(ui->RxData, item->Tcp()->errorString());
    item->deleteLater();
}
