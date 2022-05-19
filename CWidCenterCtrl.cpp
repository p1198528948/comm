#include "CWidCenterCtrl.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <windows.h>
#include <functional>
#include <QDebug>
#include <QDateTime>

CWidCenterCtrl::CWidCenterCtrl(QWidget *parent)
    : QWidget(parent)
{
    InitData();
    InitConnect();

    TestC11();
}

void CWidCenterCtrl::InitData()
{
    // Label Title
    m_pLabSwitchCtrl = new QLabel(QStringLiteral("开关控制"), this);
    m_pLabScreenCtrl = new QLabel(QStringLiteral("屏幕控制"), this);
    m_pLabMicCtrl = new QLabel(QStringLiteral("话筒控制"), this);

    // btns
    QVBoxLayout* pBtnStartUpLayout = new QVBoxLayout;
    m_pBtnStartUp = new QPushButton(this);
    m_pLabStartUp = new QLabel(QStringLiteral("全体开机"), this);
    pBtnStartUpLayout->addWidget(m_pBtnStartUp);
    pBtnStartUpLayout->addWidget(m_pLabStartUp);

    QVBoxLayout* pBtnShutdownLayout = new QVBoxLayout;
    m_pBtnShutdown = new QPushButton(this);
    m_pLabShutdown = new QLabel(QStringLiteral("全体关机"), this);
    pBtnShutdownLayout->addWidget(m_pBtnShutdown);
    pBtnShutdownLayout->addWidget(m_pLabShutdown);

    //---------------------------------------------------------------------------

    QVBoxLayout* pBtnRiseLayout = new QVBoxLayout;
    m_pBtnRise = new QPushButton(this);
    m_pLabRise = new QLabel(QStringLiteral("全体上升"), this);
    pBtnRiseLayout->addWidget(m_pBtnRise);
    pBtnRiseLayout->addWidget(m_pLabRise);

    QVBoxLayout* pBtnDescendLayout = new QVBoxLayout;
    m_pBtnDescend = new QPushButton(this);
    m_pLabDescend = new QLabel(QStringLiteral("全体下降"), this);
    pBtnDescendLayout->addWidget(m_pBtnDescend);
    pBtnDescendLayout->addWidget(m_pLabDescend);

    QVBoxLayout* pBtnTearsLayout = new QVBoxLayout;
    m_pBtnTears = new QPushButton(this);
    m_pLabTears = new QLabel(QStringLiteral("全体前仰"), this);
    pBtnTearsLayout->addWidget(m_pBtnTears);
    pBtnTearsLayout->addWidget(m_pLabTears);

    QVBoxLayout* pBtnHypsokinesisLayout = new QVBoxLayout;
    m_pBtnHypsokinesis = new QPushButton(this);
    m_pLabHypsokinesis = new QLabel(QStringLiteral("全体后仰"), this);
    pBtnHypsokinesisLayout->addWidget(m_pBtnHypsokinesis);
    pBtnHypsokinesisLayout->addWidget(m_pLabHypsokinesis);

    //---------------------------------------------------------------------------
    QVBoxLayout* pBtnMicRiseLayout = new QVBoxLayout;
    m_pBtnMicRise = new QPushButton(this);
    m_pLabMicRise = new QLabel(QStringLiteral("话筒上升"), this);
    pBtnMicRiseLayout->addWidget(m_pBtnMicRise);
    pBtnMicRiseLayout->addWidget(m_pLabMicRise);

    QVBoxLayout* pBtnMicDescendLayout = new QVBoxLayout;
    m_pBtnMicDescend = new QPushButton(this);
    m_pLabMicDescend = new QLabel(QStringLiteral("话筒下降"), this);
    pBtnMicDescendLayout->addWidget(m_pBtnMicDescend);
    pBtnMicDescendLayout->addWidget(m_pLabMicDescend);

    // 主垂直布局
    QVBoxLayout* pMainLayout = new QVBoxLayout;

    pMainLayout->addWidget(m_pLabSwitchCtrl);
    QFrame* pFrameLine1 = new QFrame(this);
    pFrameLine1->setFixedHeight(1);
    pFrameLine1->setObjectName("pFrameLine");
    pFrameLine1->setStyleSheet(GetFrameStyle());
    pMainLayout->addWidget(pFrameLine1);

    QHBoxLayout* pHBoxLayout1 = new QHBoxLayout;
    pHBoxLayout1->addLayout(pBtnStartUpLayout);
    pHBoxLayout1->addLayout(pBtnShutdownLayout);
    pHBoxLayout1->addStretch();
    pMainLayout->addLayout(pHBoxLayout1);

    pMainLayout->addWidget(m_pLabScreenCtrl);
    QFrame* pFrameLine2 = new QFrame(this);
    pFrameLine2->setFixedHeight(1);
    pFrameLine2->setObjectName("pFrameLine");
    pFrameLine2->setStyleSheet(GetFrameStyle());
    pMainLayout->addWidget(pFrameLine2);

    QHBoxLayout* pHBoxLayout2 = new QHBoxLayout;
    pHBoxLayout2->addLayout(pBtnRiseLayout);
    pHBoxLayout2->addLayout(pBtnDescendLayout);
    pHBoxLayout2->addLayout(pBtnTearsLayout);
    pHBoxLayout2->addLayout(pBtnHypsokinesisLayout);
    pHBoxLayout2->addStretch();
    pMainLayout->addLayout(pHBoxLayout2);

    pMainLayout->addWidget(m_pLabMicCtrl);
    QFrame* pFrameLine3 = new QFrame(this);
    pFrameLine3->setFixedHeight(1);
    pFrameLine3->setObjectName("pFrameLine");
    pFrameLine3->setStyleSheet(GetFrameStyle());
    pMainLayout->addWidget(pFrameLine3);

    QHBoxLayout* pHBoxLayout3 = new QHBoxLayout;
    pHBoxLayout3->addLayout(pBtnMicRiseLayout);
    pHBoxLayout3->addLayout(pBtnMicDescendLayout);
    pHBoxLayout3->addStretch();
    pMainLayout->addLayout(pHBoxLayout3);

    setLayout(pMainLayout);

}

void CWidCenterCtrl::InitConnect()
{

}

void CWidCenterCtrl::TestC11()
{
    qDebug() << "CWidCenterCtrl::TestC11() in" << QDateTime::currentDateTime();

    int nResult = 0;

    std::future<int> temp_result = start_result.get_future();
    std::chrono::milliseconds span(3000);
    std::future_status status = temp_result.wait_for(span);
    if (status == std::future_status::ready)
    {
        nResult = temp_result.get();
    }
    else
    {
        // 执行耗时任务 4s
        qDebug() << QStringLiteral("执行耗时任务 4s in") << QDateTime::currentDateTime();
        Sleep(4000);
        qDebug() << QStringLiteral("执行耗时任务 4s out" ) << QDateTime::currentDateTime();
    }

    start_result = std::promise<int>(); //promise重置

    qDebug() << "CWidCenterCtrl::TestC11() out" << QDateTime::currentDateTime();
}

QString CWidCenterCtrl::GetFrameStyle()
{
    return "QFrame#pFrameLine"
            "{border:1px solid rgb(246,125,115);}";
}
