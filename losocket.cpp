#include "losocket.h"
#include "ui_losocket.h"

LoSocket::LoSocket(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoSocket)
{
// Default Config
    ui->setupUi(this);
    setMinimumSize(800, 600);
    ui->rbngType->setId(ui->rbnText, 0);
    ui->rbngType->setId(ui->rbnHex,  1);
    ui->rbngType->setId(ui->rbnFile, 2);
    ui->rbngTime->setId(ui->rbnMS,  0);
    ui->rbngTime->setId(ui->rbnS,   1);
    ui->rbngTime->setId(ui->rbnMin, 2);
    auto ag = new QActionGroup(ui->menuLanguage);
    ag->addAction(ui->actionChinese);
    ag->addAction(ui->actionEnglish);
    ag->setExclusive(true);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onDataSent()));

// Create Sub Widget
    m_pageList.clear();
    m_udp       = new LS_PageUdp();
    m_tcpClient = new LS_PageTcpClient();
    m_tcpSrv    = new LS_PageTcpSrv();
    ui->SocketPages->addTab(m_udp, "UDP");
    ui->SocketPages->addTab(m_tcpClient, "TCP Client");
    ui->SocketPages->addTab(m_tcpSrv, "TCP Server");
    m_pageList.push_back(m_udp);
    m_pageList.push_back(m_tcpClient);
    m_pageList.push_back(m_tcpSrv);

// User Config
    QFile f(CFG_FILE);
    if(f.exists()) {
        QSettings settings(CFG_FILE, QSettings::IniFormat);
        if(settings.value(CFG_WND_RECT).isValid()) {
            setGeometry(settings.value(CFG_WND_RECT).toRect());
        }
        if(settings.value(CFG_WND_MAXED).isValid() &&
           settings.value(CFG_WND_MAXED).toBool()) {
            setWindowState(Qt::WindowMaximized);
        }
        if(settings.value(CFG_WND_LAN).isValid()) {
            switch(settings.value(CFG_WND_LAN).toInt()) {
            case enUS: ui->actionEnglish->setChecked(true); break;
            default:   ui->actionChinese->setChecked(true); break;
            }
        }
        if(settings.value(CFG_TX_ATTACH_CR).isValid())
            ui->cbAttachCR->setChecked(settings.value(CFG_TX_ATTACH_CR).toBool());
        if(settings.value(CFG_TX_ATTACH_LF).isValid())
            ui->cbAttachLF->setChecked(settings.value(CFG_TX_ATTACH_LF).toBool());
        if(settings.value(CFG_TX_ATTACH_ZR).isValid())
            ui->cbAttachZR->setChecked(settings.value(CFG_TX_ATTACH_ZR).toBool());
        if(settings.value(CFG_TX_MODE).isValid()) {
            switch (settings.value(CFG_TX_MODE).toInt()) {
            case 0: ui->rbnText->setChecked(true); break;
            case 1: ui->rbnHex->setChecked(true);  break;
            case 2: ui->rbnFile->setChecked(true); break;
            default: break;
            }
        }
        if(settings.value(CFG_TX_SEND2ALL).isValid())
            ui->cbSend2All->setChecked(settings.value(CFG_TX_SEND2ALL).toBool());
        if(settings.value(CFG_TX_ENTER).isValid())
            ui->cbEnter->setChecked(settings.value(CFG_TX_ENTER).toBool());
        if(settings.value(CFG_TX_AUTCLEAR).isValid())
            ui->cbAutClear->setChecked(settings.value(CFG_TX_AUTCLEAR).toBool());
        if(settings.value(CFG_TX_AUTSEND_VALUE).isValid())
            ui->sbAutSend->setValue(settings.value(CFG_TX_AUTSEND_VALUE).toInt());
        if(settings.value(CFG_TX_AUTSEND_MODE).isValid()) {
            switch (settings.value(CFG_TX_AUTSEND_MODE).toInt()) {
            case 0: ui->rbnMS->setChecked(true);  break;
            case 1: ui->rbnS->setChecked(true);   break;
            case 2: ui->rbnMin->setChecked(true); break;
            default: break;
            }
        }
        if(settings.value(CFG_TX_TXT_SEND).isValid())
            ui->TxData->setText(settings.value(CFG_TX_TXT_SEND).toString());
        if(settings.value(CFG_RX_SOCKET_PAGE).isValid())
            ui->SocketPages->setCurrentIndex(settings.value(CFG_RX_SOCKET_PAGE).toInt());
    }

// Preconfig
    onLanguageChanged();
}

LoSocket::~LoSocket()
{
// Save Config
    QSettings settings(CFG_FILE, QSettings::IniFormat);
    if(isMaximized()) {
        settings.setValue(CFG_WND_MAXED, QVariant(true));
    } else {
        settings.setValue(CFG_WND_MAXED, QVariant(false));
        settings.setValue(CFG_WND_RECT,  QVariant(geometry()));
    }
    if     (ui->actionChinese->isChecked()) settings.setValue(CFG_WND_LAN, QVariant(zhCN));
    else if(ui->actionEnglish->isChecked()) settings.setValue(CFG_WND_LAN, QVariant(enUS));
    if(ui->cbAttachCR->isChecked()) settings.setValue(CFG_TX_ATTACH_CR, QVariant(true));
    else                            settings.setValue(CFG_TX_ATTACH_CR, QVariant(false));
    if(ui->cbAttachLF->isChecked()) settings.setValue(CFG_TX_ATTACH_LF, QVariant(true));
    else                            settings.setValue(CFG_TX_ATTACH_LF, QVariant(false));
    if(ui->cbAttachZR->isChecked()) settings.setValue(CFG_TX_ATTACH_ZR, QVariant(true));
    else                            settings.setValue(CFG_TX_ATTACH_ZR, QVariant(false));
    settings.setValue(CFG_TX_MODE,          QVariant(ui->rbngType->checkedId()));
    settings.setValue(CFG_TX_SEND2ALL,      QVariant(ui->cbSend2All->isChecked()));
    settings.setValue(CFG_TX_ENTER,         QVariant(ui->cbEnter->isChecked()));
    settings.setValue(CFG_TX_AUTCLEAR,      QVariant(ui->cbAutClear->isChecked()));
    settings.setValue(CFG_TX_AUTSEND_VALUE, QVariant(ui->sbAutSend->value()));
    settings.setValue(CFG_TX_AUTSEND_MODE,  QVariant(ui->rbngTime->checkedId()));
    settings.setValue(CFG_TX_TXT_SEND,      QVariant(ui->TxData->text()));
    settings.setValue(CFG_RX_SOCKET_PAGE,   QVariant(ui->SocketPages->currentIndex()));
    delete ui;
}

void LoSocket::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if(ui->cbEnter->isChecked()) {
            onDataSent();
            if(ui->cbAutClear->isChecked())
                ui->TxData->clear();
        }
        break;
    default:
        break;
    }
}

void LoSocket::onLanguageChanged()
{
    int id;
    if(ui->actionEnglish->isChecked()) {
        id = enUS;
    } else {
        id = zhCN;
    }
    foreach(LS_Page *page, m_pageList)
        page->changeLanguage(id);
}

void LoSocket::onAboutShown()
{
}

void LoSocket::onDataSent()
{
    QByteArray d;
    switch(ui->rbngType->checkedId()) {
        case 1: {
            d = LoTools::Hex2BArray(ui->TxData->text());
            break;
        }
        case 2: {
            QFile f(ui->TxData->text());
            if((!f.exists()) || (!f.open(QFile::ReadOnly)))
                return;
            d = f.readAll();
            f.close();
            break;
        }
        default: {
            d = ui->TxData->text().toUtf8();
            break;
        }
    }
    if(ui->cbAttachCR->isChecked()) d.append('\r');
    if(ui->cbAttachLF->isChecked()) d.append('\n');
    if(ui->cbAttachZR->isChecked()) d.append('\0');
    if(ui->cbSend2All->isChecked()) {
        foreach(LS_Page *page, m_pageList)
            page->sendData(d);
    } else {
        ((LS_Page*)ui->SocketPages->currentWidget())->sendData(d);
    }
}

void LoSocket::onAutSendChanged(bool isBegin)
{
    m_timer->stop();
    if(isBegin) {
        int t = ui->sbAutSend->value();
        if(ui->rbnS->isChecked())
            t *= 1000;
        else if(ui->rbnMin->isChecked())
            t *= 1000 * 60;
        m_timer->start(t);
    }
}

void LoSocket::onTxtSendConverted(bool isHex)
{
    QString ls;
    QString rs = ui->TxData->text();
    if(isHex) ls = LoTools::Str2Hex(rs);
    else      ls = LoTools::Hex2Str(rs);
    ui->TxData->setText(ls);
}

void LoSocket::onFileOpened()
{
    QString path;
    QString f_name = ui->TxData->text();
    QFile f(f_name);
    if(f.exists()) path = f_name.left(f_name.lastIndexOf('/'));
    else           path = ".";
    f_name = QFileDialog::getOpenFileName(this, tr("Select File"), path);
    if(!f_name.isEmpty())
        ui->TxData->setText(f_name);
}
