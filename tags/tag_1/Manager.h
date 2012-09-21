#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <string>
#include <vector>

#include "Bigbool.h"
#include "ResourceSettings.h"


#include <iostream>


using namespace std;

class Manager
{
    private:
	vector<ResourceSettings> 	m_vectorResources;
	vector<int>			m_vectorNumbValues;
	static Manager 			m_manInstance;

    protected:
        Manager();
        Manager(ResourceSettings *, int);
	~Manager();	
    public:
	static Manager &getInstance();
        void addResource(ResourceSettings);
        int getValue(string);
};

#endif
