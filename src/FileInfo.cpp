#include <sys/stat.h>
#include <unistd.h>
#include "SString.h"
#include "Exception.h"
#include "SystemException.h"
#include "FileInfo.h"


FileInfo::FileInfo(const char *filename)
{
	if (NULL == filename)
		return;
	stat(filename);
}


FileInfo::FileInfo(int fd)
{
	stat(fd);
}


FileInfo& FileInfo::operator=(const char *filename)
{
	if (NULL == filename)
		throw Exception("Null pointer!");
	stat(filename);
	return *this;
}


FileInfo& FileInfo::operator=(int fd)
{
	stat(fd);
	return *this;
}


void FileInfo::stat(const char *filename)
{
	struct stat buf;
	if (-1 == ::stat(filename, &buf))
		throw SystemException(SString("Can't stat file \"%s\"!", filename));
	size = buf.st_size;
	mtime = buf.st_mtime;
	dir = S_ISDIR(buf.st_mode);
}


void FileInfo::stat(int fd)
{
	struct stat buf;
	if (-1 == ::fstat(fd, &buf))
		throw SystemException(SString("Can't stat file descriptor %d!", fd));
	size = buf.st_size;
	mtime = buf.st_mtime;
	dir = S_ISDIR(buf.st_mode);
}
