#ifndef	STRING_H
#define	STRING_H

#include <stdlib.h>

/**
 * Klasa u³atwiaj±ce korzystanie z danych z³o¿onych z ci±gów znaków. Podobna
 * do klasy SString, jednak tutaj bufor znakowy jest przydzielany
 * dynamicznie (malloc), za¶ jego wielko¶æ nieograniczona.
 */

class String {
public:
	String(const char *);
	~String() { free(s); }
	operator const char*() const { return s; }
	size_t length() const { return len; }
private:
	char	*s;
	size_t	len;
};

#endif
