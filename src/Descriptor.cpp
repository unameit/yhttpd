#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include "SystemException.h"
#include "EOFException.h"
#include "SString.h"
#include "Descriptor.h"


void Descriptor::makeAsync()
{
	if (-1 == ::fcntl(fd, F_SETFL, O_NDELAY))
		throw SystemException("fcntl(fd, F_SETFL, O_NDELAY) failed!");
}


Descriptor& Descriptor::operator=(int newfd)
{
	close();
	fd = newfd;
	return *this;
}


void Descriptor::close()
{
	if (-1 == fd)	return;

	if (-1 == ::close(fd))
		throw SystemException(SString("Can't close descriptor %d!", fd));

	fd = -1;
}


int Descriptor::read()
{
	char c;
	switch(::read(fd, &c, 1)) {
		case 1:
			return c;
		case 0:
			throw EOFException("End of file reached!");
		case -1:
		default:
			throw SystemException("Read error!\nread(fd, (char *), 1);");
	}
}


size_t Descriptor::read(void *buffer, const size_t nbytes, bool full)
{
	const size_t nread = ::read(fd, buffer, nbytes);

	if ((size_t) -1 == nread)
		throw SystemException(SString("Read error!\nread(fd, 0x%X, %u);", fd, buffer, nbytes));

	if (full && nread < nbytes)
		throw EOFException("End of file reached!");

	return nread;
}


size_t Descriptor::readv(const struct iovec *iov, const size_t count, bool full)
{
	const size_t nread = ::readv(fd, iov, count);
	size_t nbytes = 0;

	for(size_t i = 0; i < count; ++i)
		nbytes += iov[i].iov_len;

	if ((size_t) -1 == nread)
		throw SystemException(SString("Read error!\nreadv(%d, 0x%X, %u);", fd, iov, count));

	if (full && nread < nbytes)
		throw EOFException("End of file reached!");

	return nread;
}


void Descriptor::write(u_char c)
{
	switch(::write(fd, &c, 1)) {
		case 1:
			return;
		case 0:
		case -1:
		default:
			throw SystemException("Write error!\nwrite(fd, &c, 1);");
	}
}


size_t Descriptor::write(const void *buffer, const size_t nbytes, bool full)
{
	const size_t written = ::write(fd, buffer, nbytes);

	if ((size_t) -1 == written)
		throw SystemException(SString("Write error!\nwrite(%d, 0x%X, %u);", fd, buffer, nbytes));

	if (full && written < nbytes)
		throw SystemException(SString("Write error!\n%u = write(%d, 0x%X, %u);", written, fd, buffer, nbytes));

	return written;
}


size_t Descriptor::writev(const struct iovec *iov, const size_t count, bool full)
{
	const size_t written = ::writev(fd, iov, count);
	size_t nbytes = 0;

	for(size_t i = 0; i < count; ++i)
		nbytes += iov[i].iov_len;

	if ((size_t) -1 == written)
		throw SystemException(SString("Write error!\nwritev(%d, 0x%X, %u);", fd, iov, count));

	if (full && written < nbytes)
		throw SystemException(SString("Write error!\nwritev(%d, 0x%X, %u);", fd, iov, count));

	return written;
}
