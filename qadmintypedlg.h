#ifndef QADMINTYPEDLG_H
#define QADMINTYPEDLG_H

#include <QObject>
#include <QDialog>
#include <QMessageBox>
#include <QDebug>

#include <QTimer>
#include <QTableView>
#include <QThread>
#include <QMutex>
#include "digitalMysql.h"

namespace Ui {
class QAdminTypeDlg;
}

static QMutex adminProgressMutex;//全局互斥量[保护m_nProgressValue]
class QAdminTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QAdminTypeDlg(QWidget *parent = nullptr);
    ~QAdminTypeDlg();

    friend class PackageThread;

protected:
    QString                     m_strHumanName;
    QStringList                 m_strPropKeyList;
    digitalmysql::HumanSource   m_nowHumanSourceItem;

    // 创建定时器对象
    int                         m_nProgressValue;
    QTimer                      m_timerUpdateUI;

    //线程对象
    PackageThread*              m_pPackageThread;

    void InitNormalList();
    void InitActionList();
    void AddAction();
    void DelAction(int nRowIdx);
    void updateProgress(int nValue);

    bool SaveUIHumanSource();


private slots:
    void on_timerUpdateUI();
    void on_m_editHumanName_editingFinished();
    void onNormalListButtonClicked(int rowNum, int colNum);

    void on_m_btnAddAction_clicked();
    void on_m_btnMinusAction_clicked();
    void onAcionListButtonClicked(int rowNum, int colNum);

    void on_m_btnPackage_clicked();
    void on_threadAddProgress(int data);

private:
    Ui::QAdminTypeDlg   *ui;
};

// 自定义线程类2
class AddValueThread : public QThread
{
    Q_OBJECT

signals:
    void sendValueData(int data);

public:
    AddValueThread() {}

private:

    // 线程执行函数
    void run() override
    {
        int nValue = 0;
        while(nValue<100)
        {
            nValue++;
            emit sendValueData(nValue);//发送信号到父线程中
            msleep(100);//100毫秒
        }
    }
};

class PackageThread : public QThread
{
    Q_OBJECT

signals:
    void sendProgressData(int data);

public:
    PackageThread(QAdminTypeDlg* parent) : m_pParent(parent) {}

private:
    QAdminTypeDlg* m_pParent;

#if 1
    // 线程执行函数[测试子线程与主线程交互]
    void run() override
    {
        int nValue = 0;
        while(nValue<=100)
        {
            nValue++;
            msleep(100);//100毫秒

            //直接访问主线程变量【需注意访问共享资源】
            //progressMutex.lock();
            //m_pParent->m_nProgressValue = nValue;
            //progressMutex.unlock();

            //发送信号到主线程【可避免访问共享资源】
            emit sendProgressData(nValue);
        }
    }
#else
    // 线程执行函数[测试子线程与孙子线程交互]
    void run() override
    {
        AddValueThread* pAddValueThread = new AddValueThread();
        QObject::connect(pAddValueThread, &AddValueThread::sendValueData, this, &PackageThread::recvValueData);


        pAddValueThread->start();   //启动子线程
        pAddValueThread->wait();    //等待子线程退出
    }

private slots:
    void recvValueData(int data)
    {
        if(data<=100)
        {
            emit sendProgressData(data);//发送信号到主线程
        }
    }

#endif

};







#endif // QADMINTYPEDLG_H
