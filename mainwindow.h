#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QWidget"
#include "qusertypedlg.h"
#include "qadmintypedlg.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum UIType
    {
        UIType_User = 0,
        UIType_Admin,
    };

protected:
    int                 m_curUIType;
    QUserTypeDlg*       m_pUserTypeDlg;
    QAdminTypeDlg*      m_pAdminTypeDlg;

    void hideAllDlg();
    void switchType(int nType);

private slots:
    void on_actionuser_triggered();
    void on_actionadmin_triggered();
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
