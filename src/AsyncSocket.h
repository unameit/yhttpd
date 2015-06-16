#ifndef	ASYNCSOCKET_H
#define	ASYNCSOCKET_H

#include <stddef.h>
#include <sys/uio.h>
#include "Socket.h"

/**
 * Klasa ta, dziedzicz±ca publicznie po klasie Socket, zmienia wywo³ania
 * niektórych jej metod, by poprawnie zachowywa³y siê dla po³±czeñ
 * asynchronicznych (nieblokuj±cych).
*/

class AsyncSocket : public Socket {
public:
	AsyncSocket() {}
	AsyncSocket(int i) : Socket(i) { makeAsync(); }
	AsyncSocket(int domain, int type, int protocol = 0) :
		Socket(domain, type, protocol) { makeAsync(); }

	inline size_t read(void *buf, size_t nbytes) { return Socket::read(buf, nbytes, false); }
	inline size_t readv(const struct iovec *iov, size_t count) { return Socket::readv(iov, count, false); }

	inline size_t write(const void *buf, size_t nbytes) { return Socket::write(buf, nbytes, false); }
	inline size_t writev(const struct iovec *iov, size_t count) { return Socket::writev(iov, count, false); }

	/** Przypisz mi nowy deskryptor, a ja zamieniê go przy okazji na asynchroniczny. */
	virtual AsyncSocket& operator=(int);

private:
	;
};

#endif
