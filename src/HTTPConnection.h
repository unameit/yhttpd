#ifndef	HTTPCONNECTION_H
#define	HTTPCONNECTION_H

#include <netinet/in.h>			// private: struct sockaddr_in peer;
#include "AsyncConnection.h"
#include "AsyncSocket.h"
#include "HTTPRequest.h"

class Properties;
class File;

/**
 * Klasa implementuj±ca interfejs AsyncConnection i realizuj±ca po³±czenie z
 * klientem (cze¶ci± obiektu HTTPConnection jest obiekt AsyncSocket).
 * Zajmuje siê tylko odbieraniem (i zapamiêtywaniem) danych przes³anych od
 * klienta, oraz wysy³aniem odpowiedzi wygenerowanej przez obiekt
 * HTTPRequest. Obiekt tej klasy potrafi te¿ wys³aæ plik wskazany przez
 * HTTPRequest'a.
 */

class HTTPConnection : public AsyncConnection {
public:
	/** Utwórz obiekt. Zapamiêtaj tych parê rzeczy, które Ci siê nied³ugo przydadz±. */
	HTTPConnection(const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/** desktruktor */
	virtual ~HTTPConnection() {}

	/** inicjuj nowe po³±czenie (na podanym deskryptorze) */
	HTTPConnection& operator=(int sockfd);

	/** Ustaw sobie adres IP 'drugiej strony' celem zapisania tej informacji w logu. */
	void setPeer(struct sockaddr_in *p) { peer = *p; }

private:
	/** socket - po³±czenie ze ¶wiatem */
	AsyncSocket sock;

	/** tu zapamiêtujemy adres klienta - w celu zapisu w logu */
	struct sockaddr_in peer;

	/** ten obiekt parsuje zapytanie przegl±darki i czyni odpowied¼ */
	HTTPRequest request;

	/** 32k - powinno wystarczyæ */
	static const size_t bufferSize = 32 * 1024;

	/** bufor do odbierania i wysy³ania danych */
	char buffer[bufferSize];

	/** ile ju¿ odebrali¶my/wys³ali¶my */
	size_t nread, nwritten;

	/** ile trzeba bêdzie wys³aæ bajtów nag³ówka */
	size_t responseLength;

	/** czy odebrali¶my ju¿ kompletny request? */
	bool requestReceived;

	/** czy manager wie, ¿e po³±czenie zakoñczone? */
	bool finalized;

	/** Wska¼nik do pliku, który od czasu do czasu zdarzy siê nam wys³aæ. */
	File *file;

	/** metody, które musz± byæ 'przykryte' dla ConnectionsManager'a: */

	bool isReadNeeded() const { return !requestReceived && !finalized; }

	/** Odpowiedz na pytanie, czy ten obiekt jest w tym momencie zaineresowany zapisem. */
	bool isWriteNeeded() const { return requestReceived && !finalized; }

	/** Metoda wywo³ywana, gdy odczyt jest mo¿liwy */
	void onReadable();

	/** Metoda wywo³ywana, gdy zapis jest mo¿liwy */
	void onWriteable();

	/** Zwróæ deskryptor naszego po³±czenia - czyli socket'u. */
	operator int() const { return (int) sock; }

	/** 'Zfinalizuj siê', tzn. odnotuj fakt, ¿e ConnectionsManager zauwa¿y³, i¿ po³±czenie zosta³o zakoñczone i usun±³ ten obiekt ze swojej tablicy. Bêdzie wiêc mo¿na po re-inicjalizacji podaæ mu go znowu. */
	void finalize();

	/** Odpowiada na pytanie, czy obiekt jest ju¿ 'zfianlizowany'. */
	bool isFinalized() const { return finalized; }
};

#endif
