#ifndef __BASICOBJECTWORLDINFOMANAGER_H__
#define __BASICOBJECTWORLDINFOMANAGER_H__

#include "common_header.h"
#include "TimeFrameScheduledObject.h"


class BasicObjectWorldInfoReport
{
public:
	int	nObjId; 
	// ...
};


class BasicObjectWorldInfoManager : public TimeFrameScheduledObject
{
public:
	BasicObjectWorldInfoManager(BasicObject* a_pObj);
	virtual ~BasicObjectWorldInfoManager();

	void m_do();

	void m_setNumFramesInMemory(int a_nFrameNum);

	// TODO: to fill
	inline BasicObjectWorldInfoReport*
		m_generateWorldInfoReport() {}
    
protected:
	vector<BasicObjectWorldInfoFrame*>	v_vFrames;

protected:
	BasicObject*						v_pObj;	

protected:
	int									v_nNumFramesInMemory;	

protected:
    /**
     * Median value of friend units in visibility area.
     */
	int									v_nMedianFriendsNo;

	/**
     * Median value of enemy units in visibility area.
     */
	int									v_nMedianEnemiesNo;

	/**
     * Median value of static units units in visibility area.
	 */
	int									v_nMedianStaticUnitsNo;

	// other statistics...

public:

	static const int 					DEFAULT_NUM_FRAMES_IN_MEMORY;


};

#endif // __BASICOBJECTWORLDINFOMANAGER_H__
