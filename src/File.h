#ifndef	FILE_H
#define	FILE_H

#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include "Descriptor.h"

/**
 * Obiekty tej klasy to standardowe pliki systemu UNIX(R). Celem obudowania
 * funkcji do obs³ugi plików w jedn± klasê jest uproszczenie i uczynienie
 * bardziej przejrzystym kodu ¼ród³owego. Klasa dziedziczy publicznie klasê
 * Descriptor. Zawiera dodatkowe metody specyficzne dla plików, takie jak
 * blokowanie pliku, zmiana pozycji z której bêda czytane lub zapisywane
 * dane, pobieranie rozmiaru pliku, czy te¿ daty jego ostatniej modyfikacji. 
 * Wszystkie metody tej klasy, w przypadku niepowodzenia, rzucaj± wyj±tkami
 * z odpowiednim komentarzem.
 */


class File : public Descriptor {
public:
	/** Konstruktor przypisuj±cy istniej±cy ju¿ deskryptor do tego obiektu, lub -1, co oznacza deskryptor ju¿ zamkniêty. */
	File(int d = -1) : Descriptor(d) {}

	/** Otwórz wskazany plik, z odpowiednim trybem */
	File(const char *, int = O_RDONLY);

	/** Jak konstruktor wy¿ej, lecz umo¿liwia re-inicjowane obiektu. */
	File& open(const char *, int = O_RDONLY);

	/** Przesuñ wska¼nik pliku na jego pocz±tek. */
	File& rewind();

	/** Ustaw wska¼nik pliku. */
	File& seek(off_t offset, int whence = SEEK_CUR);

	/** Podobnie jak wy¿ej. */
	File& setpos(off_t offset);

	/** Zablokuj fragment pliku */
	File& lock(off_t len = 0, bool mandatory = false);

	/** Odblokuj. */
	File& unlock(off_t len = 0);

	/** Podaj mi rozmiar tego pliku w bajtach. */
	off_t getSize() const { return size; }

	/** Podaj mi czas ostatniej modyfikacji tego pliku. */
	time_t getModificationTime() const { return mtime; }

	/** Niech ten obiekt zostanie przypisany innemu deskryptorowi */
	File& operator=(int);
private:
	File&	open(int);

	/** Tu zapamiêtujemy rozmiar pliku (w bajtach). */
	off_t	size;

	/** Tu zapamiêtujemy czas ostatniej modyfikacji w pliku. */
	time_t	mtime;
};

#endif
