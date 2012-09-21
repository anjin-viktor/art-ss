#include "Manager.h"

Manager Manager::m_manInstance;

Manager &Manager::getInstance()
{
    return m_manInstance;
}

Manager::Manager()
{
}

Manager::~Manager()
{
}


Manager::Manager(ResourceSettings *resources, int n)
{
    for(int i=0; i<n; i++)
	m_vectorResources[i] = resources[i];
    m_vectorNumbValues.insert(m_vectorNumbValues.begin(), 0, n);
}

void Manager::addResource(ResourceSettings obj)
{
    m_vectorResources.push_back(obj);
    m_vectorNumbValues.push_back(0);
}

int Manager::getValue(string strResourceName)
{
    int i=0;

    for(vector<ResourceSettings>::iterator itr = m_vectorResources.begin(); itr != m_vectorResources.end(); i++, itr++)
	if(itr -> name == strResourceName)
	{
	    m_vectorNumbValues[i] = m_vectorNumbValues[i] + 1;
	    if(m_vectorNumbValues[i] - 1 >= itr -> boolSettings.size())
		return 1;
	    return (itr -> boolSettings.get(m_vectorNumbValues[i] - 1));
	}
    return 1;
}
