#ifndef _CATCH_H_
#define _CATCH_H_

#include <iostream>
#include <stdio.h>
#include <string>

#include "Manager.h"
#include "ResourceSettings.h"

#define fopen(path, mode) catch_fopen(path, mode)

const string fopenResourceName = "fopen";

const string checkFileName = "check_file";

using namespace std;

FILE *catch_fopen(const char *path, const char *mode);
void catchStart(int, char**);
void catchStop();

#endif
