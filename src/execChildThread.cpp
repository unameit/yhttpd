#include "ChildThread.h"
#include "execChildThread.h"

#ifdef	DEBUG
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "getChildNo.h"
#endif


void execChildThread(int sockfd, const char *hostName, const Properties *mimeTypes, const char *documentRoot, const char *userDir)
{
	ChildThread childThread(sockfd, hostName, mimeTypes, documentRoot, userDir);
#ifdef	DEBUG	
	char tmp[40];
	sprintf(tmp, "debug/%d", getChildNo());
	mkdir(tmp, 0700);
	chdir(tmp);
	// ¿eby ka¿de dziecko mog³o zrzuciæ swojego w³asnego core'a
#endif

	childThread.main();
}
