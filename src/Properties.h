#ifndef	PROPERTIES_H
#define	PROPERTIES_H

#include "Vector.h"
#include "String.h"
#include <stdio.h>

/**
 * Prywatna klasa klasy Properties zapamiêtuj±ca (jako obiekty klasy String)
 * klucz i odpowiadaj±c± mu warto¶æ.
 */

class PropertiesNode {
public:
	PropertiesNode(const char *k, const char *v) : key(k), value(v) {}
	const char *getKey() const { return key; }
	const char *getValue() const { return value; }
private:
	String key, value;
};

/**
 * Obiekty tej klasy potrafi± wczytaæ plik tekstowy zawieraj±cy pary
 * klucz-warto¶æ oraz zapamiêtaæ go tak (dziêki obiektowi klasy Vector), by
 * odszukiwanie danej warto¶ci na podstawie podanego klucza odbywa³o siê
 * szybko. Klasa ta rozpoznaje linie komentarza rozpoczynaj±ce siê od znaku
 * "#". Akceptuje odwrotn± kolejno¶æ w pliku tekstowym, a tak¿e wiele kluczy
 * dla jednej warto¶ci.
 */


class Properties {
public:
	Properties() {}
	Properties(const char *, bool = false, bool = false);
	~Properties() { clear(); }

	/** Dodaj parê klucz-warto¶æ. */
	void add(const char *, const char *);

	/** Podaj mi warto¶æ odpowiadaj±c± kluczowi podanemu jako argument. */
	const char *getValue(const char *) const;

	/** poka¿ ca³± swoj± zawarto¶æ */
	void showContents() const;

	/** usuñ ca³± swoj± zawarto¶æ */
	void clear();
private:
	void load(FILE *, bool, bool);
	int	go2nextLine(FILE *) const;
	int go2nextWord(FILE *) const;

	Vector nodes;
};

#endif
