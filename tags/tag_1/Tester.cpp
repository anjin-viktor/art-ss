#include "Tester.h"

Tester::Tester(string str)
{
    m_strName = str;
    m_currentNumb = 0;
    m_maxNumb = 0;
}

void Tester::setName(string str)
{
    m_strName = str;
}

string Tester::getName()
{
    return m_strName;
}

void Tester::addResourceName(string strName)
{
    Bigbool b(1);
    m_vectBool.push_back(b);
    m_vectResourceName.push_back(strName);
    m_maxNumb++;
}

void Tester::startTest()
{
    char *str = new char[4096];
    char *strTmp = new char[32];

    for(;;)
    {
	string command = m_strName + " ";
	int i=0;
	for(vector<string>::iterator itr = m_vectResourceName.begin(); itr != m_vectResourceName.end(); itr++)
	{
	    m_vectBool[i].print(str, 4096);
	    command += *itr + " " + string(str) + " ";
	}
	system(command.c_str());

	m_vectBool[m_currentNumb].bust();
	m_currentNumb %= m_maxNumb;
	FILE *fp = fopen(checkFileName.c_str(), "r");
	if(fp == NULL)
	{
	    std::cerr << "Tester: error! Can`t open check file " << checkFileName << endl;
	    exit(1);
	}
	fgets(strTmp, 32, fp);
	if(strcmp(str, "true"))
	    error();
	fclose(fp);
	fp = fopen(checkFileName.c_str(), "w");
	if(fp == NULL)
	{
	    std::cerr << "Tester: error! Can`t open check file " << checkFileName << endl;
	    exit(1);
	}
	fclose(fp);
    }
    delete [] str;
    delete [] strTmp;
}

void Tester::error()
{
    std::cerr << "error\n";

    string strMess = "";
    char *tmpStr = new char[4096];
    strMess += "-------------\n";
    vector<Bigbool>::iterator iterBool = m_vectBool.begin();
    vector<string>::iterator iterName = m_vectResourceName.begin();
    for(; iterBool != m_vectBool.end(); iterBool++, iterName++)
    {
	strMess += *iterName + ": ";
	iterBool -> print(tmpStr, 4096);
	strMess += tmpStr;
	strMess += "\n";
    }
    std::cerr << strMess;
    delete [] tmpStr;
}
