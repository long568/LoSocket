#include "ls_page.h"

LS_Page::LS_Page(QWidget *parent) : QWidget(parent)
{
    m_log   = 0;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onLogCleared()));
}

LS_Page::~LS_Page()
{
    closeLog();
}

void LS_Page::loadConfig()
{
    QStringList ip_list = LoTools::getLocalIPv4List();
    ip_list.push_back("0.0.0.0");
    ip_list.push_back("255.255.255.255");
    ui_IPLocal->addItems(ip_list);
    if(ui_IPTarget)
        ui_IPTarget->addItems(ip_list);

    QSettings settings("long", "LoSocket");
    if(settings.value(CFG_LOCAL_IP).isValid())
        ui_IPLocal->setCurrentText(settings.value(CFG_LOCAL_IP).toString());
    if(settings.value(CFG_LOCAL_PO).isValid())
        ui_PortLocal->setValue(settings.value(CFG_LOCAL_PO).toInt());
    if(ui_IPTarget && settings.value(CFG_TARGET_IP).isValid())
        ui_IPTarget->setCurrentText(settings.value(CFG_TARGET_IP).toString());
    if(ui_PortTarget && settings.value(CFG_TARGET_PO).isValid())
        ui_PortTarget->setValue(settings.value(CFG_TARGET_PO).toInt());
    if(settings.value(CFG_AUT_CLEAR).isValid())
        ui_cbAutClear->setChecked(settings.value(CFG_AUT_CLEAR).toBool());
    if(settings.value(CFG_AUT_CLRVAL).isValid())
        ui_sbAutClear->setValue(settings.value(CFG_AUT_CLRVAL).toInt());
    if(settings.value(CFG_HEX).isValid())
        ui_cbHex->setChecked(settings.value(CFG_HEX).toBool());
    if(settings.value(CFG_SAVE_LOG).isValid())
        ui_cbSaveLog->setChecked(settings.value(CFG_SAVE_LOG).toBool());

    onSaveChanged(ui_cbSaveLog->isChecked());
    onAutClearChanged(ui_cbAutClear->isChecked());
}

void LS_Page::saveConfig()
{
    QSettings settings("long", "LoSocket");
    settings.setValue(CFG_LOCAL_IP,   QVariant(ui_IPLocal->currentText()));
    settings.setValue(CFG_LOCAL_PO,   QVariant(ui_PortLocal->value()));
    if(ui_IPTarget && ui_PortTarget) {
        settings.setValue(CFG_TARGET_IP, QVariant(ui_IPTarget->currentText()));
        settings.setValue(CFG_TARGET_PO, QVariant(ui_PortTarget->value()));
    }
    settings.setValue(CFG_AUT_CLEAR,  QVariant(ui_cbAutClear->isChecked()));
    settings.setValue(CFG_AUT_CLRVAL, QVariant(ui_sbAutClear->value()));
    settings.setValue(CFG_HEX,        QVariant(ui_cbHex->isChecked()));
    settings.setValue(CFG_SAVE_LOG,   QVariant(ui_cbSaveLog->isChecked()));
    saveLog();
}

void LS_Page::openLog()
{
    if(m_log)
        m_log->open(QFile::WriteOnly);
}

void LS_Page::saveLog()
{
    if(ui_cbSaveLog->isChecked() && m_log && m_log->isWritable())
        m_log->write(ui_RxData->toPlainText().toUtf8());
}

void LS_Page::closeLog()
{
    if(m_log && m_log->isOpen())
        m_log->close();
}

void LS_Page::onLogCleared()
{
    saveLog();
    clearLog();
}

void LS_Page::onSaveChanged(bool isBegin)
{
    closeLog();
    if(isBegin)
        openLog();
}

void LS_Page::onAutClearChanged(bool isBegin)
{
    m_timer->stop();
    if(isBegin)
        m_timer->start(ui_sbAutClear->value() * 1000 * 60);
}
