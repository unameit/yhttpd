#include "Configuration.h"
#include "Exception.h"
#include "SString.h"

Configuration::Configuration(const char *filename) :
	Properties(filename)
{
	;
}


const char *Configuration::getValue(const char *key) const
{
	const char *value = Properties::getValue(key);
	if (NULL == value)
		throw Exception(SString("No \"%s\" key was found in configuration file!", key));
	return value;
}
