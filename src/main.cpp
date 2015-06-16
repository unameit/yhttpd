#include <time.h>					// time_t
#include <stdio.h>					// FILE
#include <stdlib.h>					// atoi
#include <signal.h>					// SIGTERM
#include <unistd.h>					// close
#include "String.h"					// documentRoot
#include "Configuration.h"			// conf
#include "Properties.h"				// mimeTypes
#include "AsyncSocket.h"			// serverSocket
#include "ConnectionWithChild.h"	// children
#include "Exception.h"				// catch..
#include "SystemException.h"		// throw
#include "SString.h"				// throw
#include "debug.h"
#include "execParentThread.h"
#include "execChildThread.h"
#include "getChildNo.h"

#ifdef	DEBUG
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#endif


FILE *ferrorlog = stderr;
FILE *fdebuglog = stderr;
FILE *facceslog = NULL;
pid_t *pids = NULL;
int maxChildren = 0;
static int childNo = 0;		// zamist pid'ów


void daemonStart(void);
void daemonStop(void);
void catchSignal(int);
void catchSIGPIPE(int);
int getChildNo() { return childNo; }
static FILE *openFile(const char *filename);


int main(void)
{
	time_t czas;

	try {

	/*** odczytujemy konfiguracjê ***/

	Configuration *conf = new Configuration("yhttpd.conf");
	if (NULL == conf)	throw SystemException("new Configuration(\"yhttpd.conf\") failed!");

	AsyncSocket *serverSocket = new AsyncSocket(AF_INET, SOCK_STREAM);
	if (NULL == serverSocket)	throw SystemException("new AsyncSocket(AF_INET, SOCK_STREAM) failed!");
	serverSocket->bind(conf->getValue("BindAddress"), atoi(conf->getValue("BindPort")));
	serverSocket->listen(atoi(conf->getValue("ListenBacklog")));

	ferrorlog = openFile(conf->getValue("ErrorLog"));
	fdebuglog = openFile(conf->getValue("DebugLog"));
	facceslog = openFile(conf->getValue("AccesLog"));

	czas = time(NULL);
	fprintf(fdebuglog, "%s[PID=%d] yhttpd STARTED\n", ctime(&czas), getpid());

#ifndef	DEBUG
	close(0);	close(1);	close(2);
	daemonStart();
#else
	mkdir("debug", 0700);
#endif

#ifdef  SIGPIPE
    signal(SIGPIPE, catchSIGPIPE);
#endif

	FILE *f = fopen(conf->getValue("PidFile"), "w");
	if (NULL == f)	throw SystemException(SString("Can't open file \"%s\" for writing!", conf->getValue("PidFile")));
	fprintf(f, "%d\n", getpid());
	if (EOF == fclose(f))
		throw SystemException(SString("Can't close file \"%s\"!", conf->getValue("PidFile")));

	Properties mimeTypes(conf->getValue("MimeTypesFile"), true, true);

	maxChildren = atoi(conf->getValue("Children"));
	const int maxChildConnections = atoi(conf->getValue("MaxChildConnections"));

	const String documentRoot(conf->getValue("DocumentRoot")),
		userDir(conf->getValue("UserDir")),
		hostName(conf->getValue("HostName"));

	delete conf;	// zwalniamy nieu¿ywan± ju¿ pamiêæ

	/*** tworzymy procesy potomne ***/

	pids = new pid_t[maxChildren];
	if (NULL == pids)	throw SystemException(SString("new pid_t[%d] failed!", maxChildren));

	ConnectionWithChild **children = new ConnectionWithChild*[maxChildren];
	if (NULL == pids)	throw SystemException(SString("new ConnectionWithChild*[%d] failed!", maxChildren));

	int sockvec[2];

	for(int i = 0; i < maxChildren; ++i) {

		if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, sockvec))
			throw SystemException("socketpair(AF_UNIX, SOCK_STREAM, 0, int[2]) failed!");

		fflush(NULL);

		childNo = i + 1;

		switch(pids[i] = fork()) {
			case -1:
				throw SystemException(SString("[%d] fork() failed!", i));

			case 0:
				fprintf(fdebuglog, "[PID=%d] yhttpd %3d child started\n", getpid(), getChildNo());
				fflush(fdebuglog);

					// usuwamy nieu¿ywane przez proces-dziecko obiekty
				close(sockvec[0]);		// drugi koniec ³±cza komunikacyjnego
				delete serverSocket;	// socket 'dla ¶wiata'
				while(i > 0)
					delete children[--i];
				delete children;		// tablicê z po³±czeniami do procesów-dzieci
				delete pids;			// tablicê PIDów procesów-dzieci
				pids = NULL;
				execChildThread(sockvec[1], hostName, &mimeTypes, documentRoot, userDir);
				break;
			default:
				close(sockvec[1]);
				children[i] = new ConnectionWithChild(sockvec[0], maxChildConnections);
				continue;
		}
	}

	fclose(facceslog);
	childNo = 0;
	execParentThread(children, maxChildren, serverSocket);

	}
	catch(Exception e) {
		czas = time(NULL);
		fprintf(ferrorlog, "%s[%3d] %s\n", ctime(&czas), getChildNo(), (const char *) e);
	}
	catch(const char *e) {
		czas = time(NULL);
		fprintf(ferrorlog, "%s[%3d] %s\n", ctime(&czas), getChildNo(), e);
	}


	// nie delete'ujemy tu ¿adnych obiektów, ani nie zamykamy deskrytporów,
	// gdy¿ i tak w tym miejscu proces ten siê koñczy, wiêc system
	// operacyjny za nas posprz±ta..

	daemonStop();

	return EXIT_SUCCESS;
}


void daemonStart(void)
{
	fflush(NULL);

	switch(fork()) {
		case -1:
			throw SystemException("fork() failed!");
		case 0:
			break;
		default:
			exit(0);
	}

	if (setpgrp() == -1)
		throw SystemException("Can't change process group!\nsetpgrp() failed!");

#ifdef  SIGTTOU
    signal(SIGTTOU, SIG_IGN);
#endif

#ifdef  SIGTTIN
    signal(SIGTTIN, SIG_IGN);
#endif

#ifdef  SIGTSTP
    signal(SIGTSTP, SIG_IGN);
#endif

#ifdef  SIGHUP
    signal(SIGHUP, SIG_IGN);
#endif

#ifdef  SIGTERM
    signal(SIGTERM, catchSignal);
#endif

#ifdef  SIGABRT
    signal(SIGABRT, catchSignal);
#endif

#ifdef  SIGINT
    signal(SIGINT, catchSignal);
#endif

#ifdef  SIGQUIT
    signal(SIGQUIT, catchSignal);
#endif

#ifdef  SIGPWR
    signal(SIGPWR, catchSignal);
#endif

	switch(fork()) {
		case -1:
			throw SystemException("fork() failed!");
		case 0:
			break;
		default:
			exit(0);
	}
}


void catchSIGPIPE(int signal)
{
	if (SIGPIPE != signal)
		return;
	fprintf(fdebuglog, "[%3d] SIGPIPE received\n", getChildNo());
}



/* Funkcja to to 'handler' odbieraj±cy sygna³y wysy³ane nam przez j±dro
 * systemu. Domy¶lnym naszym zachowaniem jest zakoñczenie pracy.
 */

void catchSignal(int sygnal)
{
	char *s;
	fflush(NULL);
	switch(sygnal) {
#ifdef	SIGTERM
		case SIGTERM:
			s = "SIGTERM";
			break;
#endif
#ifdef	SIGABRT
		case SIGABRT:
			s = "SIGABRT";
			break;
#endif
#ifdef	SIGINT
		case SIGINT:
			s = "SIGINT";
			break;
#endif
#ifdef	SIGQUIT
		case SIGQUIT:
			s = "SIGQUIT";
			break;
#endif
#ifdef	SIGPWR
		case SIGPWR:
			s = "SIGPWR";
			break;
#endif
		default:
			s = "UNKNOWN";
			break;
	}

	fprintf(fdebuglog, "[%3d] Zostalem zabity sygnalem nr %d czyli %s", getChildNo(), sygnal, s);

	daemonStop();
}


void daemonStop()
{
	// zabijamy dzieci, gdy nie DEBUGujemy programu
#ifndef	DEBUG
	if (NULL != pids) {
		for(int i = 0; i < maxChildren; ++i)
			kill(pids[i], SIGTERM);
		delete pids;
	}
#endif
	fprintf(fdebuglog, "[%3d] yhttpd daemon TERMINATED\n", getChildNo());
	if (fdebuglog != ferrorlog)
		fprintf(ferrorlog, "[%3d] yhttpd daemon TERMINATED\n", getChildNo());
	exit(0);
}


FILE *openFile(const char *filename)
{
	FILE *f = fopen(filename, "w");
	if (NULL == f)	throw SystemException(SString("Can't open file \"%s\" for writing!", filename));
	f = freopen(filename, "a", f);
	if (NULL == f)	throw SystemException(SString("Can't reopen file \"%s\" for writing!", filename));
#ifdef	DEBUG
	setbuf(f, NULL);
#endif
	return f;
}
