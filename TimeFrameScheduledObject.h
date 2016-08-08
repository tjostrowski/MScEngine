#ifndef __TIMEFRAMESCHEDULEDOBJECT_H__
#define __TIMEFRAMESCHEDULEDOBJECT_H__

#include "common_header.h"


class TimeFrameScheduledObject
{
public:
	inline TimeFrameScheduledObject(const int a_nFrameGap = UNDEFINED) :
		v_nFrameGap(a_nFrameGap), v_nLastExecuted(UNDEFINED) {}
	inline virtual ~TimeFrameScheduledObject() {}
    
	inline void	m_setFrameGap(const int a_nFrameGap) { v_nFrameGap = a_nFrameGap; }
	inline int 	m_getFrameGap() { return v_nFrameGap; }

	bool		m_tryToScheduledObjAtTimeFrame();

protected:
	int 		v_nFrameGap;
	int			v_nLastExecuted;
};

#endif // __TIMEFRAMESCHEDULEDOBJECT_H__
