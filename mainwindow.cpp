#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置任务栏图标
    setWindowIcon(QIcon(":/Main.ico"));

    m_curUIType = -1;
    m_pUserTypeDlg = new QUserTypeDlg(this);
    m_pAdminTypeDlg = new QAdminTypeDlg(this);
    switchType(UIType_User);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideAllDlg()
{
    if (m_pUserTypeDlg)
        m_pUserTypeDlg->hide();
    if (m_pAdminTypeDlg)
        m_pAdminTypeDlg->hide();
}

void MainWindow::switchType(int nType)
{
    if(m_curUIType == nType) return;

    hideAllDlg();
    switch (nType)
    {
    case UIType_User:
        {
            m_pUserTypeDlg->show();
            m_curUIType = nType;
        }
        break;
    case UIType_Admin:
        {
            m_pAdminTypeDlg->show();
            m_curUIType = nType;
        }
        break;
    default:
        break;
    }

}

void MainWindow::on_actionuser_triggered()
{
    switchType(UIType_User);
}


void MainWindow::on_actionadmin_triggered()
{
    switchType(UIType_Admin);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (m_pUserTypeDlg)
    {
        m_pUserTypeDlg->setGeometry(this->centralWidget()->geometry());
    }
    if (m_pAdminTypeDlg)
    {
        m_pAdminTypeDlg->setGeometry(this->centralWidget()->geometry());
    }
}
