#ifndef	CHILDTHREAD_H
#define	CHILDTHREAD_H

#include "AsyncConnection.h"
#include "ConnectionsManager.h"
#include "AsyncMessageQueue.h"
#include "Vector.h"
#include "SystemException.h"

class Properties;

/**
 * Obiekt tej klasy odpowiedzialny s± za przyjmowanie nowych po³±czeñ od
 * procesu-rodzica, tworzenie z nich obiektów HTTPConnection i obs³ugê
 * tych¿e.  Jego czê¶ci± jest obiekt ConnectonsManager, który "za³atwia ca³±
 * robotê" z po³±czeniami.
 */


class ChildThread : public AsyncConnection {
public:
	/** Standardowy konstruktor. */
	ChildThread(int sockfd, const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/** Standardowy (wirtualny) desktruktor). */
	virtual ~ChildThread();

	/** g³ówna funkcja procesu-dziecka. Tu nale¿y oddaæ sterowanie. */
	void main() { manager.main(); }

private:
	/** Manager po³±czeñ odwalaj±cy de facto ca³± czarn± robotê. */
	ConnectionsManager manager;

	/** nasze ³±cze komunikacyjne z procesem-ojcem. */
	AsyncMessageQueue msgq;

	/** tu zapamiêtujemy nasze aktywne-otwarte po³±czenia z klientam. */
	Vector openedConnections;

	/** tu mamy po³±czenia ju¿ zakoñczone - gotowe go re-inicjalizacji (dziêki nim unikamy kosztowengo 'new' i 'delete'). */
	Vector closedConnections;

	/** warto¶æ int, któr± nale¿y zaraz wys³aæ do procesu-rodzica. */
	int intToSend;

	/** Wska¼niki do obiektów, które musimy przekazywaæ nowo-tworzonym obiektom HTTPconnection. */
	const char *const hostName;
	const Properties *const mimeTypes;
	const char *const documentRoot;
	const char *const userDir;

	/** metoda uaktualnia tablicê aktywnych i nieaktywnych po³±czeñ */
	void updateConnections();

	/** methods inherited from AsyncConnection */

	bool isReadNeeded() const { return true; }
	bool isWriteNeeded() const { return -1 != intToSend; }

	void onReadable();
	void onWriteable();

	operator int() const { return (int) msgq; }

	void finalize() { throw SystemException("[child] AsyncMessageQueue failed!"); }
};

#endif
