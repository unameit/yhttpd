#include "EOFException.h"
#include "ConnectionWithChild.h"

#include "debug.h"
#include <unistd.h>


ConnectionWithChild::ConnectionWithChild(int sockfd, int maxChildConnections) :
	msgq(sockfd),
	openConnections(0),
	maxConnections(maxChildConnections)
{
	;
}


bool ConnectionWithChild::sendSocket(int sockfd, const struct sockaddr_in *p)
{
	if (-1 != socketToSend)		// czy mamy co¶ do zaleg³ego do wys³ania?
		return false;			// tak, wiêc nie przyjmê tego zg³oszenia.
	if (maxConnections == openConnections)
		return false;			// tego te¿ nie, bo by³oby za du¿o
	socketToSend = sockfd;		// nie, wiêc zapamiêtaj i za chwilê wy¶lij.
	peer = *p;
	return true;
}


void ConnectionWithChild::onWriteable()
{
	msgq.sendDescriptor(socketToSend, &peer);
	socketToSend.close();
}


void ConnectionWithChild::onReadable()
{
	try {
		openConnections = msgq.recvInt();
	} catch(EOFException e) { ; }
}
