#include <netinet/in.h>				// sockaddr_in
#include "debug.h"
#include "ConnectionWithChild.h"
#include "ParentThread.h"


ParentThread::ParentThread(AsyncSocket *socket, ConnectionWithChild **init_children, int init_maxChildren) :
	serverSocket(socket),
	children(init_children),
	maxChildren(init_maxChildren),
	socketfdToSend(-1)
{
	manager.add(this);						// a mo¿e za pêtla?
	for(int i = 0; i < maxChildren; ++i)
		manager.add(children[i]);
}


ConnectionWithChild *ParentThread::getChild() const
{
	static int no = -1;
	if (maxChildren == ++no)
		no = 0;
	return children[no];
}


void ParentThread::onReadable()
{
	/**
	 * Teraz powinni¶my odebraæ po³±czenie (accept()). Mog³o siê jednak
	 * zdarzyæ tak, ¿e poprzednio odebrane po³±czenie nie zosta³o jeszcze
	 * przekazane ¿adnemu z procesów potomnych (gdy¿ np. procesy by³y zbyt
	 * zajête sob±, lub te¿ posiadaj± ju¿ maksymaln± dozwolon± liczbê
	 * otwartych po³±czeñ). Je¶li wiêc mamy takie 'zaleg³e' po³±czenie do
	 * przekazania, to teraz nie przyjmiemy nowego po³±czenia, choæ wiemy,
	 * ¿e n nas czeka (bo przecie¿ onReadable()).  Spróbujemy za to wys³aæ
	 * tamto ponownie.
	 */

	if (-1 == socketfdToSend)
		socketfdToSend = serverSocket->accept(&peer);

	/**
	 * Ponawiaj próby przekazania po³±czenia dopóty nie zostanie uwieñczone
	 * sukcesem, lub liczba prób zrówna siê z ilo¶ci± procesów potomnych. 
	 * Je¶li siê nie uda, od³ó¿ na pó¼niej - widocznie procesy-dzieci s±
	 * zbyt zajête.
	 */

	bool sent = false;
	int count = 0;
	while(!sent && count++ < maxChildren)
		sent = getChild()->sendSocket(socketfdToSend, &peer);

	/**
	 * W tym miejscu nie zamykamy tego socketa. Zrobi to zaraz po wys³aniu
	 * go do dziecka, obiekt zajumuj±cy siê komunikacj± z dzieckiem
	 * (ConnectionWithChild).
	 */

	/** Je¶li uda³o Ci siê przekazaæ po³±czenie, to odnotuj ten fakt. */

	if (sent)
		socketfdToSend = -1;
}
