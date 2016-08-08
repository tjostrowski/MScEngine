#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

#include "common_header.h"


enum BEHAVIOR_IDS
{
	ATTACK_BEHAVIOR_ID = 0,
	ESCAPE_BEHAVIOR_ID,
	PATROL_BEHAVIOR_ID,
	TRACE_BEHAVIOR_ID,
};

class Behavior
{
public:
	Behavior(BasicObject* a_pObj);
	virtual ~Behavior();

	/**
     * Behavior changes sths in parent object.
     * So as to BasicoObject has to invoke only
     * getNextStep
	 */
	virtual void		m_do() = 0;

	inline void 		m_startBehavior()
	{
		__startBehavior();
	}
	inline void			m_stopBehavior()
	{
		v_bIsAssignedToStop = true;
		__stopBehavior();
	}	
	inline void			m_resumeBehavior()
	{
		__resumeBehavior();
		v_bIsStopped = false;
	}
	inline void			m_haltBehavior()
	{
		v_bIsAssignedToHalt = true;
		__haltBehavior();
	}
	
	inline bool			m_isActive() 
	{ 
		return !v_bIsStopped && !v_bIsHalted &&
			   !v_bIsAssignedToStop && !v_bIsAssignedToHalt;
	}
    
	void				m_assocActivity(int a_nActivityId, TextureObject& a_rObj);

	inline bool			m_isReconfiguredToBehavior() { return v_bReconfiguredToBehavior; }

	virtual BITMAP*		m_getCurrentAnimationFrame() = 0;

	inline int			m_getId() { return v_nId; }

	inline virtual void	m_reconfigureToNewParams() {}

	// returns if texture object was found
	bool				m_getTextureObj( TextureObject& a_txtObj, const int a_nActivity = 0);

	virtual cpt3f 		m_getNewPos() { return INVALID_POS;  }
    
	inline CtrlObject* 	m_getCtrlObj() { return v_pCtrlObj; }
	
protected:
	virtual void		__startBehavior() {}
    virtual void		__haltBehavior()
	{
		v_bIsHalted = true;
	}
	virtual void		__stopBehavior()
	{
		v_bIsStopped = true;
	}
	virtual void 		__resumeBehavior() {}

 protected:
	BasicObject*		v_pObj;
	int					v_nId;

	typedef map<int, TextureObject> ACTIV2TXT_MAP;
	/**
     * Activities associated with specific behavior.
     * E.g. => combat.
     * 
     */
	map<int /* ACTIVITY ID */, TextureObject /* animation associated with >> activity << */> v_mapActivities;

	bool 				v_bReconfiguredToBehavior;

public:
	inline bool			m_isStopped() { return v_bIsStopped; }
	inline bool			m_isAssignedToStop() { return v_bIsAssignedToStop; }
    inline bool			m_isHalted() { return v_bIsHalted; }
	inline bool			m_isAssignedToHalt() { return v_bIsAssignedToHalt; }
    
protected:
	// stopping or halting behavior could take a while
	// so additional boolean flags introduced
	bool				v_bIsAssignedToStop;
	bool				v_bIsStopped;
	bool				v_bIsAssignedToHalt;
	bool				v_bIsHalted;


	CtrlObject*			v_pCtrlObj;

};

#endif // __BEHAVIOR_H__
