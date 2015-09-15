
#include "DemoH264RTSPServer.h"


DemoH264RTSPServer* DemoH264RTSPServer::createNew(UsageEnvironment& env,  Port rtspPort, 
		UserAuthenticationDatabase* authDatabase, unsigned reclamationTestSeconds)
{
	int rtspSock = -1;

	rtspSock = setUpOurSocket(env, rtspPort);
	if(rtspSock == -1 )
		return NULL;
	return new DemoH264RTSPServer(env, rtspSock, rtspPort, authDatabase, reclamationTestSeconds);
	
}


DemoH264RTSPServer::DemoH264RTSPServer(UsageEnvironment& env, int ourSock, Port rtspPort, UserAuthenticationDatabase* authDatabase,
		unsigned reclamationTestSeconds):RTSPSever(env, ourSock, rtspPort, authDatabase, reclamationTestSeconds), fRTSPServerState(true)
{
	 printf("create DemoH264RTSPServer \n");
}

DemoH264RTSPServer::~DemoH264RTSPServer()
{

}


ServerMediaSession* DemoH264RTSPServer::lookupServerMediaSession(const char* streamName)
{
	// streamName, ΪURL��ַ������ַ��� �� 
	// rtsp://10.0.2.15/streamNameCH00StreamType00, ��streamName = "streamNameCH00StreamType00";
	// ���ͻ��˷���url����ʱ�����Խ���streamName���ж������Ǹ�ͨ������������

	// 1 ����url �����ﲻ���������Լ��ص��ӿڽ�������
	int channelNO   = 0;  // ͨ����
	int streamType  = 0;  // ��������
	int videoType   = 0;  // ��Ƶ or ��Ƶ
	int requestType = 0;  // �������� ʵʱԤ�� or �ط�

	ServerMediaSession* sms = NULL;

	sms = RTSPServer::lookupServerMediaSession(streamName);
	if ( NULL == sms )
	{
		switch(requestType)
		{
		case 0:  // realtime 
			sms = ServerMediaSession::createNew(envir(), streamName, NULL, NULL);
			DemoH264MediaSubsession *session = createNew(envir(), streamType, videoType, channelNO,false);

			sms->addSubsession(session);

			this->addServerMediaSession(sms);
			break;
		case 1: // play back
			break;
				
			
		}
	}
}


DemoH264RTSPServer::DemoH264RTSPClientSession* DemoH264RTSPServer::DemoH264RTSPClientSession::createNew(unsigned clientSessionID)
{
	DemoH264RTSPServer::DemoH264RTSPClientSession* client = new DemoH264RTSPServer::DemoH264RTSPClientSession(*this, clientSessionID);
	fClientSessionList.push_back(client);
	printf("add client session success!\n");
	return client;
}

int DemoH264RTSPServer::stopDemoH264RTSPServer(void* param)
{
	// �ƺ���
	if(NULL == param)
	{
		printf(" param error !\n");
		return -1;
	}
	DemoH264RTSPServer* pRtspServer = (DemoH264RTSPServer*)param;
	// ɾ�����еĿͻ��˵�session
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		pRtspServer->fClientSessionList.begin();
	for(pos; pos != pRtspServer->fClientSessionList.end(); pos ++ )
	{
		DemoH264RTSPServer::DemoH264RTSPClientSession* tmp = *pos;
		delete tmp;
	}

	delete pRtspServer; //

	return 0;
	
}

DemoH264RTSPServer::DemoH264RTSPClientSession::DemoH264RTSPClientSession(DemoH264RTSPServer& rtspServer, 
	unsigned clietnSessionID):RTSPServer::RTSPClientSession(rtspServer, clietnSessionID);
{
}

DemoH264RTSPServer::DemoH264RTSPClientSession::~DemoH264RTSPClientSession()
{
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		(DemoH264RTSPServer&)fOurServer.fClientSessionList.begin();
	for(pos; pos != (DemoH264RTSPServer&)fOurServer.fClientSessionList.end(); pos ++ )
	{
		if ((*pos)->fOurSessionId == this->fOurSessionId) 
		{
			(DemoH264RTSPServer&)fOurServer.fClientSessionList.erase(pos);
			printf("client session has been delete !\n");
			break ;
		}
	}
}

