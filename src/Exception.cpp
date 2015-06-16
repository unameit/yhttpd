#include "Exception.h"

void Exception::init(const SString &details)
{
	string = getName();
	string += ": ";
	string += details;
}
