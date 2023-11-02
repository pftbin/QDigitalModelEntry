#include "mainwindow.h"
#include <QApplication>
#include "qglobaldefine.h"

#include "QFile"
#include <QTranslator>
int main(int argc, char *argv[])
{
    //放在QApplication的前面,解决DPI缩放问题
    if(QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    //语言翻译
    QTranslator translator;
    QString currentDir = QCoreApplication::applicationDirPath();
    QLocale currentLocale = QLocale();
    QLocale::Language languageCode = currentLocale.language();
    if (languageCode == QLocale::Chinese)
    {
        // 当前系统为中文环境
        QString strQMFilePath = QString("%1/language/DigitalModelEntry_zh_CN.qm").arg(currentDir);
        if(isPathExists(strQMFilePath))
            translator.load(strQMFilePath);
    }
    a.installTranslator(&translator);

    //加载样式表
    QFile file(":/qss/GreenStyle.qss");
    if (file.open(QFile::ReadOnly))
    {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);

        file.close();
    }


    MainWindow w;
    w.show();

    return a.exec();
}
