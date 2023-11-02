#ifndef QUSERTYPEDLG_H
#define QUSERTYPEDLG_H

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
class QUserTypeDlg;
}

static QMutex UserProgressMutex;//全局互斥量[保护m_nProgressValue]
class QUserTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QUserTypeDlg(QWidget *parent = nullptr);
    ~QUserTypeDlg();

protected:
    digitalmysql                m_AccessDBObject;

    // 创建定时器对象
    int                         m_nProgressValue;
    QTimer                      m_timerUpdateUI;

    void InitDBParmeter();
    void SaveDBParmeter();

    void InitHumanList();
    void UpdateHumanList();

    void updateProgress(int nValue);

private slots:
    void on_timerUpdateUI();
    void on_m_btnHumanList_clicked();

private:
    Ui::QUserTypeDlg *ui;
};

#endif // QUSERTYPEDLG_H
