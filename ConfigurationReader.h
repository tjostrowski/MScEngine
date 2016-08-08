#ifndef __CONFIGURATIONREADER_H__
#define __CONFIGURATIONREADER_H__

#include "common_header.h"

typedef void (ConfigurationReader::*HANDLER_PTR)(char*);


class ConfigurationReader
{
public:
	ConfigurationReader(const char* a_strFileName);
	virtual ~ConfigurationReader();

	void 	m_readConfiguration(vector<BasicObject*>& a_rvObjs);

protected:

	string	v_strFileName;
};

#endif // __CONFIGURATIONREADER_H__
