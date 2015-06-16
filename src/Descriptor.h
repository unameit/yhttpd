#ifndef	DESCRIPTOR_H
#define	DESCRIPTOR_H

#include <stddef.h>
#include <sys/uio.h>
#include <sys/types.h>

/**
 * Klasa ta obudowuje klasyczne UNIXowe funkcje operuj±ce na deskryptorach w
 * klasê jêzyka C++, by upro¶ciæ korzystanie z nich, a jednocze¶nie zapewniæ
 * lepsz± obs³ugê sytuacji awaryjnych (niepowodzeñ wywo³añ tych¿e funkcji
 * systemowych) poprzez rzucanie wyj±tków jêzyka C++, a przez to u³atwienie
 * sterowania wykonywaniem programu. Metody tej klasy s± wspólne dla plików
 * jak i gniazd.
 */


class Descriptor {
protected:
	/** Chroniony konstruktor  - niech nie istnieja obiekty tej klasy tworzone przez inaczej, ni¿ w klasach pochdnych. */
	Descriptor(int newfd = -1) { fd = newfd; }

	/** Zrób z tego deskryptora, deskryptor asynchroniczny. */
	void makeAsync();

public:
	/** Przypisz temu obiektowi ca³kiem nowy deskryptor. */
	virtual Descriptor& operator=(int);

	/** Standardowy destruktor. */
	virtual ~Descriptor() { close(); }

	/** Zamknij deskryptor. Zwykle oznacza to tak¿e zapisanie danych przetrzymywanych nadal w buforach systemowych. */
	void close();

	/** Odczytaj jeden znak. */
	int read();

	/** Odczytaj nbytes znaków do bufora wskazywanego przez buffer. Je¶li ostatni argument bêdzie true, to rzuæ wyj±tkiem EOFException, je¶li nie uda³o Ci siê wczytaæ ca³ej ¿±danej porcji danych. */
	size_t read(void *buffer, size_t nbytes, bool full = true);

	/** Podobnie jak wy¿ej, lecz dla tablicy buforów iovec[] */
	size_t readv(const struct iovec *iov, size_t count, bool full = true);

	/** Zapisz jeden znak. */
	void write(u_char);

	/** Zapisz bufor. Parametry analogicznie, jak przy read. */
	size_t write(const void *buffer, size_t, bool full = true);

	/** Podobnie jak wy¿ej. */
	size_t writev(const struct iovec *iov, size_t count, bool full = true);

	/** Zwróæ mi swój UNIXowy deskryptor. */
	inline operator int() const { return fd; }

protected:
	/** I tu jest ten nasz deskryptor. */
	int fd;
};

#endif
