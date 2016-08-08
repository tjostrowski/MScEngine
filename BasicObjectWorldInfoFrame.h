#ifndef __BASICOBJECTWORLDINFOFRAME_H__
#define __BASICOBJECTWORLDINFOFRAME_H__

#include "common_header.h"
#include "TimeFrameScheduledObject.h"


class BasicObjectWorldInfoFrame
{
public:
	BasicObjectWorldInfoFrame(ulong a_nFrameNo);
	virtual ~BasicObjectWorldInfoFrame();

	void 					initAndRecalculate(vector<BasicObject*>& a_rvObjs);

	inline ulong			m_getFrameNo() { return v_nFrameNo; }
	inline int				m_getFriendsNo() { return v_nFriendsNo; }
	inline int				m_getEnemiesNo() { return v_nEnemiesNo; }
	inline int 				m_getStaticUnitsNo() { return v_nStaticUnitsNo;  }	
    
protected:
	ulong					v_nFrameNo;

	/**
     * Number of friend units in visibility area.
	 */
	int 					v_nFriendsNo;

	/**
     * Number of enemy units in visibility area.
     */
	int						v_nEnemiesNo;

	/**
     * Number of static units in visibility area.
	 */
	int						v_nStaticUnitsNo; 

	// other statistics...


	/**
     * List of objects in visibility area.
	 */
	vector<BasicObject*>	v_vObjs;


};

#endif // __BASICOBJECTWORLDINFOFRAME_H__
