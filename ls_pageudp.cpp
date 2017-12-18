#include "ls_pageudp.h"
#include "ui_ls_pageudp.h"

LS_PageUdp::LS_PageUdp(QWidget *parent) :
    LS_Page(parent),
    ui(new Ui::LS_PageUdp)
{
    ui->setupUi(this);

    m_log = new QFile(CFG_UDP_LOG_FILE);
    CFG_LOCAL_IP   = CFG_UDP_LOCAL_IP;
    CFG_LOCAL_PO   = CFG_UDP_LOCAL_PO;
    CFG_TARGET_IP  = CFG_UDP_TARGET_IP;
    CFG_TARGET_PO  = CFG_UDP_TARGET_PO;
    CFG_AUT_CLEAR  = CFG_UDP_AUT_CLEAR;
    CFG_AUT_CLRVAL = CFG_UDP_AUT_CLRVAL;
    CFG_HEX        = CFG_UDP_HEX;
    CFG_SAVE_LOG   = CFG_UDP_SAVE_LOG;
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

    m_isBind = false;
    m_udp    = 0;

}

LS_PageUdp::~LS_PageUdp()
{
    if(m_isBind) {
        disconnect(m_udp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        m_udp->close();
        m_udp->deleteLater();
    }
    saveConfig();
    delete ui;
}

void LS_PageUdp::sendData(QByteArray a)
{
    if(m_isBind) {
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
        m_udp->writeDatagram(a, ip, port);
    }
}

void LS_PageUdp::changeLanguage(int id)
{
    (void)id;
    ui->retranslateUi(this);
    if(m_isBind)
        ui->bnBind->setText(tr("Disbind"));
}

void LS_PageUdp::onBindUdp()
{
    if(m_isBind) {
        disconnect(m_udp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        m_udp->close();
        m_udp->deleteLater();
        ui->bnBind->setText(tr("Bind"));
        ui->IPLocal->setEnabled(true);
        ui->PortLocal->setEnabled(true);
        ui->IPTarget->setEnabled(false);
        ui->PortTarget->setEnabled(false);
        m_isBind = false;
    } else {
        QHostAddress ip   = QHostAddress(ui->IPLocal->currentText());
        quint16      port = (quint16)ui->PortLocal->value();
        if(ip.isNull()) {
            QMessageBox::critical(this, tr("Error"), tr("Invalid Local IP !"));
            return;
        }
        m_udp = new QUdpSocket();
        if(m_udp && m_udp->bind(ip, port)) {
            connect(m_udp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            ui->bnBind->setText(tr("Unbind"));
            ui->IPLocal->setEnabled(false);
            ui->PortLocal->setEnabled(false);
            ui->IPTarget->setEnabled(true);
            ui->PortTarget->setEnabled(true);
            m_isBind = true;
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Socket Bound Failed !"));
        }
    }
}

void LS_PageUdp::onReadyRead()
{
    while (m_udp->hasPendingDatagrams()) {
        QHostAddress ip;
        quint16      port;
        long cnt     = m_udp->pendingDatagramSize();
        char *buffer = (char*)malloc(cnt);
        if(!buffer) continue;
        cnt = m_udp->readDatagram(buffer, cnt, &ip, &port);
        if(0 > cnt) continue;
        ui->RxData->append("[" + ip.toString() + QString(":%1] ").arg(port) + tr("RX"));
        QString s;
        QByteArray a = QByteArray(buffer, cnt);
        if(ui->cbHex->isChecked()) {
            s = LoTools::BArray2Hex(a);
        } else {
            s = QString::fromUtf8(a);
        }
        LoTools::append(ui->RxData, s);
    }
}
