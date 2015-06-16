#include <string.h>
#include "SystemException.h"
#include "AsyncMessageQueue.h"


void AsyncMessageQueue::init()
{
	iov[0].iov_len = sizeof(struct sockaddr_in);

	msg.msg_name = NULL;							// optional address
	msg.msg_namelen = 0;							// size of address
	msg.msg_iov = iov;								// scatter/gather array
	msg.msg_iovlen = sizeof iov / sizeof iov[0];	// # elements in msg_iov

#ifndef	OLD_MSGHDR
	mymsg.cmsg.cmsg_len = sizeof mymsg;				// data byte count, including hdr
	mymsg.cmsg.cmsg_level = SOL_SOCKET;				// originating protocol
	mymsg.cmsg.cmsg_type = SCM_RIGHTS;				// protocol-specific type

	msg.msg_control = &mymsg;						// ancillary data
	msg.msg_controllen = sizeof mymsg;				// ancillary data buffer len
#else
	msg.msg_accrightslen = sizeof(int);
#endif
}



void AsyncMessageQueue::sendDescriptor(int d, const struct sockaddr_in *peerAddr)
{
	init();
	iov[0].iov_base = (char *) peerAddr;
#ifndef	OLD_MSGHDR
	msg.msg_flags = 0;
	mymsg.fd = d;
#else
	msg.msg_accrights = (char *) &d;
#endif
	if (sendmsg(fd, &msg, 0) != iov[0].iov_len)
		throw SystemException("sendmsg() failed!");
}


int AsyncMessageQueue::recvDescriptor(struct sockaddr_in *peerAddr)
{
	init();
	iov[0].iov_base = (char *) peerAddr;
#ifdef	OLD_MSGHDR
	int d;
	msg.msg_accrights = (char *) &d;
#endif
	if (recvmsg(fd, &msg, 0) != iov[0].iov_len)
		throw SystemException("recvmsg() failed!");
#ifndef	OLD_MSGHDR
	return mymsg.fd;
#else
	return d;
#endif
}


AsyncMessageQueue& AsyncMessageQueue::operator=(int newsockfd)
{
	AsyncSocket::operator=(newsockfd);
	return *this;
}
