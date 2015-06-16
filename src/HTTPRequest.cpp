#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include "Exception.h"
#include "Properties.h"
#include "HTTPRequest.h"

#include "debug.h"

#define	SIGNATURE_STRING	"<BR><SMALL><I>Yhttpd/1.0 (Unix)</I></SMALL>\n"


HTTPRequest::HTTPRequest(const char *init_hostName, const Properties *init_mimeTypes,
	const char *init_documentRoot, const char *init_userDir) :
		hostName(init_hostName),
		mimeTypes(init_mimeTypes),
		documentRoot(init_documentRoot),
		userDir(init_userDir),

		hdr200(200, "OK", ""),

		hdr301(301, "Moved Permanently",
			"<HTML><HEAD><TITLE>301 Moved Permanently</TITLE></HEAD>\n"
			"<BODY><H1>301 Moved Permanently</H1>\n"
			"The document has moved <A HREF=\"http://%s%s/\">here</A>.\n"
			SIGNATURE_STRING
			"</BODY></HTML>\n"),

		hdr400(400, "Bad Request",
			"<HTML><HEAD><TITLE>400 Bad Request</TITLE></HEAD>\n"
			"<BODY><H1>Bad Request</H1>\n"
			"Your browser sent a request that\n"
			"this server could not understand.\n"
			SIGNATURE_STRING
			"</BODY></HTML>\n"),

		hdr404(404, "Not Found",
			"<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD>\n"
			"<BODY><H1>Not Found</H1>\n"
			"The requested URL %s was not found on this server.\n"
			SIGNATURE_STRING
			"</BODY></HTML>\n"),

		hdr501(501, "Method Not Implemented",
			"<HTML><HEAD><TITLE>501 Method Not Implemented</TITLE></HEAD>\n"
			"<BODY><H1>Method Not Implemented</H1>\n"
			"%s is not implemented in this release.\n"
			SIGNATURE_STRING
			"</BODY></HTML>\n")
{
	;
}

#undef	SIGNATURE_STRING


bool HTTPRequest::isComplete(const char *s)
{
	return NULL != strstr(s, "\n\n") || NULL != strstr(s, "\n\r\n");
}


uint HTTPRequest::getResponse(char *const buffer, const uint bufferLength)
{
	const char *pathname = NULL;
	method[0] = uri[0] = '\0';

	sendFile = false;
	haveContent = true;

	if (2 != sscanf(buffer, "%9s %499s", method, uri) || uri[0] != '/') {
		generateResponseHeader(buffer, bufferLength, hdr400,
			NULL, hdr400.getHTMLDescriptionLen(), "text/html", 0);
		sprintf(buffer + strlen(buffer), hdr400.getHTMLDescription());
		return strlen(buffer);
	}

	if (strcmp(method, "OPTIONS") == 0) {
		haveContent = false;
		generateResponseHeader(buffer, bufferLength, hdr200, NULL, 0, NULL, 0);
		return strlen(buffer);
	}

	if (strcmp(method, "GET") != 0 && strcmp(method, "HEAD") != 0) {
		generateResponseHeader(buffer, bufferLength, hdr501,
			NULL, hdr501.getHTMLDescriptionLen(), "text/html", 0);
		sprintf(buffer + strlen(buffer), hdr501.getHTMLDescription(), method);
		return strlen(buffer);
	}

	try {

		fileinfo = pathname = resolvePathName(uri + 1);
		if (fileinfo.isDirectory()) {
			haveContent = strcmp(method, "GET") == 0;
			generateResponseHeader(buffer, bufferLength, hdr301,
				uri, hdr301.getHTMLDescriptionLen() + strlen(uri) + strlen(hostName),
				"text/html", 0);
			if (haveContent)
				sprintf(buffer + strlen(buffer), hdr301.getHTMLDescription(), hostName, uri);
			return strlen(buffer);
		}

		if (strcmp(method, "GET") == 0) {
			file.open(pathname);
			sendFile = true;			
		}

	} catch(Exception e) {
		haveContent = strcmp(method, "GET") == 0;
		sendFile = false;
		generateResponseHeader(buffer, bufferLength, hdr404,
			NULL, hdr404.getHTMLDescriptionLen() + strlen(uri), "text/html", 0);
		if (haveContent)
			sprintf(buffer + strlen(buffer), hdr404.getHTMLDescription(), uri);
		return strlen(buffer);
	}

	const char *ext = strrchr(pathname, '.');
	const char *contentType = mimeTypes->getValue(ext == NULL? NULL: ext + 1);
	if (NULL == contentType)
		contentType = "text/plain";

	generateResponseHeader(buffer, bufferLength, hdr200, NULL,
		fileinfo.getSize(), contentType, fileinfo.getMTime());

	return strlen(buffer);
}


const char *HTTPRequest::resolvePathName(char *uri)
{
	static char pathname[1000];

	if ('~' == uri[0]) {
		char *s = strchr(uri, '/');
		if (NULL != s)	*s = '\0';
		struct passwd *user = getpwnam(++uri);
		if (NULL != s)	*s = '/';
		if (NULL == user) {
			return NULL;
		}

		strcpy(pathname, user->pw_dir);
		strcat(pathname, userDir);
		if (s != NULL) {
			strcat(pathname, "/");
			strcat(pathname, ++s);
		}
		// free(pwd); ??????????
	} else {
		strcpy(pathname, documentRoot);
		strcat(pathname, uri);
	}

	uint len = strlen(pathname);
	if ('/' == pathname[len - 1])
		strcat(pathname, "index.html");

	return pathname;
}


#if 0
void HTTPRequest::generateResponseHeader(char *const buffer, const uint bufferLength,
	const int responseCode, const char *const responseString,
	const char *const location,
	const ulong contentLength, const char *const contentType,
	const time_t mtime) const
{
	sprintf(buffer, "HTTP/1.0 %d %s\n", responseCode, responseString);

	time_t czas = time(NULL);
	strftime(buffer + strlen(buffer), bufferLength - strlen(buffer) - 1,
		"Date: %a, %d %b %Y %H:%M:%S %Z\n", gmtime(&czas));

	strcat(buffer, "Server: Yhttpd/1.0 (Unix)\n");

	strcat(buffer, "Allow: GET, HEAD, OPTIONS\n");
	strcat(buffer, "Accept-Ranges: none\n");

	if (location)
		sprintf(buffer + strlen(buffer), "Location: http://%s%s/\n", hostName, location);


	if (0 != mtime)
		strftime(buffer + strlen(buffer), bufferLength - strlen(buffer) - 1,
			"Last-modified: %a, %d %b %Y %H:%M:%S %Z\n", gmtime(&mtime));

	if (haveContent) {
		sprintf(buffer + strlen(buffer), "Content-Type: %s\n", contentType);
		sprintf(buffer + strlen(buffer), "Content-Length: %lu\n", contentLength);
	}

	strcat(buffer, "Connection: close\n");
	strcat(buffer, "X-Pad: avoid browser bug\n\n");
}
#endif

void HTTPRequest::generateResponseHeader(char *const buffer, const uint bufferLength,
	const ResponseHeader &hdr, 
	const char *const location,
	const ulong contentLength, const char *const contentType,
	const time_t mtime) const
{
	sprintf(buffer, "HTTP/1.0 %d %s\n", hdr.getCode(), hdr.getDescription());

	time_t czas = time(NULL);
	strftime(buffer + strlen(buffer), bufferLength - strlen(buffer) - 1,
		"Date: %a, %d %b %Y %H:%M:%S %Z\n", gmtime(&czas));

	strcat(buffer, "Server: Yhttpd/1.0 (Unix)\n");

	strcat(buffer, "Allow: GET, HEAD, OPTIONS\n");
	strcat(buffer, "Accept-Ranges: none\n");

	if (location)
		sprintf(buffer + strlen(buffer), "Location: http://%s%s/\n", hostName, location);

	if (0 != mtime)
		strftime(buffer + strlen(buffer), bufferLength - strlen(buffer) - 1,
			"Last-modified: %a, %d %b %Y %H:%M:%S %Z\n", gmtime(&mtime));

	if (haveContent) {
		sprintf(buffer + strlen(buffer), "Content-Type: %s\n", contentType);
		sprintf(buffer + strlen(buffer), "Content-Length: %lu\n", contentLength);
	}

	strcat(buffer, "Connection: close\n");
	strcat(buffer, "X-Pad: avoid browser bug\n\n");
}
