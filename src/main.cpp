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

#include "DemoH264Interface.h"

int main(int argc, char* argv[])
{

	// Init
	// ���һЩ��Ҫ���õ�rtsp������Ϣ�����û��������� �˿ڵȣ�ͨ����������
	void* param = NULL;
	DemoH264Interface::createNew()->InitLive555(param);

	// start 
	if( -1 == DemoH264Interface::createNew()->startLive555())
	{
		DBG_LIVE555_PRINT(" start live555 moudle failed!\n");
		return 0;
	}

	//stop 
	DemoH264Interface::createNew()->stopLive555();
	return 0;
}
