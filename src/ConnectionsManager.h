#ifndef	CONNECTIONSMANAGER_H
#define	CONNECTIONSMANAGER_H

#include "Vector.h"

// deklaracja zapowiadaj±ca - bo argument metody add
class AsyncConnection;			

/**
 * Obiekt tej klasy potrafi zarz±dzaæ grup± obiektów, które implementuj±
 * interfejs AsyncConnection i realizuj± nieblokuj±ce po³±czenie z u¿yciem
 * UNIXowych deskryptorów.  Obiektowi tej klasy oddaje siê sterowanie
 * zarówno w procesie-rodzicu, jak i w procesach potomnych.  Dziêki klasie
 * AsyncConnection metody z innych obiektów mog± byæ wywo³ywane w
 * odpowiednich dla nich momentach, tj. gdy zajd± zdarzenia "mo¿liwo¶æ
 * pisania na deskryptorze" lub/i "mo¿liwo¶æ czytania z deskryptora". Obiekt
 * tej klasy usuwa zamkniête po³±czenia w sposób umo¿liwiaj±cy ich dalsze
 * wykorzystanie po re-inicjalizacji.
 */

class ConnectionsManager {
public:
		/**
		 * Tu siê oddaje sterowanie. Potem tyko wyskakuje do metod obiektów
		 * AsyncConnection::onReadable() i onWriteable().  dlatego trzeba
		 * przed oddaniem sterowania dodaæ conajmniej jeden ¿ywy obiekt
		 * AsyncConnection.
		 */
	void main();

		/**
		 * Dodaj nowe po³±czenie. Je¶li zostanie ono zamkniête (zwróci -1
		 * jako swój deskryptor) zostanie 'automatycznie' usuniête z naszej
		 * tablicy obs³ugiwanych po³±czeñ. Musimy powiadomiæ inne obiekty o
		 * tym, ¿e 'zauwa¿yli¶my' zamkniêcie po³±czenia. Zrobimy to poprzez
		 * wywo³anie metody AsyncConnection::finalize(). Wtedy obiekt ten
		 * (jak i inne) bêd± wiedzia³y, ¿e nie znajduje siê on ju¿ w naszej
		 * tablicy, a wiêc mo¿na na tym obiekcie utoworzyæ nowe po³±czenie i
		 * na nowo dodaæ do tego manager'a.
		 */
	void add(AsyncConnection *);

private:
	/** tu pamiêtamy wszystkie obs³ugiwane po³±czenia */
	Vector connections;		

	/** Metoda sprawdza, czy po³±czenia s± aktywne, i ewentualnie je usuwa. */
	void update();
};

#endif
