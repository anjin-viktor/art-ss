#ifndef _TESTER_H_
#define _TESTER_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Bigbool.h"



using namespace std;


const string checkFileName = "test_file";


class Tester
{
    private:

	string			m_strName;
	vector<Bigbool>		m_vectBool;
	vector<string>		m_vectResourceName;
	int			m_currentNumb;
	int 			m_maxNumb;

	void error();
    public:
	Tester(string strName = "");
	
	void startTest();
	void setName(string);
	void addResourceName(string);
	string getName();
	
	
};


#endif

