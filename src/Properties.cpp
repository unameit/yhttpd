#include <stdio.h>
#include <string.h>
#include "SString.h"
#include "SystemException.h"
#include "Properties.h"


static int cmp(const void *p1, const void *p2)
{
	const PropertiesNode *n1 = *((const PropertiesNode **) p1), *n2 = *((const PropertiesNode **) p2);
	return strcmp(n1->getKey(), n2->getKey());
}


Properties::Properties(const char *filename, bool reverse, bool multi)
{
	try {

	FILE *f = fopen(filename, "r");

	if (NULL == f)
		throw SystemException(SString("Can't open file \"%s\"!", filename));

	load(f, reverse, multi);
	nodes.sort(cmp);

	if (EOF == fclose(f))
		throw SystemException(SString("Can't close file \"%s\"!", filename));

	} catch(...) { throw; }
}


void Properties::clear()
{
	const size_t N = nodes.getCount();
	PropertiesNode *n;

	for(size_t i = 0; i < N; ++i) {
		n = (PropertiesNode *) nodes[i];
		delete n;
	}

	nodes.clear();
}


void Properties::load(FILE *f, bool reverse, bool multi)
{
	try {

	char type[128], ext[128];
	bool extMode;

	while(true) {

		if (1 != fscanf(f, "%127s", type))	return;

		if ('#' == type[0]) {
			if (EOF == go2nextLine(f))	return;
			continue;
		}

		do {

			switch(go2nextWord(f)) {

				case EOF:
					return;

				case '\n':
					extMode = false;
					break;

				default:
					extMode = true;
					break;
			}

			if (extMode) {
				fscanf(f, "%127s", ext);

				PropertiesNode  *n;
				if (reverse)
					n = new PropertiesNode(ext, type);
				else
					n = new PropertiesNode(type, ext);
				if (NULL == n)
					throw SystemException(SString("'new PropertiesNode(\"%s\", \"%s\")' failed!", type, ext));

				nodes.add(n);
			}

		} while(extMode && multi);

	}

	} catch(...) { throw; }
}


int Properties::go2nextWord(FILE *f) const
{
	int	c;

	while(true) {

		switch(c = fgetc(f)) {

			case EOF:
				return EOF;

			case ' ':
			case '\t':
				continue;

			case '\n':
			case '\r':
				return '\n';

			default:
				if (EOF == ungetc(c, f))
					return EOF;
				return c;
		}
	}
}


int Properties::go2nextLine(FILE *f) const
{
	int	c;

	while(true) {

		switch(c = fgetc(f)) {

			case EOF:
				return EOF;

			case '\n':
				return '\n';

			case '\r':
				return '\r';

			default:
				continue;
		}
	}
}


const char *Properties::getValue(const char *ext) const
{
	try {
	if (NULL == ext)
		return NULL;
	const PropertiesNode key(ext, ""), *key_ptr = &key;
	const PropertiesNode **n = (const PropertiesNode **) nodes.search(&key_ptr, cmp);
	return NULL == n? NULL: (*n)->getValue();
	} catch(...) { throw; }
}


void Properties::showContents() const
{
	try {
	const size_t N = nodes.getCount();
	const PropertiesNode *n;

	for(size_t i = 0; i < N; ++i) {
		n = (const PropertiesNode *) nodes[i];
		printf("%-8s\t%s\n", n->getKey(), n->getValue());
	}
	} catch(...) { throw; }
}


void Properties::add(const char *key, const char *value)
{
	try {
	PropertiesNode *n = new PropertiesNode(key, value);
	if (NULL == n)
		throw SystemException(SString("'new PropertiesNode(\"%s\", \"%s\")' failed!", key, value));
	nodes.add(n);
	nodes.sort(cmp);
	} catch(...) { throw; }
}
