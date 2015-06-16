#ifndef	SYSTEMEXCEPTION_H
#define	SYSTEMEXCEPTION_H

#include "Exception.h"

/**
 * Klasa pochodna klasy Exception zapewniaj±ca dodatkowo komunikaty o
 * b³êdach pochodz±ce bezpo¶rednio od systemu operacyjnego (na podstawie
 * zmiennej errno i funkcji strerror).
 */

class SystemException : public Exception {
public:
	/** Zwyk³y konstruktor. */
	SystemException(const SString &details) : Exception() { init(details); addErrnoMsg(); }
protected:
	/** Konstruktor bez-parametrów chroniony: tylko dla klas pochodnych */
	SystemException() {}

	/** Metoda dodaj±ca systemowy opis b³êdu. */
	void addErrnoMsg();

	/** Podaj mi nazwê klasy obiektów, do której nale¿ysz */
	virtual const char *getName() const { return "SystemError"; }
};

#endif
