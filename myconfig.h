#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QDebug>
#include <QMessageBox>
#include "lotools.h"

#define CFG_FILE             "Config.ini"
#define CFG_WND_MAXED        "Window/Maxed"
#define CFG_WND_RECT         "Window/Rect"
#define CFG_WND_LAN          "Window/Language"
#define CFG_TX_ATTACH_CR     "TX/Attach_CR"
#define CFG_TX_ATTACH_LF     "TX/Attach_LF"
#define CFG_TX_ATTACH_ZR     "TX/Attach_ZR"
#define CFG_TX_MODE          "TX/Mode"
#define CFG_TX_SEND2ALL      "TX/Send_to_All_Page"
#define CFG_TX_ENTER         "TX/Enter"
#define CFG_TX_AUTCLEAR      "TX/Aut_Clear"
#define CFG_TX_AUTSEND_VALUE "TX/Aut_Send_Value"
#define CFG_TX_AUTSEND_MODE  "TX/Aut_Send_Mode"
#define CFG_TX_TXT_SEND      "TX/Text_Send"
#define CFG_RX_SOCKET_PAGE   "RX/Socket_Page"

#define CFG_UDP_LOCAL_IP     "UDP/Local_IP"
#define CFG_UDP_LOCAL_PO     "UDP/Local_Port"
#define CFG_UDP_TARGET_IP    "UDP/Target_IP"
#define CFG_UDP_TARGET_PO    "UDP/Target_Port"
#define CFG_UDP_AUT_CLEAR    "UDP/Aut_Clear"
#define CFG_UDP_AUT_CLRVAL   "UDP/Aut_Clear_Value"
#define CFG_UDP_HEX          "UDP/Hex"
#define CFG_UDP_SAVE_LOG     "UDP/Save_Log"
#define CFG_UDP_LOG_FILE     "Log_UDP.txt"

#define CFG_TCP_LOCAL_IP     "TCP/Local_IP"
#define CFG_TCP_LOCAL_PO     "TCP/Local_Port"
#define CFG_TCP_TARGET_IP    "TCP/Target_IP"
#define CFG_TCP_TARGET_PO    "TCP/Target_Port"
#define CFG_TCP_AUT_CLEAR    "TCP/Aut_Clear"
#define CFG_TCP_AUT_CLRVAL   "TCP/Aut_Clear_Value"
#define CFG_TCP_HEX          "TCP/Hex"
#define CFG_TCP_SAVE_LOG     "TCP/Save_Log"
#define CFG_TCP_LOG_FILE     "Log_TCPClient.txt"

typedef enum {
    zhCN = 0,
    enUS
} ENUM_CFG_WND_LAN;

typedef enum {
    Text = 0,
    Hex,
    File
} ENUM_CFG_TX_MODE;

typedef enum {
    MS = 0,
    S,
    Min
} ENUM_CFG_TX_AUTSEND_MODE;

#endif // MYCONFIG_H

