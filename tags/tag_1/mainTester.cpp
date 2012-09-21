#include <iostream>

#include "Tester.h"

int main(int argc, char**argv)
{
    if(argc < 2)
    {
	std::cerr << "argv1: testing file name\n";
	return 1;
    }
    Tester test(argv[1]);
    test.addResourceName("fopen");
    test.startTest();
    return 0;
}
