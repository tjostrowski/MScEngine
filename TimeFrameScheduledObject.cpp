#include "common.h"


bool TimeFrameScheduledObject::m_tryToScheduledObjAtTimeFrame()
{
	if (UNDEFINED == v_nFrameGap || v_nFrameGap <= 0)
	{
		return false;
	}

	BasicConfiguration* pConf = BasicConfiguration::m_getBasicConfiguration();
	ulong nFrameNo = pConf->m_getFrameNo();

	if (nFrameNo % v_nFrameGap == 0)
	{
		v_nLastExecuted = nFrameNo;
		return true;
	}

	// case when executed with delay
	if (UNDEFINED != v_nLastExecuted &&
		(nFrameNo - v_nLastExecuted) >= v_nFrameGap)
	{
		v_nLastExecuted = nFrameNo;
		return true;
	}

	return false;
}
