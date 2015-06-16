#ifndef	EXECCHILDTHREAD_H
#define	EXECCHILDTHREAD_H

class Properties;

void execChildThread(int sockfd, const char *hostName, const Properties *mimeTypes, const char *documentRoot, const char *userDir);

#endif
