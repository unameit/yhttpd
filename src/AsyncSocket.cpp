#include "AsyncSocket.h"


AsyncSocket& AsyncSocket::operator=(int newsockfd)
{
	Socket::operator=(newsockfd);
	makeAsync();
	return *this;
}
