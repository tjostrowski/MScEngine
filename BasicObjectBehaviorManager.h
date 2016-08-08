#ifndef __BASICOBJECTBEHAVIORMANAGER_H__
#define __BASICOBJECTBEHAVIORMANAGER_H__

#include "common_header.h"
#include "TimeFrameScheduledObject.h"


class BasicObjectBehaviorManager : public TimeFrameScheduledObject
{
public:
	BasicObjectBehaviorManager(BasicObject* a_pObj);
	virtual ~BasicObjectBehaviorManager();

	// TODO: to fill
	/**
	 * 
	 * 
     * @return Behavior* => 0 if no good behavior exists
	 */
	inline Behavior* 	m_getActualBehavior() { return v_pActualBehavior;  }
    
    Behavior* 			m_createPlainNewBehavior(const int a_nBehaviorId);

	// ONE BEHAVIOR IN ONE TIME FRAME
	/**
     * ...
     * 
	 * @param a_pBehavior
     * 
     * For all below:
     * @return bool if operation on behavior was succesful
	 */
	bool				m_startNewBehavior(Behavior* a_pBehavior);
	bool				m_stopActualBehavior();
	bool				m_resumeActualBehavior();
	bool				m_haltActualBehavior();


	void				m_do();


protected:
	BasicObject*		v_pObj;
	Behavior*			v_pActualBehavior;	

	vector<Behavior*>	v_vBehaviorsToStart;

};

typedef BasicObjectBehaviorManager UnitBehaviorMAN;

#endif // __BASICOBJECTBEHAVIORMANAGER_H__
