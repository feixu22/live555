/*
 * =====================================================================================
 *
 *       Filename:  DemoH264Interface.h
 *
 *    Description:  �ⲿ����live555�Ľӿڹ����� 
 *
 *        Version:  1.0
 *        Created:  2015��09��15�� 23ʱ32��34��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization: max_min_, 
 *
 * =====================================================================================
 */
#ifndef __DEMOH264INTERFACE_H__
#define __DEMOH264INTERFACE_H__
#include <stdio.h>

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"

#ifdef DEBUG 
#define DBG_LIVE555_PRINT(pFmt, ...) \
	do \
	{\
		fprintf(stderr, "[Live555_DBG]-[%s %d]:"pFmt, __FILE__, __LINE__, ##__VA_ARGS__);\
		fflush(stderr);\
	}while(0)
#else 
#define DBG_LIVE555_PRINT(pFmt,...)  
#endif 

// ʵʱ�����ļ�������֡ͷ��Ϣ 
typedef struct FrameHead_S_
{
	unsigned short FrameFlag;		//0x3448 magic data
	unsigned char  FrameType;		//I-0x7 p--0x8 b--0xb A--0xa
	unsigned char  Res[1];			//����
	unsigned int   FrameNo;			//֡��
	unsigned int   SysTime;			//ϵͳʱ��
	unsigned int   FrameLen;		//����������
	unsigned long long TimeStamp;		//ʱ���
	
	unsigned short ImageWidth;	//ͼ����
	unsigned short ImageHeight;	//ͼ��߶�
	unsigned char  EncodeType;	//��Ƶ�����ʽ0-h264 1-mjpeg 2-jpeg
	unsigned char  FrameRate;	//֡��(����)
	unsigned char  Pal;			//��ʽ		0-n��1-pal��
	unsigned char  Res2[1];
}FrameHead_S;


/* ��ȡ�����ӿں�������������Ϊ�ص����ܻ������*/
long openStreamHandle(int channelNO, int streamType);
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype);
void closeStreamHandle(long lHandle);

class DemoH264Interface 
{
	public:
		static DemoH264Interface* createNew();
		static int CloseInterface();
	
		void InitLive555(void *param);
		int  startLive555();
		int  stopLive555();
		
	private:
		DemoH264Interface();
		~DemoH264Interface();
	public:
		static DemoH264Interface* m_Instance;
	private:
		//live555
		bool m_liveServerFlag;
		TaskScheduler* m_scheduler;
		UsageEnvironment* m_env;
		UserAuthenticationDatabase* m_authDB;
		DemoH264RTSPServer* m_rtspServer;
		portNumBits m_rtspServerPortNum; // rtsp port 
		
};


#endif // DemoH264Interface.h 
