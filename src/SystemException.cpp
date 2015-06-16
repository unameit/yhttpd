#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "SystemException.h"

void SystemException::addErrnoMsg()
{
	char tmp[20];
	if (errno > 0) {
		sprintf(tmp, "%d", errno);
		string += "\n(errno=";
		string += tmp;
		string += "; ";
		string += strerror(errno);
		string += ")";
	}
}
