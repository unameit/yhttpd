#ifndef	CONNECTIONTOCHILD_H
#define	CONNECTIONTOCHILD_H

#include <netinet/in.h>				// sockaddr_in
#include "AsyncConnection.h"		// inherited
#include "AsyncMessageQueue.h"		// private member
#include "SystemException.h"		// thrown in finalize()
#include <sys/types.h>				// uint

/**
 * Ka¿dy obiekt tej klasy zajmuje siê komunikacj± miêdzy procesem-rodzicem a
 * jednym procesem-dzieckiem, po stronie procesu-ojca. Potrafi przekazaæ
 * dziecku nowe po³±czenie (deskryptor gniazda przekazany za pomoc± obiektu
 * AsyncMessageQueue, który jest jego czê¶ci±) oraz odebraæ informacjê o
 * aktualnej ilo¶ci otwartych po³±czeñ procesu-dziecka. To on nie pozwala,
 * by proces-dziecko mia³o wiêksz± ni¿ dozwolona w pliku konfiguracyjnym
 * ilo¶æ jednocze¶nie otwartych po³±czeñ.
 */

class ConnectionWithChild : public AsyncConnection {
public:
	/** Standardowy konstruktor, któremu podajemy przede wszystkim deskryptor ³±cza komunikacyjnego do procesu-dziecka. */
	ConnectionWithChild(int sockfd, int maxChildConnections);
	virtual ~ConnectionWithChild() {}

	/** metoda podaje aktualn± liczbê aktywnych po³±czeñ dziecka, z którym jest po³±czony */
	uint getConnectionsCount() const { return openConnections; }

	/** wy¶lij deskryptor do dziecka, albo nie, je¶li poprzedni jeszcze nie wys³any */
	bool sendSocket(int sockfd, const struct sockaddr_in *peer);

private:
	/** nasze ³±cze komunikacyjne do procesu-dziecka */
	AsyncMessageQueue msgq;

	/** liczba otwartych po³±czeñ procesu-dziecka */
	uint openConnections;

	/** maksymalan dozwolona liczba po³±czeñ procesu-dziecka */
	const uint maxConnections;

	/** obiekt Socket oczekuj±cy a wys³anie. */
	Socket socketToSend;

	/** adres klienta (po drugiej stronie socket'u) - wy¶lemy go dziecku */
	struct sockaddr_in peer;

	/** metody przys³aniane z AsyncConnection dla ConnectionsManager'a */

	/** zawsze chcemy odbieraæ to, co dziecko nam wysy³a */
	bool isReadNeeded() const { return true; }

	/** ale sami wysy³amy tylko wtedy, gdy rzeczywi¶cie mamy co¶ do wys³ania */
	bool isWriteNeeded() const { return -1 != socketToSend; }

	void onReadable();
	void onWriteable();

	operator int() const { return (int) msgq; }

	void finalize() { throw SystemException("Connection with child has been closed!!"); }
};

#endif
