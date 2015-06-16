#ifndef	CONFIGURATION_H
#define	CONFIGURATION_H

#include "Properties.h"

/**
 * Klasa dziedzicz±ca prywatnie po klasie Properties a stworzona, by
 * wygodniej obs³ugiwaæ plik konfiguracyjny naszego daemona.
 */

class Configuration : private Properties {
public:
	Configuration(const char *filename);

	/** Je¶li poszukiwany warto¶æ odpowiadaj±c podanemu kluczowi nie istnieje, to zamiast zwracanie NULL, rzucany jest wyj±tek. */
	const char *getValue(const char *key) const;

	void showContents() const { Properties::showContents(); }
private:
	;
};

#endif
