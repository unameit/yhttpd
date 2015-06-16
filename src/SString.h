#ifndef	SSTRING_H
#define	SSTRING_H

#include <string.h>

/**
 * Obiekty tej klasy u³atwiaj± operacje na ci±gach znaków, poprzez
 * zdefiniowanie kilku operatorów oraz posiadanie statycznego bufora o
 * wielko¶ci 512 bajtów. Potrafi± te¿ formatowaæ tekst tak jak czyni to np.
 * funkcja sprintf.
 */

class SString {
public:
	/** Konstruktor bez parametrów. Nic nie inicjuj. */
	SString();

	/** Utwórz obiekt zgodnie z podanym formatem (identycznie jak funkcji printf). */
	SString(const char *fmt, ...);

	/** Skopiuj nowy tekst. */
	SString& operator=(const char *str);

	/** Do³±cz tekst. */
	SString& operator+=(const char *str);

	/** Daj mi ten tekst, który przechowujesz. */
	operator const char*() const;
private:
	/** Decydujemy siê na bufor sta³ej wielko¶ci. */
	static const int BUF_LEN = 512;

	/** Tu przechowujemy ci±g znaków dla obiektów tej klasy. */
	char s[BUF_LEN];
};

#endif
