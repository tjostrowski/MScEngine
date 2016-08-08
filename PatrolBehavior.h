#ifndef __PATROLBEHAVIOR_H__
#define __PATROLBEHAVIOR_H__

#include "common_header.h"


class PatrolBehavior : public Behavior
{
public:
	PatrolBehavior(BasicObject* a_pObj);
	virtual ~PatrolBehavior();

	void				m_do();

	void				m_setPatrolPoints(vector<INT_COORDS>& a_rvPatrolPoints);
	void				m_addPatrolPoint(INT_COORDS a_patrolPoint);

	BITMAP*				m_getCurrentAnimationFrame();

protected:
	bool 				m_assertIntegrity();

protected:
	/*inline void			__startBehavior() {}
    inline void			__haltBehavior() {}
	inline void			__stopBehavior() {}
    inline void			__resumeBehavior() {}*/

protected:
	vector<INT_COORDS>	v_vPatrolPoints;

	int					v_nActualPatrolPointIndex; 

	int					v_nActivity;

public:
	static const int 	RUN_ACTIVITY;



};

#endif // __PATROLBEHAVIOR_H__
