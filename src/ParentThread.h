#ifndef	PARENTTHREAD_H
#define	PARENTTHREAD_H

#include <netinet/in.h>			// sockaddr_in
#include "AsyncConnection.h"
#include "ConnectionsManager.h"
#include "AsyncSocket.h"
#include "SystemException.h"

class ConnectionWithChild;

/**
 * Obiekt tej klasy, istniej±cy tylko w procesie-rodzica, odpowiedzialny
 * jest za przyjmowanie nowych po³±czeñ od klientów oraz przekazywanie ich
 * procesom-dzieciom za pomoc± obiektów ConnectionWithChild.  Klasa
 * implementuje interfejs AsyncConnection, do obs³ugi asynchronicznego
 * gniazda po³±czeniowego, na którym nas³uchuje w oczekiwaniu na nowe
 * po³±czenia "ze ¶wiata".
 */

class ParentThread : public AsyncConnection {
public:
	/** Standardowy konstruktor */
	ParentThread(AsyncSocket *socket, ConnectionWithChild **children, int maxChildren);

	/** Standardowy (wirtualny) destruktor. */
	virtual ~ParentThread() {}

	/** tu oddajemy sterowanie w procesie-rodzica */
	void main() { manager.main(); }

private:
	/** manager naszych po³±czeñ */
	ConnectionsManager manager;

	/* obiekt AsyncSocket na którym nas³uchujemy */
	AsyncSocket *serverSocket;

	/** Wska¼nik do tablicy ze wska¼nikami do obiektów ConnectionWithChild */
	ConnectionWithChild **children;

	/** liczba procesów-dzieci jak± posiadamy */
	const int maxChildren;

	/** tu pamiêtamy deskryptor gniazda, które zaraz wy¶lemy procesowi-dziecku */
	int socketfdToSend;

	/** tu pamiêtamy adres klienta, który pode¶lemy te¿ dziecku */
	struct sockaddr_in peer;

	/** metoda zwraca wska¼nik do proponowanego procesu-dziecka, które powinni¶my obarczyæ nastêpnym zadaniem. */
	ConnectionWithChild *getChild() const;

	// metody dla ConnectionsManager'a

	bool isReadNeeded() const { return true; }
	bool isWriteNeeded() const { return false; }

	void onReadable();

	operator int() const { return (int) *serverSocket; }

	void finalize() { throw SystemException("serverSocket has been closed!!"); }
};

#endif
