#include "DemoH264Interface.h"
#include "DemoH264RTSPServer.h"

/*��ʵʱ�������*/
long openStreamHandle(int channelNO, int streamType)
{
	//��ʼʵʱ����һЩ׼������:��ȡ������ʵʱ�����ľ�����������ֱ��get����

	// ��������ԣ����Ի������Լ�������ļ���������������ÿ�ζ��Ƕ�һ֡����
	// �ļ�����ʽΪ FrameHeader_S + H264 + FrameHeader_S + H264 ...
	FILE* fp = fopen("stream264file.h264", "rb+");
	if (NULL == fp )
	{
		DBG_LIVE555_PRINT("open streamhandle failed!\n");
		return -1;
	}

	return (long)fp;
		
}
/*ʵʱ��ȡһ֡����*/
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype)
{
	if(lHandle <= 0)
	{
		DBG_LIVE555_PRINT(" lHandle error !\n");
		return -1;
	}
	FrameHead_S stFrameHead;
	memset(&stFrameHead, 0, sizeof(FrameHead_S));
	FILE* fp = (FILE*)lHandle;
	int readlen = 0;
	// 1���ȶ�ȡһ֡���ݵ�ͷ��Ϣ
	readlen = fread(&stFrameHead, 1, sizeof(FrameHead_S), fp);
	if( readlen != sizeof(FrameHead_S))
	{
		DBG_LIVE555_PRINT(" read Frame Header Failed !\n");
		return -1;
	}

	//2����ȡһ֡H264ʵʱ����
	if(stFrameHead.FrameLen > 2*1024*1024) // ��source��databufָ������2M
	{
		DBG_LIVE555_PRINT("data is too long:framlen=%d\n", stFrameHead.FrameLen);
		//���·����ڴ洦��
		return 0;
	}
	readlen = fread(buf, 1, stFrameHead.FrameLen, fp);
	if(readlen != stFrameHead.FrameLen)
	{
		DBG_LIVE555_PRINT("read Frame rawdata Failed!\n");
		return -1;
	}

	return stFrameHead.FrameLen;
}
/*�ر��������*/
void closeStreamHandle(long lHandle)
{
	//һЩ�ر�������������
	fclose((FILE*)lHandle);
}


DemoH264Interface* DemoH264Interface::m_Instance = NULL;

DemoH264Interface* DemoH264Interface::createNew()
{
	if(NULL == m_Instance)
	{
		m_Instance = new DemoH264Interface();
	}

	return m_Instance;
}


DemoH264Interface::DemoH264Interface()
{
	m_liveServerFlag = false;
}

DemoH264Interface::~DemoH264Interface()
{
}

void DemoH264Interface::InitLive555(void *param)
{
	//��ʼ��
		
	DBG_LIVE555_PRINT(" ~~~~live555 stream start\n");
	
	// Begin by setting up the live555 usage environment 
	m_scheduler = BasicTaskScheduler::createNew();
	m_env = BasicUsageEnvironment::createNew(*m_scheduler);
		
#if ACCESS_CONTROL   // ��֤
	m_authDB = new UserAuthenticationDatabase;
	m_authDB->addUserRecord("username", "password");
#endif 

	m_rtspServer = NULL;
	m_rtspServerPortNum = 554; // �����޸�
	m_liveServerFlag = true;
}


int DemoH264Interface::startLive555()
{
	if( !m_liveServerFlag)
	{
		DBG_LIVE555_PRINT("Not Init the live server !\n");
		return -1; 
	}

	// ����RTSP����
	m_rtspServer = DemoH264RTSPServer::createNew(*m_env, m_rtspServerPortNum, m_authDB);
	
	if( m_rtspServer == NULL)
	{
		// *m_env << " create RTSPServer Failed:" << m_env->getResultMsg() << "\n";
		DBG_LIVE555_PRINT("create RTSPServer Failed:%s\n", m_env->getResultMsg());
		return -1;
	}


	// loop and not come back~
	m_env->taskScheduler().doEventLoop();
	
	return 0;
}

int DemoH264Interface::stopLive555()
{
	if(m_liveServerFlag)
	{
		if(m_rtspServer)
			m_rtspServer->stopDemoH264RTSPServer();
		m_liveServerFlag = false;	
	}
}




