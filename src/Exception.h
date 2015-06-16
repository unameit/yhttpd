#ifndef	EXCEPTION_H
#define	EXCEPTION_H

#include "SString.h"

/**
 * obiekty tej klasy s± wyj±tkami jêzyka C++ rzucanymi (jak nazwa wskazuje)
 * w sytacjach wyj±tkowych (np. niepowodzenia wywo³ania funkcji systemowej).
 * U³atwa to kontrolê sterowania programem oraz generowania tre¶ciwych
 * komunikatów o b³êdach i miejscach ich wyst±pienia.
 */

class Exception {
public:
	/** Konstruktor zwyk³ego wyj±tku z krótkim opisem. */
	Exception(const char *details) { init(details); }

	/** Konstruktor wyj±tku z opcjonalnym rozbudowanym opisem - ³±cznie z dodatkowymi argumentami. */
	Exception(const SString &details) { init(details); }

	/** daj mi tekst Ciê opisuj±cy.. */
	operator const char*() const { return string; }
protected:
	/** Chroniony konstruktor bez-parametrowy dla klas pochodnych. Innymi s³owy - nie pozwól na istnienie obiektów tej klasy bez tre¶ci. */
	Exception() {}

	/** Inicjuj ten obiekt */
	void init(const SString&);

	/** Podaj mi nazwê okre¶laj±c± nazwê Twojej klasy */
	virtual const char *getName() const { return "Exception"; }

	/** Totaj trzymamy ci±g znaków opisuj±cy ka¿dy obiekt tej klasy. */
	SString	string;
private:
	;
};

#endif
