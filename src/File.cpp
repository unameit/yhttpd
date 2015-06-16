#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include "SystemException.h"
#include "SString.h"
#include "File.h"


File::File(const char *filename, int flags)
{
	open(filename, flags);
}


File& File::open(const char *filename, int flags)
{
	fd = ::open(filename, flags);
	if (-1 == fd)
		throw SystemException(SString("Can't open file \"%s\"!\n"
			"open(\"%s\", 0%03o);", filename, filename, flags));

	struct stat	statBuf;
	if (-1 == fstat(fd, &statBuf))
		throw SystemException(SString("Can't stat file \"%s\"!\n", filename));

	size = statBuf.st_size;
	mtime = statBuf.st_mtime;

	return *this;
}


File& File::rewind()
{
	if (-1 == ::lseek(fd, 0, SEEK_SET))
		throw SystemException(SString("Can't rewind file!\nlseek(%d, 0, SEEK_SET);", fd));
	return *this;
}


File& File::seek(off_t offset, int whence)
{
	if (-1 == ::lseek(fd, offset, whence))
		throw SystemException(SString("lseek(%d, %d, %d) failed!", fd, offset, whence));
	return *this;
}


File& File::setpos(off_t offset)
{
	if (-1 == ::lseek(fd, offset, SEEK_SET))
		throw SystemException(SString("lseek(%d, %d, SEEK_SET) failed!", fd, offset));
	return *this;
}


File& File::lock(off_t len, bool mandatory)
{
	if (mandatory)
		if (-1 == ::fchmod(fd, S_ISGID | S_IRUSR | S_IWUSR))
			throw SystemException(SString("Can't change permissions!\nfchmod(%d, S_ISGID | S_IRUSR | S_IWUSR);", fd));
	if (-1 == ::lockf(fd, F_LOCK, len))
		throw SystemException(SString("Can't lock!\nlockf(%d, F_LOCK, %d);", fd, len));
	return *this;
}


File& File::unlock(off_t len)
{
	if (-1 == ::lockf(fd, F_ULOCK, len))
		throw SystemException(SString("Can't unlock!\nlockf(%d, F_ULOCK, %d);", fd, len));
	return *this;
}


File& File::operator=(int i)
{
	Descriptor::operator=(i);
	return *this;
}
