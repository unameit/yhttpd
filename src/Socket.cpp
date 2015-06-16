#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>                     
#include <netdb.h>
#include "SystemException.h"
#include "Socket.h"


Socket::Socket(int domain, int type, int protocol)
{
	if (-1 == (fd = ::socket(domain, type, protocol)))
		throw SystemException("Can't create a socket!");
}


void Socket::connect(const struct sockaddr_in *addr)
{
	if (-1 == ::connect(fd, (struct sockaddr *) addr, sizeof *addr))
		throw SystemException("connect() failed!");
}


void Socket::connect(const char *hostName, ushort port)
{
	struct sockaddr_in addr;
	convert(&addr, hostName, port);
	connect(&addr);
}


void Socket::convert(struct sockaddr_in *addr, const char *hostName, ushort port) const
{
	memset(addr, 0, sizeof *addr);
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);

	if (-1 == (signed) (addr->sin_addr.s_addr = inet_addr(hostName))) {

		struct hostent *host;

		if (NULL == (host = gethostbyname(hostName)))
			throw SystemException(SString("gethostbyname(\"%s\") failed!", hostName));

		if (host->h_length > (signed) sizeof addr->sin_addr)
			throw Exception("Funkcja systemowa poda³a zbyt d³ugi adres!\n"
							"To siê nie powinno zdarzyæ.\n"
							"Oznacza to prawdopobnie b³±d w bibliotekach systemowych");

		memcpy(&addr->sin_addr.s_addr, host->h_addr, host->h_length);
	}
}


void Socket::bind(const struct sockaddr_in *addr)
{
	if (-1 == ::bind(fd, (struct sockaddr *) addr, sizeof *addr))
		throw SystemException("bind() failed!");
}


void Socket::bind(const char *hostName, ushort port)
{
	struct sockaddr_in addr;
	convert(&addr, hostName, port);
	bind(&addr);
}


void Socket::listen(int backlog)
{
	if (-1 == ::listen(fd, backlog))
		throw SystemException(SString("listen(%d, %d) failed!", fd, backlog));
}


int Socket::accept(struct sockaddr_in *client)
{
	socklen_t addrLen = sizeof *client;
	int newsockfd = ::accept(fd, (struct sockaddr *) client, &addrLen);
	if (-1 == newsockfd)
		throw SystemException("accept() failed!");;
	return newsockfd;
}


int Socket::accept()
{
	struct sockaddr_in client;
	return accept(&client);
}


Socket& Socket::operator=(int i)
{
	Descriptor::operator=(i);
	return *this;
}
