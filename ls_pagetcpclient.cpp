#include "ls_pagetcpclient.h"
#include "ui_ls_pagetcpclient.h"

LS_PageTcpClient::LS_PageTcpClient(QWidget *parent) :
    LS_Page(parent),
    ui(new Ui::LS_PageTcpClient)
{
    ui->setupUi(this);

    m_log = new QFile(CFG_TCP_LOG_FILE);
    CFG_LOCAL_IP   = CFG_TCP_LOCAL_IP;
    CFG_LOCAL_PO   = CFG_TCP_LOCAL_PO;
    CFG_TARGET_IP  = CFG_TCP_TARGET_IP;
    CFG_TARGET_PO  = CFG_TCP_TARGET_PO;
    CFG_AUT_CLEAR  = CFG_TCP_AUT_CLEAR;
    CFG_AUT_CLRVAL = CFG_TCP_AUT_CLRVAL;
    CFG_HEX        = CFG_TCP_HEX;
    CFG_SAVE_LOG   = CFG_TCP_SAVE_LOG;
    ui_IPLocal     = ui->IPLocal;
    ui_PortLocal   = ui->PortLocal;
    ui_IPTarget    = ui->IPTarget;
    ui_PortTarget  = ui->PortTarget;
    ui_cbAutClear  = ui->cbAutClear;
    ui_sbAutClear  = ui->sbAutClear;
    ui_cbHex       = ui->cbHex;
    ui_cbSaveLog   = ui->cbSaveLog;
    ui_RxData      = ui->RxData;
    loadConfig();

    m_tcp       = 0;
    m_connState = UNCONNECT;
}

LS_PageTcpClient::~LS_PageTcpClient()
{
    if(m_connState) {
        m_tcp->close();
        m_tcp->deleteLater();
    }
    saveConfig();
    delete ui;
}

void LS_PageTcpClient::sendData(QByteArray a)
{
    if(m_connState == CONNECTED) {
        QHostAddress ip   = QHostAddress(ui->IPTarget->currentText());
        quint16      port = ui->PortTarget->value();
        if(ip.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid Target IP !"));
            return;
        }
        ui->RxData->append("[" + ip.toString() + QString(":%1] ").arg(port) + tr("TX"));
        QString s;
        if(ui->cbHex->isChecked()) {
            s = LoTools::BArray2Hex(a);
        } else {
            s = QString::fromUtf8(a);
        }
        LoTools::append(ui->RxData, s);
        m_tcp->write(a);
    }
}

void LS_PageTcpClient::changeLanguage(int id)
{
    (void)id;
    ui->retranslateUi(this);
    if(m_connState)
        ui->bnConnect->setText(tr("Disconnect"));
}

void LS_PageTcpClient::closeTCP()
{
    disconnect(m_tcp, SIGNAL(connected()),    this, SLOT(onConnected()));
    disconnect(m_tcp, SIGNAL(readyRead()),    this, SLOT(onReadyRead()));
    disconnect(m_tcp, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    disconnect(m_tcp, SIGNAL(error(QAbstractSocket::SocketError)),
               this, SLOT(onError(QAbstractSocket::SocketError)));
    m_tcp->close();
    m_tcp->deleteLater();
    ui->IPTarget->setEnabled(true);
    ui->PortTarget->setEnabled(true);
    ui->IPLocal->setEnabled(true);
    ui->PortLocal->setEnabled(true);
    ui->bnConnect->setText(tr("Connect"));
    LoTools::append(ui->RxData, tr("[Local]") + " " + tr("Disconnected"));
    m_connState = UNCONNECT;
}

void LS_PageTcpClient::onConnectClicked()
{
    if(m_connState) {
        closeTCP();
    } else {
        QHostAddress ip = QHostAddress(ui->IPTarget->currentText());
        if(ip.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid Target IP !"));
            return;
        }
        QHostAddress local_ip = QHostAddress(ui->IPLocal->currentText());
        if(local_ip.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid Local IP !"));
            return;
        }
        m_tcp = new QTcpSocket();
        if(m_tcp && m_tcp->bind(local_ip, ui->PortLocal->value())) {
            quint16 port = ui->PortTarget->value();
            connect(m_tcp, SIGNAL(connected()),    this, SLOT(onConnected()));
            connect(m_tcp, SIGNAL(readyRead()),    this, SLOT(onReadyRead()));
            connect(m_tcp, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
            connect(m_tcp, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(onError(QAbstractSocket::SocketError)));
            ui->IPTarget->setEnabled(false);
            ui->PortTarget->setEnabled(false);
            ui->IPLocal->setEnabled(false);
            ui->PortLocal->setEnabled(false);
            ui->bnConnect->setText(tr("Disconnect"));
            m_tcp->connectToHost(ip, port);
            LoTools::append(ui->RxData, tr("[Local]") + " " + tr("Connecting ..."));
            m_connState = CONNECTING;
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Bind Local IP Failed !"));
            delete m_tcp;
            m_tcp = NULL;
        }
    }
}

void LS_PageTcpClient::onConnected()
{
    LoTools::append(ui->RxData, "[" + m_tcp->peerAddress().toString() + QString(":%1] ").arg(m_tcp->peerPort()) + tr("Connected"));
    m_connState = CONNECTED;
}

void LS_PageTcpClient::onReadyRead()
{
    ui->RxData->append("[" + m_tcp->peerAddress().toString() + QString(":%1] ").arg(m_tcp->peerPort()) + tr("RX"));
    QString s;
    QByteArray a = m_tcp->readAll();
    if(ui->cbHex->isChecked()) {
        s = LoTools::BArray2Hex(a);
    } else {
        s = QString::fromUtf8(a);
    }
    LoTools::append(ui->RxData, s);
}

void LS_PageTcpClient::onDisconnected()
{
    if(m_connState)
        closeTCP();
}

void LS_PageTcpClient::onError(QAbstractSocket::SocketError error)
{
    (void)error;
    if(m_connState) {
        ui->RxData->append(tr("[Local]") + " " + tr("Error"));
        LoTools::append(ui->RxData, m_tcp->errorString());
        closeTCP();
    }
}
