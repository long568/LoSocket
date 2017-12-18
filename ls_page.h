#ifndef LS_PAGE_H
#define LS_PAGE_H

#include <QFile>
#include <QWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QSettings>
#include <QVariant>
#include <QSpinBox>
#include <QComboBox>
#include <QTimer>
#include <QtNetwork>

#include "myconfig.h"

class LS_Page : public QWidget
{
    Q_OBJECT

protected:
    explicit LS_Page(QWidget *parent = nullptr);
    virtual ~LS_Page();

public:
    virtual void sendData(QByteArray a) = 0;
    virtual void changeLanguage(int id) = 0;

protected:
    virtual void loadConfig();
    virtual void saveConfig();

    virtual void openLog();
    virtual void saveLog();
    virtual void closeLog();

signals:
    void clearLog();

protected slots:
    void onLogCleared();
    void onSaveChanged(bool isBegin);
    void onAutClearChanged(bool isBegin);

protected:
    QString CFG_LOCAL_IP;
    QString CFG_LOCAL_PO;
    QString CFG_TARGET_IP;
    QString CFG_TARGET_PO;
    QString CFG_AUT_CLEAR;
    QString CFG_AUT_CLRVAL;
    QString CFG_HEX;
    QString CFG_SAVE_LOG;

    QTimer    *m_timer;
    QFile     *m_log;

    QComboBox *ui_IPLocal;
    QSpinBox  *ui_PortLocal;
    QComboBox *ui_IPTarget;
    QSpinBox  *ui_PortTarget;
    QCheckBox *ui_cbAutClear;
    QSpinBox  *ui_sbAutClear;
    QCheckBox *ui_cbHex;
    QCheckBox *ui_cbSaveLog;
    QTextEdit *ui_RxData;
};

#endif // LS_PAGE_H
