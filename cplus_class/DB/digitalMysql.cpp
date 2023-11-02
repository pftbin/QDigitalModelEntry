#include "digitalMysql.h"

#pragma comment(lib,"libmysql.lib")
#pragma warning(error:4477)  //<snprintf> warning as error

#define BUFF_SZ				1024*16			//system max stack size

static std::string row_value(char* pChar)
{
    std::string result_str = "";
    if (pChar != nullptr)
        result_str = pChar;
    return result_str;
}

/////////////////////////////////////////////////////////////////////////
digitalmysql::digitalmysql()
{

}

digitalmysql::~digitalmysql()
{

}

/////////////////////////////////////////////////////////////////////////
bool digitalmysql::setInitParemeter(std::string IP, std::string Port, std::string DBName, std::string UserName, std::string Password)
{
    m_strDBIp = IP;
    m_strDBPort = Port;
    m_strDBName = DBName;
    m_strDBUsername = UserName;
    m_strDBPassword = Password;
    if (m_strDBIp.empty() || m_strDBPort.empty() || m_strDBName.empty() || m_strDBUsername.empty() || m_strDBPassword.empty())
        return false;

    return true;
}

bool digitalmysql::selectAllHumanInfo(std::vector<HumanSource>& vecHumanInfo)
{
    bool bResult = true;
    //=====================
    MYSQL mysql;
    mysql_init(&mysql);		//inti MYSQL

    if (!mysql_real_connect(&mysql, m_strDBIp.c_str(), m_strDBUsername.c_str(), m_strDBPassword.c_str(), m_strDBName.c_str(), STR_TO_INT(m_strDBPort), NULL, 0)) //connect mysql
    {
        std::string error = mysql_error(&mysql);
        return false;
    }

    //select
    std::string sql = ("select sbt_humansource.id,sbt_userinfo.loginname,humanid,humanname,contentid,sourcefolder,available,speakspeed,seriousspeed,imagematting,keyframe,foreground,speakpath,mouthmodefile,facemodefile from  sbt_humansource left join sbt_userinfo on sbt_humansource.belongid = sbt_userinfo.id");
    mysql_query(&mysql, "SET NAMES UTF8");		//support chinese text
    if (!mysql_query(&mysql, sql.c_str()))		//success return 0,failed return random number
    {
        MYSQL_RES* result;						//table data struct
        result = mysql_store_result(&mysql);    //sava dada to result
        int rownum = mysql_num_rows(result);	//get row number
        int colnum = mysql_num_fields(result);  //get col number

        MYSQL_ROW row;							//table row data
        while (row = mysql_fetch_row(result))
        {
            if (colnum >= 15)
            {
                int i = 0;
                HumanSource HumanItem;
                int row_id = atoi(row_value(row[i++]).c_str());					HumanItem.strIndex = INT_TO_STR(row_id);
                std::string row_username = row_value(row[i++]);					HumanItem.strUserName = (row_username);
                std::string row_humanid = row_value(row[i++]);					HumanItem.strHumanId = (row_humanid);
                std::string row_humanname = row_value(row[i++]);				HumanItem.strHumanName = (row_humanname);
                std::string row_contentid = row_value(row[i++]);				HumanItem.strContentId = (row_contentid);
                std::string row_sourcefolder = row_value(row[i++]);				HumanItem.strSourceFolder = (row_sourcefolder);
                int row_available = atoi(row_value(row[i++]).c_str());			HumanItem.strAvailable = INT_TO_STR(row_available);
                double row_speakspeed = atof(row_value(row[i++]).c_str());		HumanItem.strNomalSpeakSpeed = DOUBLE_TO_STR(row_speakspeed);
                double row_seriousspeed = atof(row_value(row[i++]).c_str());	HumanItem.strSeriousSpeakSpeed= DOUBLE_TO_STR(row_seriousspeed);
                std::string row_imagematting = row_value(row[i++]);				HumanItem.strImageMattingFile = (row_imagematting);
                std::string row_keyframe = row_value(row[i++]);					HumanItem.strKeyFrameFile = (row_keyframe);
                std::string row_forground = row_value(row[i++]);				HumanItem.strForegroundFile = (row_forground);
                std::string row_speakmodelpath = row_value(row[i++]);			HumanItem.strSpeakModelFile = (row_speakmodelpath);
                std::string row_mouthmodelfile = row_value(row[i++]);			HumanItem.strMouthModelFile = (row_mouthmodelfile);
                std::string row_facemodelfile = row_value(row[i++]);			HumanItem.strFaceModelFile = (row_facemodelfile);
                vecHumanInfo.push_back(HumanItem);
            }
            else
            {
                bResult = false;
            }
        }
        mysql_free_result(result);				//free result
    }
    else
    {
        bResult = false;
        std::string error = mysql_error(&mysql);
    }

    //=====================
    mysql_close(&mysql);	//close connect

    return bResult;
}
