
#include "DemoH264RTSPServer.h"
#include "DemoH264Interface.h"
#include "DemoH264MediaSubsession.h"


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
		unsigned reclamationTestSeconds):RTSPServer(env, ourSock, rtspPort, authDatabase, reclamationTestSeconds), fRTSPServerState(true)
{
	 DBG_LIVE555_PRINT("create DemoH264RTSPServer \n");
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
	switch(requestType)
	{
		case 0:  // realtime 
			sms = RTSPServer::lookupServerMediaSession(streamName);
			if ( NULL == sms )
			{
				sms = ServerMediaSession::createNew(envir(), streamName, NULL, NULL);
				DemoH264MediaSubsession *session = DemoH264MediaSubsession::createNew(envir(), streamType, videoType, channelNO, false);

				sms->addSubsession(session);
				this->addServerMediaSession(sms);
			}
			break;
		case 1:
			// play back
			DBG_LIVE555_PRINT("play back request !\n");
			break;
		default:
			DBG_LIVE555_PRINT("unknown  request type!\n");
			break;

	}
	return sms;
}


DemoH264RTSPServer::DemoH264RTSPClientSession* DemoH264RTSPServer::createNewClientSession(unsigned clientSessionID, int clientSocket, struct sockaddr_in clientAddr)
{
	DemoH264RTSPServer::DemoH264RTSPClientSession* client = new DemoH264RTSPClientSession(*this, clientSessionID, clientSocket, clientAddr);
	fClientSessionList.push_back(client);
	DBG_LIVE555_PRINT("add client session success!\n");
	return client;
}

int DemoH264RTSPServer::stopDemoH264RTSPServer()
{
	// ɾ�����еĿͻ��˵�session
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		this->fClientSessionList.begin();
	for(pos; pos != this->fClientSessionList.end(); pos ++ )
	{
		DemoH264RTSPServer::DemoH264RTSPClientSession* tmp = *pos;
		delete tmp;
	}

	delete this; //

	return 0;
	
}

DemoH264RTSPServer::DemoH264RTSPClientSession::DemoH264RTSPClientSession(DemoH264RTSPServer& rtspServer,unsigned clietnSessionID, int clientSocket, struct sockaddr_in clientAddr):
	RTSPServer::RTSPClientSession(rtspServer, clietnSessionID, clientSocket, clientAddr)
{
	
}

DemoH264RTSPServer::DemoH264RTSPClientSession::~DemoH264RTSPClientSession()
{
	std::list<DemoH264RTSPServer::DemoH264RTSPClientSession*> ::iterator pos =
		((DemoH264RTSPServer&)fOurServer).fClientSessionList.begin();
	for(pos; pos != ((DemoH264RTSPServer&)fOurServer).fClientSessionList.end(); pos ++ )
	{
		if ((*pos)->fOurSessionId == this->fOurSessionId) 
		{
			((DemoH264RTSPServer&)fOurServer).fClientSessionList.erase(pos);
			DBG_LIVE555_PRINT("client session has been delete !\n");
			break ;
		}
	}
}

