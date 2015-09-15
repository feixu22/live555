/*
 * =====================================================================================
 *
 *       Filename:  DemoH264FrameSource.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015��09��07�� 21ʱ48��13��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  max_min_, 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __DEMOH264FRAMESOURCE_H__
#define __DEMOH264FRAMESOURCE_H__


#include "FramedSource.hh"

class DemoH264FrameSource:public FramedSource 
{
	public:
		static DemoH264FrameSource* createNew(UsageEnvironment& env, const char* fileName, 
				unsigned preferredFrameSize = 0,  unsigned playTimePerFrame = 0);
	protected:
		DemoH264FrameSource(UsageEnvironment& env, const char* fileName,
				unsigned preferredFrameSize, unsigned playTimePerFrame);
		~DemoH264FrameSource();
	private:
		virtual void doGetNextFrame();
	public:
		void doStopGetFrame();
	public:
		char *fDataBuf;
		long fSourceHandle; 
		int fLastBufSize;
		int fLeftDataSize;
		int fSourceType;
		int fFirstFrame;
		
};


#endif // DemoH264FrameSource.h
