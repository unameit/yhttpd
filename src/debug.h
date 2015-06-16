#ifndef	DEBUG_H
#define	DEBUG_H

#include <stdio.h>


#ifdef	DEBUG
extern FILE *fdebuglog;
#endif

extern FILE *ferrorlog;
extern FILE *facceslog;


#endif
