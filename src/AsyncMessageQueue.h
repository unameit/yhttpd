#include <netinet/in.h>		// sockaddr_in
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "AsyncSocket.h"

/**
 * Klasa ta (dziedzicz±ca prywatnie klasê AsyncSocket) s³u¿y przede
 * wszystkim do komunikacji miêdzyprocesowej. Oprócz zwyk³ych danych pozwala
 * tak¿e przekazywaæ otwarte deskryptory plików i gniazd (socketów) innym
 * procesom. Ze wzglêdu na potrzeby tego projektu oraz wymagane jej cechy
 * zosta³a ona zrealizowana na ³±czu komunikacyjnym nieblokuj±cym
 * (asynchronicznym socket'cie w dziedzinie UNIXa).
 */

class AsyncMessageQueue : private AsyncSocket {
public:
	/**
     * Jako argument MUSI byæ podany deskryptor gniazda z dziedziny UNIXa -
	 * inne nie potrafi± przekazywaæ ¿ywych deskryptorów.
     */

	AsyncMessageQueue(int sockfd) : AsyncSocket(sockfd) { init(); }
	~AsyncMessageQueue() {}

	/** Wy¶lij deskryptor podany jako argument. */
	void sendDescriptor(int, const struct sockaddr_in *peer);

	/* Odbierz deskryptor. */
	int recvDescriptor(struct sockaddr_in *peer);

	/** Wy¶lij liczbê int. */
	void sendInt(int i) { AsyncSocket::Socket::write(&i, sizeof i, true); }

	/** Odbierz liczbê int. */
	int recvInt() { int i; AsyncSocket::Socket::read(&i, sizeof i, true); return i; }

	/** Przypisz temu obiektowi inny, wskazywany podanym deskryptorem. */
	virtual AsyncMessageQueue& operator=(int);

	/** Podaj mi swój deskryptor gniazda. */
	operator int() const { return fd; }

	/** Upubliczniamy metodê close(), gdy¿ zosta³a ona wcze¶niej schowana z powody prywatnego dziedziczenia. */
	AsyncSocket::close;

protected:
	/** udostêpniamy fd innym wy¿szym klasom (bo go schowali¶my przez ": private AsyncSocket") */
	AsyncSocket::fd;	

private:

	/** Struktury i zmienne potrzebne dla wywo³añ funkcji systemowych. */
#ifndef	OLD_MSGHDR
	struct {
		struct cmsghdr cmsg;
		int fd;
	} mymsg;
#endif
	struct iovec iov[1];
	struct msghdr msg;

	void init();
};
