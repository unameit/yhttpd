#ifndef	EXECPARENTTHREAD_H
#define	EXECPARENTTHREAD_H

class ConnectionWithChild;
class AsyncSocket;

void execParentThread(ConnectionWithChild **children, const int maxChildren, AsyncSocket *serverSocket);

#endif
