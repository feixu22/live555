#include "DemoH264MediaSubsession.h"
#include "DemoH264FrameSource.h"
#include "H264VideoStreamFramer.hh"


DemoH264MediaSubsession::DemoH264MediaSubsession(UsageEnvironment& env, const char*fileName, bool reuseFirstSource)
	:FileServerMediaSubsession(env, fileName, reuseFirstSource)
{
	strcpy(fFileName, fileName);
}

DemoH264MediaSubsession::~DemoH264MediaSubsession()
{
}


DemoH264MediaSubsession* DemoH264MediaSubsession::createNew(UsageEnvironment& env, const char* fileName, bool reuseFirstSource)
{
	DemoH264MediaSubsession* sms = new DemoH264MediaSubsession(env, fileName, reuseFirstSource);
	return sms;
}


FramedSource* DemoH264MediaSubsession::createNewStreamSource(unsigned clientsessionId, unsigned& estBitrate)
{
	estBitrate = 1000;
	
	DemoH264FrameSource* source = DemoH264FrameSource::createNew(envir(), fFileName);
	if ( source == NULL )
	{
		envir() << " new source failed!\n";
	}
	
	return H264VideoStreamFramer::createNew(envir(), source);
}


RTPSink* DemoH264MediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource)
{
	return NULL;
}




