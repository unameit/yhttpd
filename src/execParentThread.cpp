#include "ParentThread.h"
#include "execParentThread.h"


void execParentThread(ConnectionWithChild **children, const int maxChildren, AsyncSocket *serverSocket)
{
	ParentThread parentThread(serverSocket, children, maxChildren);
	parentThread.main();
}
