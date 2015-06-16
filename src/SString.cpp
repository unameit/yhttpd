#include <stdio.h>
#include <stdarg.h>
#include "SString.h"


SString::SString()
{
	s[0] = s[sizeof s - 1] = '\0';
}


SString::SString(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsprintf(s, fmt, ap);
	s[sizeof s - 1] = '\0';
	va_end(ap);
}


SString& SString::operator=(const char *str)
{
	strncpy(s, str, sizeof s - 1);
	return *this;
}


SString& SString::operator+=(const char *str)
{
	strncat(s, str, sizeof s - 1 - strlen(s));
	return *this;
}


SString::operator const char*() const
{
	return s;
}
