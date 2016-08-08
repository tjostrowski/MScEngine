#ifndef __CONFIGURATIONWRITER_H__
#define __CONFIGURATIONWRITER_H__

#include "common_header.h"


class ConfigurationWriter
{
public:
	ConfigurationWriter(const char* a_strFileName);
	virtual ~ConfigurationWriter();

	void 						m_genConfiguration();

	void						m_genGameObjects(vector<BasicObject*>& a_rvObjs, uint& a_numObjs /* out */);

protected:
    string						v_strFileName;
};

#endif // __CONFIGURATIONWRITER_H__
