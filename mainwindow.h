#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLibrary>
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>

typedef bool (*SO00)(QString PortStr,qint32 BaudRate,quint8 Parity,quint8 DataBits,quint8 StopBits);
typedef void (*SO01)();
typedef void (*SO02)(quint16 nTimeOuts);
typedef void (*SO03)(quint8 nComType,quint16 nResAddr,quint8 nBytes,quint8 *pResData);
typedef void (*SO04)(quint8 nStatus);
typedef quint8 (*SO05)();
typedef quint8 *(*SO06)();

//COM_TYPE
#define HANDSHAKE           0			//握手
#define READBIT             1			//读位
#define SETBIT              2			//置位
#define RSTBIT              3			//复位
#define READBYTE            4			//读字节
#define WRITEBYTE           5			//写字节

//COMM_STATUS
#define FREE                0x00		//空闲
#define HANDSHAKE_SUC       0x01        //握手成功
#define SETBIT_SUC          0x01		//置位成功
#define RSTBIT_SUC          0x01		//复位成功
#define WRITEBYTE_SUC       0x01        //写字节成功
#define READBYTE_SUC        0x02        //读字成功
#define REVERR              0xA0		//接收错误
#define VREVERR             0xA1		//接收校验错误
#define OVERTIME            0xA2		//通讯超时
#define REVLENERR           0xA3		//长度接收错误
#define COMMING             0xFF		//正在通讯

//PLC_RES
#define RES_NUM     8
#define RES_X       0
#define RES_Y       1
#define RES_M0      2
#define RES_M8      3
#define RES_S       4
#define RES_D0      5
#define RES_D8      6
#define RES_R       7

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    bool m_bUartOpen;
    bool m_bNowComm;
    QLibrary m_PlcCommLib;
    quint16 m_ResLogicAddr[RES_NUM][2];
    QTimer m_Timer;
    quint8 m_MoResType;
    quint8 m_MoResBitOffset;
public:
    //so export
    SO00 OpenUartPort;
    SO01 CloseUartPort;
    SO02 SetTimeouts;
    SO03 ComToPlc;
    SO04 SetCommStatus;
    SO05 GetCommStatus;
    SO06 GetRevData;
    //
    bool LoadSo();
    void InitResLogicAddr();
    bool IsBitRes(QString strRes);
    bool IsByteRes(QString strRes);
    bool CheckRes(QString strRes,quint16 *pResAddr,quint8 *pResBytes,bool bWrite);
    quint8 GetBitResOffset(QString strRes);
private slots:
    void on_OpenUartBtn_clicked();

    void on_SetOnBtn_clicked();

    void on_SetOffBtn_clicked();

    void on_SetValueBtn_clicked();

    void on_pBtnTest_clicked();

    void on_MonitorBtn_clicked();

    void OnTimer();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
