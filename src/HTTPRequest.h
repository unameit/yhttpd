#ifndef	HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <sys/types.h>	// uint
#include <string.h>		// strlen
#include "File.h"		// private member
#include "FileInfo.h"	// private member

class Properties;

/**
 * Obiekt tej klasy zajmuje siê parsowaniem zapytania klienta, zrozumieniem
 * go oraz wygenerowaniem w³a¶ciwej odpowiedzi (zgodnie z protoko³em HTTP). 
 * Wskazuje te¿ oraz otwiera plik, którego wys³aniem zaj±æ siê ma obiekt
 * HTTPConnection.
 */

class HTTPRequest {
public:

	/**
	 * Standardowy konstruktor, w którym podajemy wska¼niki do pó¼niej
	 * wykorzystywanych obiektów.
	 */

	HTTPRequest(const char *hostName, const Properties *mimeTypes,
		const char *documentRoot, const char *userDir);

	/**
	 * Metoda prawdza to co nam przys³a³a przegl±darka (a ja to podajê w
	 * buforze tekstowym jako string - czyli zakoñczony znakiem '\0'). 
	 * Zwraca true, je¶li request jest ju¿ odebrany w ca³o¶ci (zwykle
	 * oznacza to, ¿e koñczy siê dwoma znakami koñca linii '\n' - ale nie
	 * jest to regu³±, choæ tak powinno byæ). Lub zwraca false, je¶li
	 * request nie jest jeszcze kompletny i nale¿y spodziewaæ siê, ¿e co¶ z
	 * sieci zaraz dojdzie. Wtedy metoda ta zostanie wywo³ana ponownie z
	 * uzupe³nionym buforem.
	 */

	bool isComplete(const char *request);

	/**
	 * Metoda ta na podstawie zapytania przegl±darki (podawanego w metodzie
	 * wy¿ej, ale podana tak¿e tutaj jeszcze raz, tylko tym razem mo¿n± j±
	 * zniszczyæ - a nawet trzeba. Bufor przenaczony jest przecie¿ teraz na
	 * odpowied¼) formu³uje nag³ówek odpowiedzi. Mog± to ju¿ byæ np. jakie¶
	 * informacje o pliku, który za¿yczy³a sobie przegl±darka w zapytaniu. 
	 * Odpowied¼ tê umieszcza we wskazanym buforze tekstowym, którego
	 * wielko¶æ podana jest jako drugi argument.  Naturalnie odpowied¼ nie
	 * mo¿e przekroczyæ tego rozmiaru, gdy¿ spowoduje to "SEGMENTATION
	 * FAULT" i zakoñczenie bie¿±cego procesu. Nale¿y siê jednak spodziewaæ,
	 * ¿e dany bufor bêdzie mia³ conajmniej 32 kilobajty, a wiêc
	 * wystarczaj±co du¿o.  Metoda ta zwraca d³ugo¶æ w bajtach nag³ówka
	 * odpowiedzi, który nale¿y wys³aæ.
	 */

	uint getResponse(char *buffer, uint bufferLength);

	/**
	 * Metoda ta zwraca wska¼nik do obiektu File, którego zawarto¶æ nale¿y
	 * wys³aæ, zaraz za nag³ówkiem.  Zwraca NULL je¶li nie nale¿y nic wiêcej
	 * (poza nag³ówkiem) wysy³aæ.  Je¶li skoñczê z plikiem, to sam go zamknê
	 * - Ty wiêc o jego zamykanie nie musisz martwiæ.
	 */

	File *getFile() { return sendFile ? &file: NULL; }

	/** Zwraca metodê bie¿±cego zapytania. Metoda na potrzeby wpisu w pliku acces.log */
	const char *getMethod() const { return method; }

	/** Zwraca URI bie¿±cego zapytania. Metoda na potrzeby wpisu w pliku acces.log */
	const char *getURI() const { return uri; }

private:
	/** Nazwa komputera na którym jeste¶my uruchomieni - potrzebna w polu Location odpowiedzi HTTP. */
	const char *const hostName;

	/** Obiekt 'rozpoznaj±cy' typy plików po rozszerzeniach ich nazwy. */
	const Properties *const mimeTypes;

	/** Gdzie sk³aowane s± dokumenty z g³ównego drzewa. */
	const char *const documentRoot;

	/** W którym katalogu w katalgoach u¿ytkowników szukaæ index.html i innych dokumentów. */
	const char *const userDir;

	/** wysy³amy co¶ oprócz nag³ówka? / wysy³amy plik? */
	bool haveContent, sendFile;

	/** tu pamiêtamy dane o pliku, którym interesuje siê klient */
	FileInfo fileinfo;

	/** a tu jest ewentualnie ten plik. */
	File file;

	/** metoda przerabia URI na pe³n± scie¿kê dostêpu do pliku */
	const char *resolvePathName(char *);

	/**
	 * Prywatna klasa klasy HTTPRequest. Jej obiekty s± nag³ówkami
	 * odpowiedzi wysy³anymi do klientów. Atrybutami tych oiektów s±: kod
	 * odpowiedzi, jej krótki opis, opis w HTMLu
	 */

	class ResponseHeader {
	public:
		ResponseHeader(int c, const char *d, const char *hd) :
			code(c), description(d), htmlDescription(hd), htmlDescriptionLen(strlen(htmlDescription)) {}
		int getCode() const { return code; }
		const char *getDescription() const { return description; }
		const char *getHTMLDescription() const { return htmlDescription; }
		uint getHTMLDescriptionLen() const { return htmlDescriptionLen; }
	private:
		const int code;
		const char *const description;
		const char *const htmlDescription;
		const uint htmlDescriptionLen;
	};

	const ResponseHeader hdr200, hdr301, hdr400, hdr404, hdr501;

	/** Metoda generuje nag³ówke odpowiedzi korzystaj±c z podanych obiektów. */

	void generateResponseHeader(char *buffer, uint bufferLength,
		const ResponseHeader &hdr,
		const char *location,
		ulong contentLength, const char *contentType,
		time_t mtime) const;

	char method[10], uri[500];
};

#endif
