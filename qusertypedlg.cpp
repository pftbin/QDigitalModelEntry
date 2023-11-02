#include "qusertypedlg.h"
#include "ui_qusertypedlg.h"

#include <QSettings>

QUserTypeDlg::QUserTypeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QUserTypeDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::SubWindow);

    //连接定时器的timeout()信号到槽函数
    m_nProgressValue = 0;
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    QObject::connect(&m_timerUpdateUI, &QTimer::timeout, this, &QUserTypeDlg::on_timerUpdateUI);
    m_timerUpdateUI.start(50);

    //设定IP输入格式
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"), this);
    ui->m_editIP->setValidator(validator);

    InitDBParmeter();

    InitHumanList();
    UpdateHumanList();
}

QUserTypeDlg::~QUserTypeDlg()
{
    delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////
void QUserTypeDlg::InitDBParmeter()
{
    QString currentDir = QCoreApplication::applicationDirPath();
    QString strIniFilePath = tr("%1/Config.ini").arg(currentDir);
    QSettings settings(strIniFilePath, QSettings::IniFormat);

    QString DB_IP = settings.value("DB/IP").toString();
    QString DB_Port = settings.value("DB/Port").toString();
    QString DB_DBName = settings.value("DB/DBName").toString();
    QString DB_UserName = settings.value("DB/UserName").toString();
    QString DB_PassWord = settings.value("DB/PassWord").toString();

    ui->m_editIP->setText(DB_IP);
    ui->m_editPort->setText(DB_Port);
    ui->m_editDBName->setText(DB_DBName);
    ui->m_editUserName->setText(DB_UserName);
    ui->m_editPassWord->setText(DB_PassWord);
}

void QUserTypeDlg::SaveDBParmeter()
{
    QString currentDir = QCoreApplication::applicationDirPath();
    QString strIniFilePath = tr("%1/Config.ini").arg(currentDir);
    QSettings settings(strIniFilePath, QSettings::IniFormat);

    settings.setValue("DB/IP", ui->m_editIP->text());
    settings.setValue("DB/Port", ui->m_editPort->text());
    settings.setValue("DB/DBName",  ui->m_editDBName->text());
    settings.setValue("DB/UserName", ui->m_editUserName->text());
    settings.setValue("DB/PassWord", ui->m_editPassWord->text());
    settings.sync(); // 确保立即保存到文件
}

void QUserTypeDlg::InitHumanList()
{
    ui->m_listHumanInfo->addCol(tr("ID"),CELL_WIDTH);
    ui->m_listHumanInfo->addCol(tr("HumanName"),CELL_WIDTH*2);
    ui->m_listHumanInfo->addCol(tr("BelongUser"),CELL_WIDTH*2);
    ui->m_listHumanInfo->addCol(tr("HumanState"),CELL_WIDTH*2);
    ui->m_listHumanInfo->addCol(tr("UpdateModel"),CELL_WIDTH);
    ui->m_listHumanInfo->addCol(tr("DownloadFile"),CELL_WIDTH);
}

void QUserTypeDlg::UpdateHumanList()
{
    SaveDBParmeter();

    QString DB_IP = ui->m_editIP->text();
    QString DB_Port = ui->m_editPort->text();
    QString DB_DBName = ui->m_editDBName->text();
    QString DB_UserName = ui->m_editUserName->text();
    QString DB_PassWord = ui->m_editPassWord->text();

    ui->m_listHumanInfo->delAllRow();
    std::vector<digitalmysql::HumanSource> vecHumanInfoList;
    m_AccessDBObject.setInitParemeter(DB_IP.toStdString(),DB_Port.toStdString(),DB_DBName.toStdString(),DB_UserName.toStdString(),DB_PassWord.toStdString());
    m_AccessDBObject.selectAllHumanInfo(vecHumanInfoList);
    for(int i = 0; i < (int)vecHumanInfoList.size(); i++)
    {
        ui->m_listHumanInfo->addRow();
        ui->m_listHumanInfo->setItemText(i,0,tr("%1").arg(i));
        ui->m_listHumanInfo->setItemText(i,1,QString::fromStdString(vecHumanInfoList[i].strHumanName));
        ui->m_listHumanInfo->setItemText(i,2,QString::fromStdString(vecHumanInfoList[i].strUserName));
        ui->m_listHumanInfo->setItemText(i,3,(STR_TO_INT(vecHumanInfoList[i].strAvailable))?(tr("Enable")):(tr("Training")));
        ui->m_listHumanInfo->setItemButton(i,4,tr("..."));
        ui->m_listHumanInfo->setItemButton(i,5,tr("..."));
    }
}

void QUserTypeDlg::updateProgress(int nValue)
{
    nValue = max(ui->progressBar->minimum(), nValue);
    nValue = min(ui->progressBar->maximum(), nValue);
    if (nValue == 0)
        ui->progressBar->setVisible(false);
    else
        ui->progressBar->setVisible(true);

    ui->progressBar->setValue(nValue);
}

////////////////////////////////////////////////////////////////////////////////////////////

void QUserTypeDlg::on_timerUpdateUI()
{
    //进度条更新
    updateProgress(m_nProgressValue);
    if (m_nProgressValue >= 100)
    {
        UserProgressMutex.lock();
        m_nProgressValue = 0;
        UserProgressMutex.unlock();
    }
}

void QUserTypeDlg::on_m_btnHumanList_clicked()
{
    UpdateHumanList();
}

