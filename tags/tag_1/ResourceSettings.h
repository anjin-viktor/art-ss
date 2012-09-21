#ifndef _RESOURCE_SETTINGS_H_
#define _RESOURCE_SETTINGS_H_

#include <string>
#include "Bigbool.h"

using namespace std;

class ResourceSettings
{
    public:
	string	name;
	Bigbool boolSettings;
	ResourceSettings(string, Bigbool);
};

#endif
