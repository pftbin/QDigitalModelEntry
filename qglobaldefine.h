#ifndef QGLOBALDEFINE_H
#define QGLOBALDEFINE_H

#include <QDir>
#include <QFile>
#include <QFileDialog>

static bool isFolderExists(const QString& path)
{
    QDir dir(path);
    return dir.exists();
}

static bool isFileExists(const QString& path)
{
    QFile file(path);
    return file.exists();
}

static bool isPathExists(const QString& path)
{
    QDir dir(path);
    QFile file(path);
    return (dir.exists() || file.exists());
}

static QString openSelectFileDlg(QString strTitle, const QStringList &filterList)
{
    QString filter;
    for (const QString &fileType : filterList)
    {
        filter += fileType + ";;";
    }
    filter.chop(2); // 去除最后的两个分号

    return QFileDialog::getOpenFileName(nullptr, strTitle, "", filter);
}

static QString openSelectFolderDialog(QString strTitle)
{
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, strTitle, QDir::homePath());
    return folderPath;
}

#endif // QGLOBALDEFINE_H
