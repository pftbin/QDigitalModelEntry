#pragma once
#include <string>
#include <vector>

#include "public.h"
#include "mysql.h"


class digitalmysql
{
public:
    digitalmysql();
    ~digitalmysql();

public:
    typedef struct _HumanSource
    {
        std::string		strIndex;
        std::string		strHumanId;
        std::string		strHumanName;
        std::string		strUserName;
        std::string		strContentId;
        std::string		strSourceFolder;
        std::string		strAvailable;

        //Text+File
        std::string		strNomalSpeakSpeed;
        std::string		strSeriousSpeakSpeed;
        std::string     strImageMattingFile;
        std::string		strKeyFrameFile;
        std::string		strForegroundFile;
        std::string		strBackgroundFile;
        std::string		strForegroundFile2;
        std::string		strBackgroundFile2;
        std::string		strSpeakModelFile;
        std::string		strMouthModelFile;
        std::string		strFaceModelFile;

        //Folder
        std::string		strNormalVdoFolder;
        std::string		strSeriousVdoFolder;
        std::string		strActionVdoFolder;

        _HumanSource()
        {
            strIndex = std::string("");
            strHumanId = std::string("");
            strHumanName = std::string("");
            strUserName = std::string("");
            strContentId = std::string("");
            strSourceFolder = std::string("");
            strAvailable = std::string("0");

            strNomalSpeakSpeed = std::string("1.0");
            strSeriousSpeakSpeed = std::string("0.8");
            strImageMattingFile = std::string("");
            strKeyFrameFile = std::string("");
            strForegroundFile = std::string("");
            strBackgroundFile = std::string("");
            strForegroundFile2 = std::string("");
            strBackgroundFile2 = std::string("");
            strSpeakModelFile = std::string("");
            strMouthModelFile = std::string("");
            strFaceModelFile = std::string("");

            strNormalVdoFolder = std::string("");
            strSeriousVdoFolder = std::string("");
            strActionVdoFolder = std::string("");
        }

        void CopyData(const _HumanSource srcHumanItem)
        {
            strIndex = srcHumanItem.strIndex;
            strHumanId = srcHumanItem.strHumanId;
            strHumanName = srcHumanItem.strHumanName;
            strUserName = srcHumanItem.strUserName;
            strContentId = srcHumanItem.strContentId;
            strSourceFolder = srcHumanItem.strSourceFolder;
            strAvailable = srcHumanItem.strAvailable;

            strNomalSpeakSpeed = srcHumanItem.strNomalSpeakSpeed;
            strSeriousSpeakSpeed = srcHumanItem.strSeriousSpeakSpeed;
            strImageMattingFile = srcHumanItem.strImageMattingFile;
            strKeyFrameFile = srcHumanItem.strKeyFrameFile;
            strForegroundFile = srcHumanItem.strForegroundFile;
            strBackgroundFile = srcHumanItem.strBackgroundFile;
            strForegroundFile2 = srcHumanItem.strForegroundFile2;
            strBackgroundFile2 = srcHumanItem.strBackgroundFile2;
            strSpeakModelFile = srcHumanItem.strSpeakModelFile;
            strMouthModelFile = srcHumanItem.strMouthModelFile;
            strFaceModelFile = srcHumanItem.strFaceModelFile;

            strNormalVdoFolder = srcHumanItem.strNormalVdoFolder;
            strSeriousVdoFolder = srcHumanItem.strSeriousVdoFolder;
            strActionVdoFolder = srcHumanItem.strActionVdoFolder;
        }
    } HumanSource, *pHumanSource;

    bool setInitParemeter(std::string IP, std::string Port, std::string DBName, std::string UserName, std::string Password);
    bool selectAllHumanInfo(std::vector<HumanSource>& vecHumanInfo);


protected:
    std::string     m_strDBIp;
    std::string     m_strDBPort;
    std::string     m_strDBName;
    std::string     m_strDBUsername;
    std::string     m_strDBPassword;
};

