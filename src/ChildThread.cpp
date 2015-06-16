#include <netinet/in.h>			// sockaddr_in
#include "AsyncConnection.h"
#include "HTTPConnection.h"
#include "SystemException.h"
#include "getChildNo.h"
#include "ChildThread.h"


#ifdef	DEBUG

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "debug.h"

#endif


ChildThread::ChildThread(int sockfd, const char *init_hostName,
	const Properties *init_mimeTypes, const char *init_documentRoot,
	const char *init_userDir) :
		msgq(sockfd),
		intToSend(-1),
		hostName(init_hostName),
		mimeTypes(init_mimeTypes),
		documentRoot(init_documentRoot),
		userDir(init_userDir)
{
	manager.add(this);
	closedConnections.add( new HTTPConnection(hostName, mimeTypes, documentRoot, userDir) );

	// nie sprawdzam, czy new nie zwróci³ NULL, bo zrobi to za mnie Vector::add()
}


ChildThread::~ChildThread()
{
	for(int i = openedConnections.getCount(); i >= 0; --i)
		delete (HTTPConnection *) openedConnections[i];
	for(int i = closedConnections.getCount(); i >= 0; --i)
		delete (HTTPConnection *) closedConnections[i];
}


void ChildThread::onReadable()
{
	HTTPConnection *c;
	struct sockaddr_in peer;
	int sockfd = msgq.recvDescriptor(&peer);

	updateConnections();

#ifdef	DEBUG
	fprintf(fdebuglog, "[%3d] recv fd=%d (%s)  mam %u\n", getChildNo(), sockfd, inet_ntoa(peer.sin_addr), openedConnections.getCount() + 1);
#endif

	if (closedConnections.getCount() == 0) {	// s± jakie¶ zamkniête po³±czenia?
		// nie, wiêc stwórz nowe
		c = new HTTPConnection(hostName, mimeTypes, documentRoot, userDir);
	} else {
		// tak, wiêc je przenie¶
		c = (HTTPConnection *) closedConnections.remove(0);
	}

	openedConnections.add(c);
	*c = sockfd;
	c->setPeer(&peer);	// dla acces.log
	manager.add(c);

	intToSend = openedConnections.getCount();
}


void ChildThread::onWriteable()
{
	msgq.sendInt(intToSend);
	intToSend = -1;
}


void ChildThread::updateConnections()
{
	AsyncConnection *c;
	for(uint i = 0; i < openedConnections.getCount(); ++i) {
		c = (HTTPConnection *) openedConnections[i];
		if (-1 == (int) *c && c->isFinalized())
			closedConnections.add( openedConnections.remove(i--) );
	}
}
