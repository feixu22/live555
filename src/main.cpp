/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015��09��07�� 23ʱ03��31��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  max_min_, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"
#include "DemoH264MediaSubsession.h"


int main(int argc, char* argv[])
{
	
	printf(" live555 stream start\n");
	
	// Begin by setting up the live555 usage environment 
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment* env    = BasicUsageEnvironment::createNew(*scheduler);
	

	UserAuthenticationDatabase* authDB = NULL;
#if ACCESS_CONTROL   // ��֤
	authDB = new UserAuthenticationDatabase;
	authDB->addUserRecord(argv[1], argv[2]);
#endif 

	RTSPServer* rtspServer = NULL;
	portNumBits rtspServerPortNum = 554; // rtsp port 

	// ����RTSP����
	rtspServer = RTSPServer::createNew(*env, rtspServerPortNum, authDB);
	if( rtspServer == NULL)
	{
		*env << " create RTSPServer Failed:" << env->getResultMsg() << "\n";
		return 0;
	}

	
	const char* decription = " Session Test By live555 Stream";
	

	 //H264 Subsession 
	const char* streamName = "h264_streaming";
	const char* inputName = "tc10.264";

	
	ServerMediaSession *sms = ServerMediaSession::createNew(*env, streamName, streamName, decription);

	// ����Լ�����������MediaSubsession�࣬����ӵ�ServerMediaSession
	// ����client�����Ϲ�����ʱ�򣬻����server��lookupѰ�Ҵ�streamName��subsession
	sms->addSubsession(DemoH264MediaSubsession::createNew(*env, inputName, false));
	

	rtspServer->addServerMediaSession(sms);
	
	char* url = rtspServer->rtspURL(sms);	
	*env <<  "URL:" << url << "\n";
	

	// loop and not come back~
	env->taskScheduler().doEventLoop();
	
	return 0;
}
