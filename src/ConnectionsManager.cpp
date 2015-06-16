#include <sys/time.h>				// struct timeval dla ::select(..)
#include <sys/types.h>				// uint
#include <unistd.h>					// ::select(..)
#include "SystemException.h"		// rzucamy wyj±tkiem, gdy select zwróci -1
#include "AsyncConnection.h"		// wcze¶niej by³a tylko deklaracja zapowiadaj±ca, a my wywo³ujemy metody z tej klasy
#include "ConnectionsManager.h"		// definicja tej klasy


void ConnectionsManager::add(AsyncConnection *c)
{
	connections.add(c);
}


void ConnectionsManager::main()
{
	fd_set rfds, wfds;					// read and write file/socket descriptors
	uint i, rfds_count = 0, wfds_count = 0;
	int fd, max_fd = -1;
	AsyncConnection *c;

	while(connections.getCount() > 0) {
		// nigdy siê nie koñczymy? normalnie nie, przynajmniej na razie.

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		update();

		for(i = 0; i < connections.getCount(); ++i) {		// przeleæ wszystkie..

			c = (AsyncConnection *) connections[i];		// we¼ i-te po³±czenie
			fd = (int) *c;						// we¼ jego deskryptor

			if ( c->isReadNeeded() ) {		// jesli to i-te po³±czenie chce czytaæ, to
				FD_SET(fd, &rfds);			// odnotuj to
				rfds_count++;				// i policz, ile ich jest (chc±cych czytaæ)
				if (fd > max_fd)			// pamiêtaj maksymalny deskryptor - dla ::select(..)
					max_fd = fd;
			}

			if ( c->isWriteNeeded() ) {		// analogicznie, jesli chce zapisywaæ...
				FD_SET(fd, &wfds);
				wfds_count++;
				if (fd > max_fd)
					max_fd = fd;
			}
		}

		// nie sprawdzamy, czy jest sens wchodziæ do select'a (tzn. czy jest
		// chocia¿ jedno po³±czenie, które chce zapisywaæ lub odczytywaæ),
		// gdy¿ wiemy, ¿e tym conajmniej jednym po³±czeniem bêdzie gniazdo w
		// dziedzinie UNIXa - czyli kana³ komunikacjyny z procesem-rodzicem.

		int rc = select(max_fd + 1, (rfds_count > 0? &rfds: NULL), (wfds_count > 0? &wfds: NULL), NULL, NULL);

		if (-1 == rc)
			throw SystemException("select() failed!");

		update();

		if (rfds_count > 0) {								// by³y jakie¶ do zapisu?
			for(i = 0; i < connections.getCount(); ++i) {	// to sprawd¼my je
				c = (AsyncConnection *) connections[i];		// we¼ i-te po³±czenie
				fd = (int) *c;								// we¼ jego deskryptor
				if (FD_ISSET(fd, &rfds))					// mo¿na czytaæ?
					c->onReadable();						// wywo³aj obs³ugê tej sytuacji
			}
		}

		update();

		if (wfds_count > 0) {								// by³y jakies do zapisu?
			for(i = 0; i < connections.getCount(); ++i) {	// to sprawd¼my je
				c = (AsyncConnection *) connections[i];		// we¼ i-te po³±czenie
				fd = (int) *c;								// we¼ jego deskryptor
				if (FD_ISSET(fd, &wfds))					// mo¿na pisaæ?
					c->onWriteable();						// wywo³aj obs³ugê tej sytuacji
			}
		}
	}
}


void ConnectionsManager::update()
{
	AsyncConnection *c;
	for(uint i = 0; i < connections.getCount(); ++i) {		// przeleæ wszystkie..
		c = (AsyncConnection *) connections[i];		// we¼ i-te po³±czenie
		if (-1 == (int) *c) {				// je¶li ju¿ nieaktywne, to
			c->finalize();					// wywo³aj pseudo-desktrukor, czyli niech obiekt wie, ¿e ju¿ zauwa¿ylismy zakoñczenie po³±czenia
			connections.remove(i--);		// usuñ go z tablicy
		}
	}
}
