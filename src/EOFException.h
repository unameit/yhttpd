#ifndef	EOFEXCEPTION_H
#define	EOFEXCEPTION_H

#include "SystemException.h"

/**
 * Obiekt tej klasy jest wyj±tkiem w sensie jêzyka C++ rzucanym w sytuacji,
 * gdy za¿±dano odczytu z deskryptora dok³adnej porcji danych (tzn. nie
 * akceptowana jest porcja mniejsza), za¶ system operacyjny informuje, i¿ do
 * czytania ju¿ nic wiêcej nie ma. Klasa dziedziczy publicznie po klasie
 * SystemException.
 */

class EOFException : public SystemException {
public:
	/** Standardowy konstruktor. */
	EOFException(const SString &details) : SystemException() { init(details); addErrnoMsg(); }
protected:
	/** Podaj mi nazwê klasy obiektów, do której nale¿ysz. */
	virtual const char *getName() const { return "EOFException"; }
};

#endif
