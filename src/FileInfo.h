#ifndef	FILEINFO_H
#define	FILEINFO_H

#include <time.h>		// time_t
#include <sys/types.h>	// off_t

// typedef	unsigned long	off_t;

/**
 * Obiekty tej klasy zawieraj± informacje na temat istniej±cych plików takie
 * jak: data ostatniej modyfikacji i rozmiar. Klasa pozwala na uzyskiwanie
 * tych informacji bez otwierania wskazanego pliku.
 */

class FileInfo {
public:
	/** Pobierz informacje na temat pliku, którego nazwê Ci przekazujê jako argument */
	FileInfo(const char *filename = NULL);

	/** Pobierz informacje na temat pliku, którego deskryptor Ci przekazujê jako parametr. */
	FileInfo(int fd);

	/** Pobierz informacje na temat pliku, którego nazwê Ci przypisujê */
	FileInfo& operator=(const char *filename);

	/** Pobierz informacje na temat pliku, którego deskryptor Ci przypisujê. */
	FileInfo& operator=(int fd);

	/** Podaj mi rozmiar tego pliku. */
	off_t getSize() const { return size; }

	/** Podaj mi czas ostatniej modyfikacji obiektu, o którym przechowujesz informacjê. */
	time_t getMTime() const { return mtime; }

	/** Czy obiekt o którym informacjê przechowujesz, jest katalogiem? */
	bool isDirectory() const { return dir; }
private:
	/** Tu zapamiêtujemy rozmiar pliku w bajtach. */
	off_t	size;

	/** Tu zapamiêtujemu czas ostatniej modyifikacji. */
	time_t	mtime;

	/** Tu, czy jest to katalog, czy plik. */
	bool	dir;

	/** Pobierz informacje o pliku wskazywanym przez nazwê podan± jako paramter. */
	void stat(const char *filename);

	/** Pobierz informacje o pliku wskazywanym przez ten deskryptor, który Ci przekazujê, jako argument. */
	void stat(int fd);
};

#endif
