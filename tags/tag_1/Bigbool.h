#ifndef _BIGBOOL_H_
#define _BIGBOOL_H_

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <cstring>

#define base unsigned int

const int sz = sizeof(base);
const int sz_bit = sz * 8;

class Bigbool
{
    private:
        base *Bool;
        int size_base, size_bit;
        void  new_memory();
    public:
        Bigbool(){ Bool = NULL; size_base = size_bit = 0; };
        Bigbool(int);
        Bigbool(const char *);
	Bigbool(const Bigbool &);
        ~Bigbool(){ if(Bool) delete [] Bool; }
        Bigbool operator =(const Bigbool &);
	int get(int n);
        void print(char *, int);
        void set(int, int);
	unsigned int size();
	void bust();
};

#endif
