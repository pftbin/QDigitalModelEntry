#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

//AWS
#include "aws/core/Aws.h"
#define USE_IMPORT_EXPORT //解决aws无法解析 API_VERSION_HEADER问题

class awsUpload
{
public:
	awsUpload(void);
	virtual ~awsUpload(void);

	bool SetAWSConfig(std::string strWebPrefix, std::string strEndPoint, std::string aws_url, std::string aws_ak, std::string aws_sk, std::string aws_bucket, std::string aws_region = "");
	
	bool UploadFile(std::string object_folder, std::string localfile_path, std::string& urlfile_path, bool multipart = true);
	bool DownloadFile(std::string object_folder, std::string object_name, std::string localfile_path);
	bool RemoveFile(std::string objectfile_path);

	bool UploadFolder(std::string object_folder, std::string local_folder);
	bool DownloadFolder(std::string object_folder, std::string local_folder);
	bool RemoveFolder(std::string object_folder);

protected:
	std::string		m_strWebPrefix;
	std::string     m_strEndPoint;
	std::string		m_strUrl;
	std::string		m_strAK;
	std::string		m_strSK;
	std::string		m_strBucket;
	std::string		m_strRegion;

protected:
	Aws::SDKOptions	m_aws_options;
	bool put_s3_object(std::string objectfile_path, std::string localfile_path, std::string& urlfile_path);
	bool put_s3_object_multipart(std::string objectfile_path, std::string localfile_path, std::string& urlfile_path);
	bool get_s3_object(std::string objectfile_path, std::string localfile_path);
	bool get_s3_object_multipart(const std::string objectfile_path, const std::string localfile_path);
	bool del_s3_object(std::string objectfile_path);
	bool list_s3_folder(std::string object_folder, std::vector<std::string>& vecobject_name);
};


