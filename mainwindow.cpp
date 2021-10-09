#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        ui->COMCombo->addItem(info.portName());
    }
    ui->TypeMode0->setChecked(true);
    ui->TypeMode1->setChecked(false);


    if(!LoadSo())
    {
        QMessageBox::about(this,"提示","so文件加载失败!");
    }
    InitResLogicAddr();
    m_bUartOpen = false;
    m_bNowComm = false;
    connect(&m_Timer,&QTimer::timeout,this,&MainWindow::OnTimer);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_PlcCommLib.isLoaded())
    {
        CloseUartPort();
        m_PlcCommLib.unload();
    }
}

bool MainWindow::LoadSo()
{
    m_PlcCommLib.setFileName("./PlcComm.dll");
    if(m_PlcCommLib.load())
    {
        OpenUartPort = (SO00)m_PlcCommLib.resolve("OpenUartPort");
        CloseUartPort = (SO01)m_PlcCommLib.resolve("CloseUartPort");
        SetTimeouts = (SO02)m_PlcCommLib.resolve("SetTimeouts");
        ComToPlc = (SO03)m_PlcCommLib.resolve("ComToPlc");
        SetCommStatus = (SO04)m_PlcCommLib.resolve("SetCommStatus");
        GetCommStatus = (SO05)m_PlcCommLib.resolve("GetCommStatus");
        GetRevData = (SO06)m_PlcCommLib.resolve("GetRevData");
        SetTimeouts(5000);
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::InitResLogicAddr()
{
    //固定填写,后期通过文件读取
    //BIT RES
    m_ResLogicAddr[RES_X][0] = 0;
    m_ResLogicAddr[RES_X][1] = 31;
    m_ResLogicAddr[RES_Y][0] = 100;
    m_ResLogicAddr[RES_Y][1] = 131;
    m_ResLogicAddr[RES_M0][0] = 200;
    m_ResLogicAddr[RES_M0][1] = 583;
    m_ResLogicAddr[RES_M8][0] = 1300;
    m_ResLogicAddr[RES_M8][1] = 1331;
    m_ResLogicAddr[RES_S][0] = 1400;
    m_ResLogicAddr[RES_S][1] = 1527;
    //BYTE RES
    m_ResLogicAddr[RES_D0][0] = 4096;
    m_ResLogicAddr[RES_D0][1] = 20095;
    m_ResLogicAddr[RES_D8][0] = 20096;
    m_ResLogicAddr[RES_D8][1] = 20607;
    m_ResLogicAddr[RES_R][0] = 24048;
    m_ResLogicAddr[RES_R][1] = 32047;
}

void MainWindow::on_OpenUartBtn_clicked()
{
    QString strPort = ui->COMCombo->currentText();
    if(OpenUartPort(strPort,9600,2,7,1))
    {
        m_bUartOpen = true;
        QMessageBox::about(this,"提示","串口打开成功!");
    }
    else
    {
        m_bUartOpen = false;
        QMessageBox::about(this,"提示","串口打开失败!");
    }
}

bool MainWindow::IsBitRes(QString strRes)
{
    QString strResName = strRes.left(1);
    if(strResName == "X" || strResName == "Y" || strResName == "M" || strResName == "S")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::IsByteRes(QString strRes)
{
    QString strResName = strRes.left(1);
    if(strResName == "D" || strResName == "R")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MainWindow::CheckRes(QString strRes,quint16 *pResAddr,quint8 *pResBytes,bool bWrite)
{
    quint16 nResIndex;
    QString strResName = strRes.left(1);
    if(strResName == "X")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,8);
        *pResAddr = m_ResLogicAddr[RES_X][0] * 8 + nResIndex;
        if(*pResAddr / 8 > m_ResLogicAddr[RES_X][1])
        {
            return false;
        }
        //////////////////
        m_MoResType = 0;
        m_MoResBitOffset = *pResAddr % 8;
        //////////////////
        if(!bWrite)
        {
            *pResAddr /= 8;
        }
        *pResBytes = 1;
        return true;
    }
    else if(strResName == "Y")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,8);
        *pResAddr = m_ResLogicAddr[RES_Y][0] * 8 + nResIndex;
        if(*pResAddr / 8 > m_ResLogicAddr[RES_Y][1])
        {
            return false;
        }
        //////////////////
        m_MoResType = 0;
        m_MoResBitOffset = *pResAddr % 8;
        //////////////////
        if(!bWrite)
        {
            *pResAddr /= 8;
        }
        *pResBytes = 1;
        return true;
    }
    else if(strResName == "M")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,10);
        if(nResIndex < 8000)
        {
            *pResAddr = m_ResLogicAddr[RES_M0][0] * 8 + nResIndex;
            if(*pResAddr / 8 > m_ResLogicAddr[RES_M0][1])
            {
                return false;
            }
        }
        else
        {
            *pResAddr = m_ResLogicAddr[RES_M8][0] * 8 + (nResIndex - 8000);
            if(*pResAddr / 8 > m_ResLogicAddr[RES_M8][1])
            {
                return false;
            }
        }
        //////////////////
        m_MoResType = 0;
        m_MoResBitOffset = *pResAddr % 8;
        //////////////////
        if(!bWrite)
        {
            *pResAddr /= 8;
        }
        *pResBytes = 1;
        return true;
    }
    else if(strResName == "S")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,10);
        *pResAddr = m_ResLogicAddr[RES_S][0] * 8 + nResIndex;
        if(*pResAddr / 8 > m_ResLogicAddr[RES_S][1])
        {
            return false;
        }
        //////////////////
        m_MoResType = 0;
        m_MoResBitOffset = *pResAddr % 8;
        //////////////////
        if(!bWrite)
        {
            *pResAddr /= 8;
        }
        *pResBytes = 1;
        return true;
    }
    else if(strResName == "D")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,10);
        if(nResIndex < 8000)
        {
            *pResAddr = m_ResLogicAddr[RES_D0][0] + nResIndex * 2;
            if(*pResAddr > m_ResLogicAddr[RES_D0][1])
            {
                return false;
            }
        }
        else
        {
            *pResAddr = m_ResLogicAddr[RES_D8][0] + (nResIndex - 8000) * 2;
            if(*pResAddr > m_ResLogicAddr[RES_D8][1])
            {
                return false;
            }
        }
        //////////////////
        m_MoResType = 1;
        //////////////////
        *pResBytes = 2;
        return true;
    }
    else if(strResName == "R")
    {
        nResIndex = strRes.right(strRes.length() - 1).toInt(NULL,10);
        *pResAddr = m_ResLogicAddr[RES_R][0] + nResIndex * 2;
        if(*pResAddr > m_ResLogicAddr[RES_R][1])
        {
            return false;
        }
        //////////////////
        m_MoResType = 1;
        //////////////////
        *pResBytes = 2;
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::on_SetOnBtn_clicked()
{
    if(!m_bUartOpen)
    {
        QMessageBox::about(this,"提示","串口未打开!");
        return;
    }
    if(m_bNowComm)
    {
        QMessageBox::about(this,"提示","上个通信任务未完成!");
        return;
    }
    QString strRes = ui->BitResName->text();
    strRes = strRes.trimmed();
    strRes = strRes.toUpper();
    if(!IsBitRes(strRes))
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->BitResName->setFocus();
        ui->BitResName->selectAll();
        return;
    }
    quint16 nResAddr;
    quint8 nResBytes;
    if(CheckRes(strRes,&nResAddr,&nResBytes,true))
    {
        ComToPlc(SETBIT,nResAddr,nResBytes,NULL);
        m_bNowComm = true;
        m_Timer.start(100);
    }
    else
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->BitResName->setFocus();
        ui->BitResName->selectAll();
    }
}

void MainWindow::on_SetOffBtn_clicked()
{
    if(!m_bUartOpen)
    {
        QMessageBox::about(this,"提示","串口未打开!");
        return;
    }
    if(m_bNowComm)
    {
        QMessageBox::about(this,"提示","上个通信任务未完成!");
        return;
    }
    QString strRes = ui->BitResName->text();
    strRes = strRes.trimmed();
    strRes = strRes.toUpper();
    if(!IsBitRes(strRes))
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->BitResName->setFocus();
        ui->BitResName->selectAll();
        return;
    }
    quint16 nResAddr;
    quint8 nResBytes;
    if(CheckRes(strRes,&nResAddr,&nResBytes,true))
    {
        ComToPlc(RSTBIT,nResAddr,nResBytes,NULL);
        m_bNowComm = true;
        m_Timer.start(100);
    }
    else
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->BitResName->setFocus();
        ui->BitResName->selectAll();
    }
}

void MainWindow::on_SetValueBtn_clicked()
{
    if(!m_bUartOpen)
    {
        QMessageBox::about(this,"提示","串口未打开!");
        return;
    }
    if(m_bNowComm)
    {
        QMessageBox::about(this,"提示","上个通信任务未完成!");
        return;
    }
    QString strRes = ui->ByteResName->text();
    strRes = strRes.trimmed();
    strRes = strRes.toUpper();
    if(!IsByteRes(strRes))
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->ByteResName->setFocus();
        ui->ByteResName->selectAll();
        return;
    }
    quint16 nResAddr;
    quint8 nResBytes;
    if(CheckRes(strRes,&nResAddr,&nResBytes,true))
    {
        QString strData = ui->ByteResData->text();
        strData = strData.trimmed();
        quint32 ResData = strData.toInt(NULL,10);
        quint8 Data[4];
        Data[0] = ResData % 0x100;
        Data[1] = ResData / 0x100;
        if(ui->TypeMode0->isChecked())
        {
            ComToPlc(WRITEBYTE,nResAddr,2,&Data[0]);
        }
        else if(ui->TypeMode1->isChecked())
        {
            Data[2] = (ResData / 0x10000) % 0x100;
            Data[3] = (ResData / 0x10000) / 0x100;
            ComToPlc(WRITEBYTE,nResAddr,4,&Data[0]);
        }
        m_bNowComm = true;
        m_Timer.start(100);
    }
    else
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->ByteResName->setFocus();
        ui->ByteResName->selectAll();
    }
}

void MainWindow::on_MonitorBtn_clicked()
{
    if(!m_bUartOpen)
    {
        QMessageBox::about(this,"提示","串口未打开!");
        return;
    }
    if(m_bNowComm)
    {
        QMessageBox::about(this,"提示","上个通信任务未完成!");
        return;
    }
    QString strRes = ui->ResName->text();
    strRes = strRes.trimmed();
    strRes = strRes.toUpper();
    quint16 nResAddr;
    quint8 nResBytes;
    if(CheckRes(strRes,&nResAddr,&nResBytes,false))
    {
        ComToPlc(READBYTE,nResAddr,nResBytes,NULL);
        m_bNowComm = true;
        m_Timer.start(100);
    }
    else
    {
        QMessageBox::about(this,"提示","元件输入有误!");
        ui->ResName->setFocus();
        ui->ResName->selectAll();
    }
}

const quint8 OFFSET_TAB[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void MainWindow::OnTimer()
{
    if(!m_bNowComm)
    {
        return;
    }
    quint8 nStatus = GetCommStatus();
    if(nStatus == COMMING)
    {
        return;
    }
    else
    {
        m_Timer.stop();
        if(nStatus == SETBIT_SUC || nStatus == RSTBIT_SUC || nStatus == WRITEBYTE_SUC)
        {
            QMessageBox::about(this,"提示","执行操作完成!");
        }
        else if(nStatus == READBYTE_SUC)
        {
            QMessageBox::about(this,"提示","执行操作完成!");
            QString strResData;
            quint8 *pResData = GetRevData();
            if(pResData != NULL)
            {
                if(m_MoResType == 0)
                {
                    //BIT RES
                    if((pResData[0] & OFFSET_TAB[m_MoResBitOffset]) == 0)
                    {
                        strResData = "OFF";
                    }
                    else
                    {
                        strResData = "ON";
                    }
                }
                else
                {
                    //BYTE RES
                    qint16 ConData = pResData[0] + pResData[1] * 0x100;
                    strResData.setNum(ConData,10);

                }
                ui->ResData->setText(strResData);
            }
            else
            {
                QMessageBox::about(this,"提示","无法获取数据内容!");
            }
        }
        else
        {
            QMessageBox::about(this,"提示","执行操作失败!\n通信超时或错误!");
        }
        m_bNowComm = false;
        SetCommStatus(FREE);
    }
}

void MainWindow::on_pBtnTest_clicked()
{
    if(!m_bUartOpen)
    {
        QMessageBox::about(this,"提示","串口未打开!");
        return;
    }
    if(m_bNowComm)
    {
        QMessageBox::about(this,"提示","上个通信任务未完成!");
        return;
    }

    ComToPlc(HANDSHAKE, 0, 0, NULL);
}
