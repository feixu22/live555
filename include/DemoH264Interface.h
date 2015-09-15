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


/* ��ȡ�����ӿں�������������Ϊ�ص����ܻ������*/

long openStreamHandle(int channelNO, int streamType);
int getStreamData(long lHandle, char* buf, unsigned* bufsize, unsigned* leftbufsize, int sourcetype);
void closeStreamHandle(long lHandle);

class DemoH264Interface 
{
	public:
		static DemoH264Interface* createNew();
		static int CloseInterface();
	
		void InitLive555();
		void startLive555();
		void stopLive555();
		
	private:
		DemoH264Interface();
		~DemoH264Interface();
	public:
		static DemoH264Interface* m_Instance;
		
};


#endif // DemoH264Interface.h 
