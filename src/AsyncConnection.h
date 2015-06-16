#ifndef	ASYNCCONNECTION_H
#define	ASYNCCONNECTION_H

/**
 * Klasa ta jest tylko interfejsem który musz± implementowaæ obiekty chc±ce
 * byæ obs³ugiwane przez ConnectionsManager'a.
 *
 * Generalnie opiera siê na mechanizmie asynchronicznych unix'owych
 * socket'ów, ale ich obs³uga odbywa siê w sposób zdarzeniowy, tzn. gdy
 * mo¿liwy jest zapis/odczyt na danym deskryptorze, to generowane jest
 * odpowiednie zdarzenie obs³uguj±ce tê sytuacjê (czyli wywo³ywana
 * odpowiednia metoda dla obiektu klasy imlementuj±cej ten interfejs).
 */

class AsyncConnection {
public:
	 /** chcesz czytaæ? */
	virtual bool isReadNeeded() const { return false; }

	/** chcesz pisaæ? */
	virtual bool isWriteNeeded() const { return false; }

	/** no to czytaj, bo teraz mo¿na */
	virtual void onReadable() {}	 

	/** analogicznie - pisz */
	virtual void onWriteable() {}	 

	/**
	 * daj mi swój UNIXowy deskryptor pliku/gniazda.
	 * -1 oznacza, ¿e po³±czenie jest ju¿ zamkniête.
	 */
	virtual operator int() const { return -1; }

	/**
	 * je¶li -1 == (int) *this, wiêc po³±czenie jest ju¿ zamkniête.
	 * wywo³aj wiêc wtedy ten niby destruktor
	 */
	virtual void finalize() {}

	/**
	 * czy zosta³o ju¿ wywo³ane finalize() ?
	 * musisz zapamiêtaæ ten fakt, ¿eby inne obiekty nie przeoczy³y
	 * re-inicjalizacji po³±czenia
	 * (to zamiast niepotrzebnych new i delete)
	 */
	virtual bool isFinalized() const { return true; }


protected:
	/**
	 * chroniony konstruktor - niech nie istniej± obiekty tej klasy. 
	 * Przeznaczona jest ona tylko do dziedziczenia.
	 */
	AsyncConnection() {}
};

#endif
