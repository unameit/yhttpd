#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "debug.h"
#include "Exception.h"
#include "File.h"
#include "getChildNo.h"
#include "HTTPConnection.h"



HTTPConnection::HTTPConnection(const char *hostName, const Properties *mimeTypes,
	const char *documentRoot, const char *userDir) :
		request(hostName, mimeTypes, documentRoot, userDir),
		finalized(true)
{
	;
}


HTTPConnection& HTTPConnection::operator=(int sockfd)
{
	sock.close();
	sock = sockfd;				// we¼ nowy socket
	nread = 0;					// nic jeszcze nie przeczytali¶my
	requestReceived = false;	// request jeszcze nie odebrany
	finalized = false;
	return *this;
}


void HTTPConnection::onReadable()
{
	try {

	nread += sock.read(buffer + nread, bufferSize - nread  - 1);	// doczytaj
	buffer[nread] = '\0';							// zaterminuj string
	requestReceived = request.isComplete(buffer);	// dobierz siê do bufora
	if (!requestReceived) {							// je¶li jeszcze nie kompletny, to
		if (bufferSize == nread || 0 == nread)		// i je¶li bufor ju¿ pe³ny, lub nic nie przeczyta³
			sock.close();							// to zerwij to po³±czenie
		return;										// wróæ 'wy¿ej'
	}

	// je¶li tu jeste¶my, tzn., ¿e request ju¿ jest odebrany i sparsowany
	// przygotuj nam wiêc odpowied¼ na niego

	responseLength = request.getResponse(buffer, bufferSize);
	nwritten = 0;
	file = request.getFile();

	char txt[100];
	time_t czas = time(NULL);
	strftime(txt, sizeof txt - 1, "%Y.%m.%d %H:%M:%S", localtime(&czas));
	fprintf(facceslog, "%s %s %s %s\n", txt, inet_ntoa(peer.sin_addr), request.getMethod(), request.getURI());

	} catch(Exception e) {
		time_t czas = time(NULL);
		fprintf(ferrorlog, "%s[%3d] %s\n", ctime(&czas), getChildNo(), (const char *) e);
		sock.close();
	}
}


void HTTPConnection::onWriteable()
{
	try {

	if (nwritten < responseLength) {
		nwritten += sock.write(buffer + nwritten, responseLength - nwritten);
		return;
	}

	if (NULL == file) {		// nic wiêcej nie wysy³aæ?
		sock.close();	// zamkniêcie socketu gwarantuje, ¿e -1 == (int) sock, a wiêc wszystko OK
		return;
	}

	responseLength = file->read(buffer, bufferSize, false);
	nwritten = 0;

	if (0 == responseLength) {
		sock.close();
		file->close();
	}

	} catch(Exception e) {
		time_t czas = time(NULL);
		fprintf(ferrorlog, "%s[%3d] %s\n", ctime(&czas), getChildNo(), (const char *) e);
		sock.close();
		if (NULL != file)	file->close();
	}
}


void HTTPConnection::finalize()
{
	try {
		sock.close();
	} catch(...) {}		// nie obchodz± nas ¿adne b³êdy zamykania socketu
	finalized = true;
}
