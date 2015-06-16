#ifndef	SOCKET_H
#define	SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include "Descriptor.h"

#ifndef	__linux__
typedef	int	socklen_t;
#endif

/**
 * Obiekty tej klasy to standardowe gniazda (socket'y) systemu UNIX(R).
 * Klasa rozszerza mo¿liwo¶ci klasy Descriptor o metody specyficzne dla
 * deskryptorów gniazd (takie jak: connect, bind, listen, accept).
 */

class Socket : public Descriptor {
public:
	/** Otwórz obiekt Socket, ale go ie otwieraj */
	Socket() {}

	/** Utwórz obiekt Socket, z deskryptora, który Ci dajê. */
	Socket(int i) : Descriptor(i) {}

	/** Utwórz Socket ze wskazanej rodziny i podanego typu */
	Socket(int domain, int type, int protocol = 0);

	/** Po³±cz to gniazdo z podanym komputerem, na podanym porcie. */
	void connect(const char *hostName, ushort port);

	/** Po³±cz to gniazdo z komputerem, którego adres podany jest w tej strukturze. */
	void connect(const struct sockaddr_in *addr);

	/** Zrób bind tego Socket'a, czyli przypisz go do interfejsu sieciowego wskazywanego podan± nazw± komputera oraz do portu o podanym numerze. */
	void bind(const char *hostName, ushort port);

	/** Przypisz to gniazdo do adresu podanego w tej strukturze danych przekazanej jako argument. */
	void bind(const struct sockaddr_in *addr);

	/** Akceptuj po³±czenie ze ¶wiata. Zwróæ mi deskryptor nowo utworzonego w tym celu gniazda. */
	int accept();

	/** Akceptuj po³±czenie ze ¶wiata. Adres maszyny, sk±d pochodzi po³±czenie zapisz do struktury przekazywanej jako parametr. */
	int accept(struct sockaddr_in *);

	/** Pozwól na backlog oczekuj±cych po³±czeñ w kolejce na zaakceptowanie przez nas. */
	void listen(int backlog);

	/** Podany deskryptor niech bêdzie nowym moim. */
	Socket& operator=(int);
private:
	/** Zapytaj DNSa o IP tego komputera i wpisz mi to do struktury sockaddr_in. */
	void convert(struct sockaddr_in *addr, const char *hostName, ushort port) const;
};

#endif
