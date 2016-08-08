#ifndef __BASICOBJECTDECISIONMANAGER_H__
#define __BASICOBJECTDECISIONMANAGER_H__

#include "common_header.h"
#include "TimeFrameScheduledObject.h"


class BasicObjectDecisionManager : public TimeFrameScheduledObject
{
public:
	BasicObjectDecisionManager(BasicObject* a_pObj);
	virtual ~BasicObjectDecisionManager();

	void m_addWorldInfoReport(BasicObjectWorldInfoReport* a_pReport);
	void m_addParamsReport(BasicObjectParamsReport* a_pReport);

	void m_do();

	/**
     * How often (every each? frame) manager will be executed.
	 */
	inline void m_setFrameRate(int a_nFrameRate)
	{
		v_nFrameRate = a_nFrameRate;
	}

	inline int m_getFrameRate()
	{
		return v_nFrameRate;
	}

protected:
	BasicObject*						v_pObj;

	vector<BasicObjectWorldInfoReport*>	v_vWorldInfoReports;
	vector<BasicObjectParamsReport*>	v_vParamsReport;

	int									v_nFrameRate;

public:
	static const int 					DEFAULT_FRAME_RATE;

};

#endif // __BASICOBJECTDECISIONMANAGER_H__
