#ifndef CWIDCENTERCTRL_H
#define CWIDCENTERCTRL_H

#include <QWidget>

class QLabel;
class QPushButton;

// 中控

class CWidCenterCtrl : public QWidget
{
    Q_OBJECT
public:
    explicit CWidCenterCtrl(QWidget *parent = nullptr);

signals:

protected:
    void InitData();
    void InitConnect();

private:
    QString GetFrameStyle();

private:
    QLabel*                     m_pLabSwitchCtrl;           // 开关控制
    QLabel*                     m_pLabScreenCtrl;           // 屏幕控制
    QLabel*                     m_pLabMicCtrl;              // 话筒控制

    QLabel*                     m_pLabStartUp;
    QLabel*                     m_pLabShutdown;

    QLabel*                     m_pLabRise;
    QLabel*                     m_pLabDescend;

    QLabel*                     m_pLabTears;
    QLabel*                     m_pLabHypsokinesis;

    QLabel*                     m_pLabMicRise;
    QLabel*                     m_pLabMicDescend;

    QPushButton*                m_pBtnStartUp;              // 开机
    QPushButton*                m_pBtnShutdown;             // 关机

    QPushButton*                m_pBtnRise;                 // 屏幕上升
    QPushButton*                m_pBtnDescend;              // 屏幕下降

    QPushButton*                m_pBtnTears;                // 屏幕前仰
    QPushButton*                m_pBtnHypsokinesis;         // 屏幕后仰

    QPushButton*                m_pBtnMicRise;              // 话筒上升
    QPushButton*                m_pBtnMicDescend;           // 话筒下降
};

#endif // CWIDCENTERCTRL_H
