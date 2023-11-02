#include "qadmintypedlg.h"
#include "ui_qadmintypedlg.h"

#include "qglobaldefine.h"

/////////////////////////////////////////////////////////////////////
enum ROW_TYPE
{
    Row_NormalSpeed = 0,
    Row_SeriousSpeed,
    Row_Matting,
    Row_KeyFrame,
    Row_Foreground,
    Row_Background,
    Row_Foreground2,
    Row_Background2,
    Row_SpeakModel,
    Row_MouthModel,
    Row_FaceMode,
    Row_NormalVdoGroup,
    Row_SeriousVdoGroup,
};

QAdminTypeDlg::QAdminTypeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QAdminTypeDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::SubWindow);

    //连接线程自定义信号到槽函数
    m_pPackageThread = new PackageThread(this);
    QObject::connect(m_pPackageThread, &PackageThread::sendProgressData, this, &QAdminTypeDlg::on_threadAddProgress);

    //连接定时器的timeout()信号到槽函数
    m_nProgressValue = 0;
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    QObject::connect(&m_timerUpdateUI, &QTimer::timeout, this, &QAdminTypeDlg::on_timerUpdateUI);
    m_timerUpdateUI.start(50);

    //
    m_strHumanName = "";
    m_strPropKeyList.append(tr("Normal SpeakSpeed"));
    m_strPropKeyList.append(tr("Serious SpeakSpeed"));
    m_strPropKeyList.append(tr("Imatting File"));
    m_strPropKeyList.append(tr("Keyframe File"));
    m_strPropKeyList.append(tr("Foreground[16:9]"));
    m_strPropKeyList.append(tr("Background[16:9]"));
    m_strPropKeyList.append(tr("Foreground2[9:16]"));
    m_strPropKeyList.append(tr("Background2[9:16]"));
    m_strPropKeyList.append(tr("SpeakModel File"));
    m_strPropKeyList.append(tr("MouthModel File"));
    m_strPropKeyList.append(tr("FaceModel File"));
    m_strPropKeyList.append(tr("Normal VideoGroup"));
    m_strPropKeyList.append(tr("Serious VideoGroup"));

    InitNormalList();
    InitActionList();
}

QAdminTypeDlg::~QAdminTypeDlg()
{
    m_timerUpdateUI.stop();

    delete m_pPackageThread;
    m_pPackageThread = nullptr;

    delete ui;
}

/////////////////////////////////////////////////////////////////////
void QAdminTypeDlg::InitNormalList()
{
    ui->m_listNormal->addCol(tr("Property"),CELL_WIDTH);
    ui->m_listNormal->addCol(tr("Property Value"),CELL_WIDTH*4);
    ui->m_listNormal->addCol(tr("    "),CELL_WIDTH);
    QObject::connect(ui->m_listNormal, SIGNAL(buttonClicked(int,int)), this, SLOT(onNormalListButtonClicked(int,int)));

    for(int i = 0; i < m_strPropKeyList.count(); i++)
    {
        ui->m_listNormal->addRow();
        ui->m_listNormal->setItemText(i,0,m_strPropKeyList.at(i));

        if(i==Row_NormalSpeed)
        {
            ui->m_listNormal->setItemEdit(i,1,tr("1.0"));
        }
        else if(i==Row_SeriousSpeed)
        {
            ui->m_listNormal->setItemEdit(i,1,tr("0.8"));
        }
        else
        {
            ui->m_listNormal->setItemText(i,1,tr(""));
            ui->m_listNormal->setItemButton(i,2,tr("..."));
        }
    }
}

void QAdminTypeDlg::InitActionList()
{
    ui->m_listAction->addCol(tr("ActionName"),CELL_WIDTH);
    ui->m_listAction->addCol(tr("Video File"),CELL_WIDTH*4);
    ui->m_listAction->addCol(tr("  "),CELL_WIDTH);
    ui->m_listAction->addCol(tr("Keyframe File"),CELL_WIDTH*4);
    ui->m_listAction->addCol(tr("  "),CELL_WIDTH);
    QObject::connect(ui->m_listAction, SIGNAL(buttonClicked(int,int)), this, SLOT(onAcionListButtonClicked(int,int)));

    //列宽校正
    AddAction();
    DelAction(0);
}

void QAdminTypeDlg::AddAction()
{
    int nRowCount = ui->m_listAction->addRow();
    ui->m_listAction->setItemEdit(nRowCount-1, 0, tr("Unknow Action"));
    ui->m_listAction->setItemText(nRowCount-1, 1, tr(""));
    ui->m_listAction->setItemButton(nRowCount-1, 2, tr("..."));
    ui->m_listAction->setItemText(nRowCount-1, 3, tr(""));
    ui->m_listAction->setItemButton(nRowCount-1, 4, tr("..."));
}

void QAdminTypeDlg::DelAction(int nRowIdx)
{
    if (nRowIdx >= 0 && nRowIdx < ui->m_listAction->getrowCount())
        ui->m_listAction->delRow(nRowIdx);
}

void QAdminTypeDlg::updateProgress(int nValue)
{
    nValue = max(ui->progressBar->minimum(), nValue);
    nValue = min(ui->progressBar->maximum(), nValue);
    if (nValue == 0)
        ui->progressBar->setVisible(false);
    else
        ui->progressBar->setVisible(true);

    ui->progressBar->setValue(nValue);
}

bool QAdminTypeDlg::SaveUIHumanSource()
{
    for(int i = 0; i < m_strPropKeyList.count(); i++)
    {
        std::string strItemText = (ui->m_listNormal->getItemText(i,1)).toStdString();
        if (strItemText.empty())
        {
            QMessageBox::warning(this,tr("Error"), tr("No.%1 Value is empty").arg(i));
            return false;
        }

        switch (i)
        {
        case Row_NormalSpeed:
            m_nowHumanSourceItem.strNomalSpeakSpeed = strItemText;
            break;
        case Row_SeriousSpeed:
            m_nowHumanSourceItem.strSeriousSpeakSpeed = strItemText;
            break;
        case Row_Matting:
            m_nowHumanSourceItem.strImageMattingFile = strItemText;
            break;
        case Row_KeyFrame:
            m_nowHumanSourceItem.strKeyFrameFile = strItemText;
            break;
        case Row_Foreground:
            m_nowHumanSourceItem.strForegroundFile = strItemText;
            break;
        case Row_Background:
            m_nowHumanSourceItem.strBackgroundFile = strItemText;
            break;
        case Row_Foreground2:
            m_nowHumanSourceItem.strForegroundFile2 = strItemText;
            break;
        case Row_Background2:
            m_nowHumanSourceItem.strBackgroundFile2 = strItemText;
            break;
        case Row_SpeakModel:
            m_nowHumanSourceItem.strSpeakModelFile = strItemText;
            break;
        case Row_MouthModel:
            m_nowHumanSourceItem.strMouthModelFile = strItemText;
            break;
        case Row_FaceMode:
            m_nowHumanSourceItem.strFaceModelFile = strItemText;
            break;
        case Row_NormalVdoGroup:
            m_nowHumanSourceItem.strNormalVdoFolder = strItemText;
            break;
        case Row_SeriousVdoGroup:
            m_nowHumanSourceItem.strSeriousVdoFolder = strItemText;
            break;
        default:
            break;
        }
    }

    return true;
}

/////////////////////////////////////////////////////////////////////

void QAdminTypeDlg::on_timerUpdateUI()
{
    //进度条更新
    updateProgress(m_nProgressValue);
    if (m_nProgressValue >= 100)
    {
        adminProgressMutex.lock();
        m_nProgressValue = 0;
        adminProgressMutex.unlock();
    }
}

void QAdminTypeDlg::on_m_editHumanName_editingFinished()
{
    m_strHumanName = ui->m_editHumanName->text();
}

void QAdminTypeDlg::onNormalListButtonClicked(int rowNum, int colNum)
{
    QStringList filterList;
    if (colNum == 2)
    {
        QString strTitle = tr("Select File");
        QString strTitle2 = tr("Select Folder");
        filterList.clear();
        switch (rowNum)
        {
        case Row_Matting://xml
        {
            filterList.append(tr("XML File (*.xml)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_KeyFrame://png
        {
            filterList.append(tr("Picture File (*.png)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_Foreground://png
        {
            filterList.append(tr("Picture File (*.png)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_Background://png
        {
            filterList.append(tr("Picture File (*.png)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_Foreground2://png
        {
            filterList.append(tr("Picture File (*.png)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_Background2://png
        {
            filterList.append(tr("Picture File (*.png)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_SpeakModel://pdz
        {
            filterList.append(tr("SpeakModel File (*.pdz)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_MouthModel://pth
        {
            filterList.append(tr("MouthModel File (*.pth)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_FaceMode://dfm
        {
            filterList.append(tr("FaceModel File (*.dfm)"));
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFileDlg(strTitle,filterList));
        }
            break;
        case Row_NormalVdoGroup:
        {
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFolderDialog(strTitle2));
        }
            break;
        case Row_SeriousVdoGroup:
        {
            ui->m_listNormal->setItemText(rowNum, 1, openSelectFolderDialog(strTitle2));
        }
            break;
        default:
            break;
        }
    }
}

void QAdminTypeDlg::on_m_btnAddAction_clicked()
{
    AddAction();
}

void QAdminTypeDlg::on_m_btnMinusAction_clicked()
{
    int nRowIdx = ui->m_listAction->currentIndex().row();
    DelAction(nRowIdx);
}

void QAdminTypeDlg::onAcionListButtonClicked(int rowNum, int colNum)
{
    if (colNum == 2)//video
    {
        QStringList filterList;
        filterList.append(tr("Video File (*.mp4)"));
        QString strFilePath = openSelectFileDlg(tr("Select File"), filterList);
        ui->m_listNormal->setItemText(rowNum, 1, strFilePath);
    }

    if (colNum == 4)//keyframe
    {
        QStringList filterList;
        filterList.append("Picture File (*.png *.jpg *.jpeg *.bmp)");
        QString strFilePath = openSelectFileDlg(tr("Select File"), filterList);
        ui->m_listNormal->setItemText(rowNum, 1, strFilePath);
    }
}

void QAdminTypeDlg::on_m_btnPackage_clicked()
{
    if (!SaveUIHumanSource())
        return;

    //线程更新进度
    m_pPackageThread->start();
}

void QAdminTypeDlg::on_threadAddProgress(int data)
{
    m_nProgressValue = data;
}

