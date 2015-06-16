#include <stdlib.h>
#include <string.h>
#include "SystemException.h"
#include "String.h"


String::String(const char *str)
{
	len = strlen(str);
	s = (char *) malloc(len + 1);
	if (NULL == s)
		throw SystemException("malloc() failed!");
	strcpy(s, str);
}
